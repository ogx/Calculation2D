package ogx.control;

import org.json.JSONException;
import org.json.JSONObject;

public class Command {
	
	final String name;
	final JSONObject params;
	
	public Command(String name, JSONObject params) {
		this.name = name;
		this.params = params;
	}
	
	public JSONObject format() {
		JSONObject json = new JSONObject();
		try {
			json.put("cmd", name);
			json.put("params", params);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return json;
	}
}
