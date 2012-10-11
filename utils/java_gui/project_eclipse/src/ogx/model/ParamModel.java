package ogx.model;

import org.json.JSONException;
import org.json.JSONObject;

public class ParamModel {
	
	public class ParamType { 
		public static final int STRING = 0; 
		public static final int INT = 1;
		public static final int DOUBLE = 2;
		public static final int BOOL = 3;
		public static final int IMAGENAME = 4;
		public static final int LAYERNAME = 5;
		public static final int UNKNOWN = 6;
		
		final int val;
		
		ParamType(int val) {
			this.val = val;
		}
		
		public int get() {
			return val;
		}
		
		@Override
		public String toString() {
			switch (val) {
			case STRING: return "STRING";
			case INT: return "INT";
			case DOUBLE: return "DOUBLE";
			case BOOL: return "BOOL";
			case IMAGENAME: return "IMAGENAME";
			case LAYERNAME: return "LAYERNAME";
			default: return "UNKNOWN";
			}
		}
		
		@Override
		public boolean equals(Object obj) {
			if (obj.getClass().equals(this.getClass())) {
				if (((ParamType)obj).get() == this.val) return true;
			}
			return false;
		}
	}
		
	
	final JSONObject param;

	public ParamModel(JSONObject serial_param) {
		param = serial_param;
	}
	
	public String getName() {
		String result = "";
		try {
			result = param.getString("name");
		}
		catch (JSONException e) {
			e.printStackTrace();
		}
		return result;
	}
	
	public ParamType getType() {
		ParamType result;
		String type_string = "";
		try {
			type_string = param.getString("type");
		}
		catch (JSONException e) {
			e.printStackTrace();
		}
		if (type_string.equals("string")) result = new ParamType(ParamType.STRING);
		else if (type_string.equals("int")) result = new ParamType(ParamType.INT);
		else if (type_string.equals("real")) result = new ParamType(ParamType.DOUBLE);
		else if (type_string.equals("bool")) result = new ParamType(ParamType.BOOL);
		else if (type_string.equals("image")) result = new ParamType(ParamType.IMAGENAME);
		else if (type_string.equals("layer")) result = new ParamType(ParamType.LAYERNAME);
		else result = new ParamType(ParamType.UNKNOWN);
		return result;
	}
	
	public String getValue() {
		String result = "";
		try {
			result = param.getString("value");
//			if (getType().get() == ParamType.BOOL) {
//				if (param.getBoolean("value")) result = "true";
//				else result = "false";
//			}
//			else if (getType().get() == ParamType.INT) {
//				result = String.valueOf(param.getInt("value"));
//			}
//			else if (getType().get() == ParamType.DOUBLE) {
//				result = String.valueOf(param.getDouble("value"));
//			}
//			else {
//				result = param.getString("value");
//			}
		}
		catch (JSONException e) {
			e.printStackTrace();
		}
		return result;
	}
	
	public void setValue(String value) {
		try {
			param.put("value", value);
//			if (value.equals("true")) {
//				param.put("value", true);
//			}
//			else if (value.equals("false")) {
//				param.put("value", false);
//			}
//			else if (getType().get() == ParamType.INT) {
//				param.put("value", Integer.parseInt(value));
//			}
//			else if (getType().get() == ParamType.DOUBLE) {
//				param.put("value", Double.parseDouble(value));
//			}
//			else {
//				param.put("value", value);
//			}
		}
		catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
}
