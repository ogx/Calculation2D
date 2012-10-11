package ogx.model;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.awt.image.Raster;
import java.awt.image.RescaleOp;
import java.util.Collections;
import java.util.Vector;

public final class ImageModel {
	
	public final static int HIST_SIZE = 256;
	public final static int HIST_MAX = 100;
	private final String name;
	private BufferedImage image;
	private BufferedImage hist_img = new BufferedImage(HIST_SIZE, HIST_MAX, BufferedImage.TYPE_INT_RGB);
	private Vector<Integer> histogram = new Vector<Integer>(HIST_SIZE);
	private Rectangle roi = new Rectangle();
	private final int id;
	
	public ImageModel(String p_name, BufferedImage p_image, int id) {
		name = p_name;
		image = p_image;
		this.id = id;
		roi.x = 0;
		roi.y = 0;
		roi.width = image.getWidth();
		roi.height = image.getHeight();
		for (int i = 0; i < HIST_SIZE; ++i) {
			histogram.add(0);
		}
	}

	public String getName() {
		return name;
	}
	
	public BufferedImage getImage() {
		return image;
	}

	@Override
	public String toString() {
		return getName();
	}
	
	public Vector<Integer> getHistogram() {
		return histogram;
	}
	
	public BufferedImage getHistogramImage() {
		return hist_img;
	}
	
	public void calcHistogram() {
		double index = 0;
		int pixel = 0;
		Collections.fill(histogram, 0);
		for (int i = roi.x; i < roi.x + roi.width; ++i) {
			for (int j = roi.y; j < roi.y + roi.height; ++j) {
				pixel = image.getRGB(i, j);
				index = (((pixel >> 16) & 0xFF) + ((pixel >> 8) & 0xFF) + (pixel & 0xFF)) / 3.0;
				histogram.set((int)index, histogram.get((int)index) + 1);
			}
		}
		double max = Collections.max(histogram);
		for (int i = 0; i < histogram.size(); ++i) {
			histogram.set(i, (int)(histogram.get(i)*HIST_MAX/max));
		}
		int[] rgb = new int[HIST_MAX*HIST_SIZE];
		for (int y = 0; y < HIST_MAX; ++y) {
			for (int x = 0; x < HIST_SIZE; ++x) {
				if (HIST_MAX - y < histogram.get(x)) {
					rgb[HIST_SIZE*y + x] = 0;
				}
				else {
					rgb[HIST_SIZE*y + x] = 0x00FFFFFF;
				}
			}
		}
		hist_img.setRGB(0, 0, HIST_SIZE, HIST_MAX, rgb, 0, HIST_SIZE);
	}

	public Rectangle getROI() {
		return roi;
	}

	public void setROI(Rectangle roi) {
		float[] scales = { 1f, 1f, 1f, 2f };
		float[] offsets = new float[4];
		if (roi == null) {
			this.roi.height = image.getHeight();
			this.roi.width = image.getWidth();
			this.roi.x = 0;
			this.roi.y = 0;
			RescaleOp rop = new RescaleOp(scales, offsets, null);
			rop.filter(image, image);
		}
		else {
			this.roi = roi;
			Raster roi_raster = image.getData(this.roi);
			scales[3] = 0.5f;
			RescaleOp rop = new RescaleOp(scales, offsets, null);
			rop.filter(image, image);
			image.setData(roi_raster);
		}
		calcHistogram();
	}

	public int getID() {
		return id;
	}

}
