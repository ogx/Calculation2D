package ogx.model;

import java.util.Vector;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class MethodModel {
	
	JSONObject method = null;

	public MethodModel(JSONObject serial_method) {
		method = serial_method;
	}
	
	public MethodModel(MethodModel source) {
		try {
			this.method = new JSONObject(source.method.toString());
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public String getName() {
		String result = "";
		try {
			result = method.getString("name");
		}
		catch (JSONException e) {
			;
		}
		return result;
	}
	
	public String getID() {
		String result = "";
		try {
			result = method.getString("id");
		}
		catch (JSONException e) {
			;
		}
		return result;
	}
	
	public String getDescription() {
		String result = "";
		try {
			result = method.getString("description");
		}
		catch (JSONException e) {
			;
		}
		return result;
	}
	
	public String getAuthor() {
		String first_name = "";
		String last_name = "";
		try {
			first_name = method.getJSONObject("author").getString("first_name");
			last_name = method.getJSONObject("author").getString("last_name");
		}
		catch (JSONException e) {
			;
		}
		return first_name + " " + last_name;
	}
	
	public Vector<ParamModel> getParams() {
		Vector<ParamModel> result = new Vector<ParamModel>();
		try {
			JSONArray params = method.getJSONObject("params").getJSONArray("in");
			for (int i = 0; i < params.length(); ++i) {
				result.add(new ParamModel(params.getJSONObject(i)));
			}
		}
		catch (JSONException e) {
			
		}
		return result;
	}
	
	public JSONObject getSerialMethod() {
		return method;
	}
	
	@Override
	public String toString() {
		return "[" + getAuthor() + "] " + getName();
	}
}
