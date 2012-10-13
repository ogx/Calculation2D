package ogx.view;

import java.io.File;

import javax.swing.filechooser.FileFilter;

public class ImageFilter extends FileFilter {
	
	private final String[] extensions = {"jpg", "bmp", "png", "gif", "jpeg"};

	@Override
	public boolean accept(File f) {
		for (int i = 0; i < extensions.length; ++i) {
			if (f.getName().toLowerCase().endsWith(extensions[i])) return true;
		}
		return false;
	}

	@Override
	public String getDescription() {
		return "Image file";
	}

}
