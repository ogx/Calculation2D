package ogx.control;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.nio.ByteBuffer;
import java.util.Arrays;

import javax.swing.tree.DefaultMutableTreeNode;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import ogx.model.ImagesStruct;
import ogx.model.MethodListModel;
import ogx.model.MethodModel;

public class Control {
	
	class Status {
		private final String message;
		private final boolean success;
		public Status(String message, boolean success) {
			this.message = message;
			this.success = success;
		}
		public String getMessage() {
			return message;
		}
		public boolean isSuccess() {
			return success;
		}
	}
	
	final ImagesStruct images_struct;
	MethodListModel method_list = null;
	Process calc2d = null;
	InputStream stdout = null;
	OutputStream stdin = null;
	BufferedReader reader = null;
	BufferedWriter writer = null;
	
	public Control(ImagesStruct images_struct, String calc2d_path) {
		this.images_struct = images_struct;
		method_list = new MethodListModel();
		ProcessBuilder proc_build = new ProcessBuilder(calc2d_path);
		try {
			calc2d = proc_build.start();
		} catch (IOException e) {
			calc2d = null;
			e.printStackTrace();
		}
		if (calc2d != null) {
			stdin = calc2d.getOutputStream();
			stdout = calc2d.getInputStream();
			reader = new BufferedReader(new InputStreamReader(stdout));
			writer = new BufferedWriter(new OutputStreamWriter(stdin));
		}
	}
	
	public MethodListModel getMethods() {
		sendCommand(new Command("getmethods", new JSONObject()));
		JSONArray methods = null;
		try {
			methods = readResponse().getJSONArray("methods");
			for (int i = 0; i < methods.length(); ++i) {
				method_list.addMethodModel(new MethodModel((JSONObject)methods.get(i)));
			}
		} catch (JSONException e) {
			e.printStackTrace();
			return null;
		}
		return method_list;
	}
	
	public boolean disconnectCalcServer() {
		sendCommand(new Command("finalize", new JSONObject()));
		if (getStatus().isSuccess()) return true;
		return false;
	}
	
	public boolean runMethod(MethodModel current_method_model) {
		boolean result = false;
		String message;
		// synchronize regions of interest in all images
		sendCommand(new Command("sync_roi", images_struct.serialize()));
		result = getStatus().isSuccess();
		if (result && current_method_model != null) {
			// send command "run"
			sendCommand(new Command("run", current_method_model.getSerialMethod()));
			Status current_status = getStatus();
			//System.err.println(current_status.getMessage());
			if (result = current_status.isSuccess()) {
				do {
					sendCommand(new Command("getstatus", new JSONObject()));
					try {
						Thread.sleep(100);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					current_status = getStatus();
					message = current_status.getMessage();
					//System.err.println(message);
				}
				while (!message.equals("finished"));
				if (current_status.isSuccess()) {
					result = true;
				}
				sendCommand(new Command("sync_images", images_struct.serialize()));
				JSONObject response = readResponse();
				if (response == null) return false;
				readImages(response);
			}
		}
		return result;
	}
	
	public void loadImage(String path) {
		JSONObject params = new JSONObject();
		try {
			params.put("path", path);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		sendCommand(new Command("load", params));
		JSONObject response = readResponse();
		if (response == null) return;
				
		readImages(response);	
	}
	
	private boolean readImages(JSONObject missing_struct) {
		
		// communication protocol for reading binary data
		// 1. send JSON with image request
		// 2. receive rgb binary data
		// 3. send confirmation command and receive response
		// 4. repeat 5-7 for each layer:
		// 5. send JSON with layer request
		// 6. receive layer binary data
		// 7. send confirmation command and receive response
		boolean result = false;
		JSONArray images_to_read = null;
		JSONArray layers_to_read = null;
		DefaultMutableTreeNode current_image_node = null;
		DefaultMutableTreeNode current_layer_node = null;
		if (missing_struct != null) {
			try {
				// read missing images
				images_to_read = missing_struct.getJSONArray("images_structure");
				for (int i = 0; i < images_to_read.length(); ++i) {
					String name = images_to_read.getJSONObject(i).getString("name");
					int width = images_to_read.getJSONObject(i).getInt("width");
					int height = images_to_read.getJSONObject(i).getInt("height");
					int id = images_to_read.getJSONObject(i).getInt("id");
					current_image_node = images_struct.findImageNode(id);
					if (current_image_node == null) {
						current_image_node = images_struct.addImage(name, width, height, id);
					}
					
					ByteBuffer byte_buffer = ByteBuffer.allocate(width*height*4);				
					// 1.
					JSONObject img_request = new JSONObject(images_to_read.getJSONObject(i), Arrays.asList("name", "id", "width", "height").toArray(new String[0]));
					sendCommand(new Command("query_image", img_request));	
					// 2.
					if (readBinaryData(byte_buffer.array(), width*height*4)) {
						images_struct.fillImage(current_image_node, byte_buffer.array(), width, height, 4);
						// 4.
						layers_to_read = images_to_read.getJSONObject(i).getJSONArray("layers");
						for (int j = 0; j < layers_to_read.length(); ++j) {
							name = layers_to_read.getJSONObject(j).getString("name");
							id = layers_to_read.getJSONObject(j).getInt("id");
							current_layer_node = images_struct.findLayerNode(current_image_node, id);
							if (current_layer_node == null) {
								current_layer_node = images_struct.addLayer(current_image_node, name, id);
							}
							
							// 5. confirmation is a request for another piece of data
							JSONObject layer_request = new JSONObject(layers_to_read.getJSONObject(j), Arrays.asList("name", "id", "owner").toArray(new String[0]));
							sendCommand(new Command("query_layer", layer_request));
							// 6.
							if (readBinaryData(byte_buffer.array(), width*height)) {
								images_struct.fillLayer(current_layer_node, byte_buffer.array(), width, height);
							}
						}
					}
				}
				// 3. confirmation that all data has been received
				sendCommand(new Command("data_received", new JSONObject()));
				result = getStatus().isSuccess();
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}
		return result;
	}

	public Status getStatus() {
		JSONObject result = readResponse();
		Status status = null;
		if (result.has("status")) {
			try {
				status = new Status(result.getString("status"), statusSuccess(result));
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}
		else {
			status = new Status("unknown", false);
		}
		return status;
	}
	
	private void sendCommand(Command cmd) {
		try {
			writer.write(cmd.format().toString() + "\n");
			writer.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private JSONObject readResponse() {
		JSONObject result = null;
		try {
			result = new JSONObject(reader.readLine());
			if (result.has("error")) {
				System.err.println("Error received from client: " + result.getString("error"));
				return null;
			}
		} catch (JSONException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} 
		return result;
	}
	
	private boolean readBinaryData(byte[] buffer, int size) {
		int bytes_read = 0;
		int bytes_to_read = size;
		while (bytes_to_read > 0) {
			try {
				bytes_read = stdout.read(buffer, bytes_read, size - bytes_read);
			} catch (IOException e) {
				e.printStackTrace();
				return false;
			}
			bytes_to_read -= bytes_read;
		}
		return true;
	}
	
	private boolean statusSuccess(JSONObject status_response) {
		boolean result = false;
		if (status_response != null) {
			if (status_response.has("success")) {
				try {
					result = status_response.getBoolean("success");
				} catch (JSONException e) {
					e.printStackTrace();
				}
			}
		}
		return result;
	}
	
}
