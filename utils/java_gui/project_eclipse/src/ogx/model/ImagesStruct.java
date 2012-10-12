package ogx.model;

import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.util.Vector;

import javax.swing.tree.DefaultMutableTreeNode;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class ImagesStruct {
	
	DefaultMutableTreeNode root = new DefaultMutableTreeNode("Images structure");
	
	public ImagesStruct() {
		
	}
	
	public DefaultMutableTreeNode addImage(String name, int width, int height, int id) {
		DefaultMutableTreeNode result = null;
		if (findImageNode(name) == null) {
			root.add(new DefaultMutableTreeNode(new ImageModel(name, new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB), id)));
			result = (DefaultMutableTreeNode) root.getLastChild();
			addLayer(result, "R", 0);
			addLayer(result, "G", 0);
			addLayer(result, "B", 0);
		}
		return result;
	}
	
	public DefaultMutableTreeNode addLayer(DefaultMutableTreeNode image_node, String layer_name, int id) {
		DefaultMutableTreeNode result = null;
		if (image_node != null) {
			// ensure two level tree (root->image->layer)
			if (image_node.getLevel() == 1) {
				if (findLayerNode(image_node, layer_name) == null) {
					int width = ((ImageModel)image_node.getUserObject()).getImage().getWidth();
					int height = ((ImageModel)image_node.getUserObject()).getImage().getHeight();
					image_node.add(new DefaultMutableTreeNode(new ImageModel(layer_name, new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB), id)));
					result = (DefaultMutableTreeNode) image_node.getLastChild();
				}
			}
		}
		return result;
	}
	
	public void fillImage(DefaultMutableTreeNode image_node, byte[] buffer, int width, int height, int pixel_size) {
		BufferedImage current_image = null;
		if (image_node != null) {
			// ensure two level tree (root->image->layer)
			if (image_node.getLevel() == 1) {
				current_image = ((ImageModel)(image_node.getUserObject())).getImage();
				if (current_image.getWidth() == width && current_image.getHeight() == height) {
					Rectangle current_roi = ((ImageModel)(image_node.getUserObject())).getROI();
					int pixel;
					for (int i = current_roi.x; i < current_roi.width; ++i) {
						for (int j = current_roi.y; j < current_roi.height; ++j) {
							pixel = 0xFF000000 | 
									((int)buffer[pixel_size*(j*width + i) + 2] << 16 & 0x00FF0000) | 		// red
									((int)buffer[pixel_size*(j*width + i) + 1] << 8  & 0x0000FF00) | 		// green
									((int)buffer[pixel_size*(j*width + i)    ]       & 0x000000FF);			// blue
							current_image.setRGB(i, j, pixel);
						}
					}
					((ImageModel)(image_node.getUserObject())).calcHistogram();
					// fill special R,G,B layers
					DefaultMutableTreeNode layerR = findLayerNode(image_node, "R");
					DefaultMutableTreeNode layerG = findLayerNode(image_node, "G");
					DefaultMutableTreeNode layerB = findLayerNode(image_node, "B");
					if (layerR != null && layerG != null && layerB != null) {
						byte[] layer_buffer = new byte[width*height]; 
						for (int i = 0; i < width*height; ++i) {
							layer_buffer[i] = buffer[pixel_size*i+2];
						}
						fillLayer(layerR, layer_buffer, width, height);
						for (int i = 0; i < width*height; ++i) {
							layer_buffer[i] = buffer[pixel_size*i+1];
						}
						fillLayer(layerG, layer_buffer, width, height);
						for (int i = 0; i < width*height; ++i) {
							layer_buffer[i] = buffer[pixel_size*i];
						}
						fillLayer(layerB, layer_buffer, width, height);
						layer_buffer = null;
					}
				}
			}
		}
	}
	
	public void fillLayer(DefaultMutableTreeNode layer_node, byte[] buffer, int width, int height) {
		BufferedImage current_image = null;
		if (layer_node != null) {
			// ensure two level tree (root->image->layer)
			if (layer_node.getLevel() == 2) {
				current_image = ((ImageModel)(layer_node.getUserObject())).getImage();
				Rectangle current_roi = ((ImageModel)(layer_node.getUserObject())).getROI();
				int pixel;
				for (int i = current_roi.x; i < current_roi.width; ++i) {
					for (int j = current_roi.y; j < current_roi.height; ++j) {
						pixel = 0xFF000000 | 
								(buffer[j*width + i] << 16 & 0x00FF0000) | 
								(buffer[j*width + i] << 8  & 0x0000FF00) | 
								(buffer[j*width + i]       & 0x000000FF);
						current_image.setRGB(i, j, pixel);
					}
				}
				((ImageModel)(layer_node.getUserObject())).calcHistogram();
			}
		}
	}
	
	public DefaultMutableTreeNode findImageNode(String image_name) {
		DefaultMutableTreeNode result = null;
		DefaultMutableTreeNode current_node = null;
		for (int i = 0; i < root.getChildCount(); ++i) {
			current_node = (DefaultMutableTreeNode)root.getChildAt(i);
			if (current_node.toString() == image_name) {
				result = current_node;
				break;
			}
		}
		return result;
	}
	
	public DefaultMutableTreeNode findLayerNode(DefaultMutableTreeNode image_node, String layer_name) {
		DefaultMutableTreeNode result = null;
		DefaultMutableTreeNode current_node = null;
		for (int j = 0; j < image_node.getChildCount(); ++j) {
			current_node = (DefaultMutableTreeNode)image_node.getChildAt(j);
			if (current_node.toString() == layer_name) {
				result = current_node;
				break;
			}
		}
		
		return result;
	}
	
	public void removeImage(DefaultMutableTreeNode image_node) {
		if (image_node != null) {
			image_node.removeAllChildren();
			root.remove(image_node);
		}
	}
	
	public void removeLayer(DefaultMutableTreeNode layer_node) {
		if (layer_node != null) {
			layer_node.removeAllChildren();
			DefaultMutableTreeNode parent_image_node = (DefaultMutableTreeNode) layer_node.getParent();
			parent_image_node.remove(layer_node);
		}
	}
	
	public DefaultMutableTreeNode getRoot() {
		return root;
	}

	public Vector<String> getImageNames() {
		Vector<String> image_names = new Vector<String>();
		for (int i = 0; i < root.getChildCount(); ++i) {
			String name = ((DefaultMutableTreeNode)root.getChildAt(i)).getUserObject().toString();
			image_names.add(name);
		}
		return image_names;
	}
	
	public Vector<String> getLayerNames(String image_name) {
		Vector<String> layer_names = new Vector<String>();
		DefaultMutableTreeNode image_node = findImageNode(image_name);
		if (image_node != null) {
			for (int i = 0; i < image_node.getChildCount(); ++i) {
				String name = ((DefaultMutableTreeNode)image_node.getChildAt(i)).getUserObject().toString();
				layer_names.add(name);
			}
		}
		return layer_names;
	}
	
	public JSONObject serialize() {
		JSONArray images_struct_array = new JSONArray();
		for (int i = 0; i < root.getChildCount(); ++i) {
			images_struct_array.put(serializeImage((DefaultMutableTreeNode)root.getChildAt(i)));
		}
		JSONObject result = new JSONObject();
		try {
			result.put("images_structure", images_struct_array);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return result;
	}
	
	public JSONObject serializeImage(DefaultMutableTreeNode image_node) {
		JSONObject result = null;
		ImageModel current_image = null;
		if (image_node != null) {
			// ensure two level tree (root->image->layer)
			current_image = (ImageModel)image_node.getUserObject();
			try {
				result = new JSONObject();
				result.put("name", current_image.getName());
				result.put("id", current_image.getID());
				if (image_node.getLevel() == 1) {
					result.put("width", current_image.getImage().getWidth());
					result.put("height", current_image.getImage().getHeight());
					JSONObject roi_node = new JSONObject();
					roi_node.put("left", current_image.getROI().x);
					roi_node.put("top", current_image.getROI().y);
					roi_node.put("width", current_image.getROI().width);
					roi_node.put("height", current_image.getROI().height);
					result.put("roi", roi_node);
					JSONArray layers_array = new JSONArray();
					for (int i = 0; i < image_node.getChildCount(); ++i) {
						DefaultMutableTreeNode layer_node = (DefaultMutableTreeNode)image_node.getChildAt(i);
						if (layer_node.toString() != "R" && layer_node.toString() != "G" && layer_node.toString() != "B") {
							layers_array.put(serializeImage(layer_node));
						}
					}
					result.put("layers", layers_array);
				}
				
			} catch (JSONException e) {
				e.printStackTrace();
			}		
		}
		return result;
	}

	public DefaultMutableTreeNode findImageNode(int id) {
		DefaultMutableTreeNode result = null;
		DefaultMutableTreeNode current_node = null;
		ImageModel current_image = null;
		for (int i = 0; i < root.getChildCount(); ++i) {
			current_node = (DefaultMutableTreeNode)root.getChildAt(i);
			current_image = (ImageModel)current_node.getUserObject();
			if (current_image.getID() == id) {
				result = current_node;
				break;
			}
		}
		return result;
	}
	
	public DefaultMutableTreeNode findLayerNode(DefaultMutableTreeNode image_node, int id) {
		DefaultMutableTreeNode result = null;
		DefaultMutableTreeNode current_node = null;
		ImageModel current_layer = null;
		for (int j = 0; j < image_node.getChildCount(); ++j) {
			current_node = (DefaultMutableTreeNode)image_node.getChildAt(j);
			current_layer = (ImageModel)current_node.getUserObject();
			if (current_layer.getID() == id) {
				result = current_node;
				break;
			}
		}
		return result;
	}

}
