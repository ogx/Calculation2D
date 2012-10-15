package ogx.view;

import java.io.File;

import javax.swing.filechooser.FileFilter;

public class ImageFilter extends FileFilter {
	
	private final String[] extensions = {"jpg", "bmp", "png", "jpeg", "tif", "tiff", "ixml"};

	@Override
	public boolean accept(File f) {
		if (f.isDirectory()) return true;
		for (int i = 0; i < extensions.length; ++i) {
			if (f.getName().toLowerCase().endsWith(extensions[i])) return true;
		}
		return false;
	}

	@Override
	public String getDescription() {
		return "Image file [jpg, bmp, png, tif, ixml]";
	}

}
