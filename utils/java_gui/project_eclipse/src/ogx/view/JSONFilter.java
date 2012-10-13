package ogx.view;

import java.io.File;

import javax.swing.filechooser.FileFilter;

public class JSONFilter extends FileFilter {

	@Override
	public boolean accept(File f) {
		if (f.getName().toLowerCase().endsWith("json")) {
			return true;
		}
		return false;
	}

	@Override
	public String getDescription() {
		return "Text file (*.json)";
	}

}
