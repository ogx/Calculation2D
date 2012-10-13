package ogx.model;

import java.util.Vector;

import javax.swing.ListModel;
import javax.swing.event.ListDataListener;

import org.json.JSONException;
import org.json.JSONObject;

public class MethodListModel implements ListModel {
	
	Vector<MethodModel> list = new Vector<MethodModel>();
	
	public MethodListModel() {
		// it will be created by controller for use by view
		//
		// temporary for testing
//		try {
//			addMethodModel(new MethodModel(new JSONObject("{\"author\":{\"email\":\"j.doe@example.com\",\"first_name\":\"John\",\"id\":0,\"last_name\":\"Doe\"},\"description\":\"Adds a new layer when provided with image, name and default value.\",\"id\":\"{521FEFE6-C9D3-485F-B411-83FCEB407158}\",\"name\":\"Add layer\",\"params\":{\"in\":[{\"name\":\"Image\",\"type\":\"image\",\"value\":\"\"},{\"name\":\"Name\",\"type\":\"string\",\"value\":\"New layer\"},{\"name\":\"Value\",\"type\":\"real\",\"value\":-1.0}],\"out\":[{\"name\":\"Name\",\"type\":\"layer\",\"value\":\"New layer\"},{\"name\":\"Value\",\"type\":\"real\",\"value\":-1.0}]}}")));
//			addMethodModel(new MethodModel(new JSONObject("{\"author\":{\"email\":\"j.doe@example.com\",\"first_name\":\"John\",\"id\":0,\"last_name\":\"Doe\"},\"description\":\"Flips selected image horizontally and/or vertically to a new image based on ROI.\",\"id\":\"{BA728F54-6D6B-4478-AFAC-C4367BDB494F}\",\"name\":\"Flip Image (example)\",\"params\":{\"in\":[{\"name\":\"Image\",\"type\":\"image\",\"value\":\"\"},{\"name\":\"Horizontally?\",\"type\":\"bool\",\"value\":true},{\"name\":\"Vertically?\",\"type\":\"bool\",\"value\":true}],\"out\":[{\"name\":\"Flipped image\",\"type\":\"image\",\"value\":\"\"}]}}")));
//		}
//		catch (JSONException e) {
//			System.out.println("error parsing json");
//		}
		
	}
	
	public void addMethodModel(MethodModel methodmodel) {
		list.add(methodmodel);
		// it will be invoked by the controller after receiving answer for "getmethods" request
	}

	@Override
	public void addListDataListener(ListDataListener arg0) {

	}

	@Override
	public Object getElementAt(int arg0) {
		Object result = null;
		try {
			result = list.get(arg0);
		}
		catch (IndexOutOfBoundsException e) {
			
		}
		return result;
	}

	@Override
	public int getSize() {
		return list.size();
	}

	@Override
	public void removeListDataListener(ListDataListener arg0) {

	}

}
