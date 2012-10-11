package ogx.view;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;

import javax.swing.JLabel;
import javax.swing.event.MouseInputAdapter;

public class ImageLabel extends JLabel {

	Rectangle currentRect = null;
    Rectangle rectToDraw = null;
    Rectangle previousRectDrawn = new Rectangle();
	
	static final long serialVersionUID = 1L;
	Image image = null;
	Dimension dimension = null;
	
	public ImageLabel() {
		super();
		MyListener myListener = new MyListener();
        addMouseListener(myListener);
        addMouseMotionListener(myListener);
	}
	
	public Rectangle getSelection() {
		return rectToDraw;
	}
	
	private class MyListener extends MouseInputAdapter {
        public void mousePressed(MouseEvent e) {
            int x = e.getX();
            int y = e.getY();
            currentRect = new Rectangle(x, y, 0, 0);
            updateDrawableRect(getWidth(), getHeight());
            repaint();
        }
 
        public void mouseDragged(MouseEvent e) {
            updateSize(e);
        }
 
        public void mouseReleased(MouseEvent e) {
            updateSize(e);
        }
 
        /*
         * Update the size of the current rectangle
         * and call repaint.  Because currentRect
         * always has the same origin, translate it
         * if the width or height is negative.
         *
         * For efficiency (though
         * that isn't an issue for this program),
         * specify the painting region using arguments
         * to the repaint() call.
         *
         */
        void updateSize(MouseEvent e) {
            int x = e.getX();
            int y = e.getY();
            currentRect.setSize(x - currentRect.x,
                                y - currentRect.y);
            updateDrawableRect(getWidth(), getHeight());
            Rectangle totalRepaint = rectToDraw.union(previousRectDrawn);
            repaint(totalRepaint.x, totalRepaint.y,
                    totalRepaint.width, totalRepaint.height);
        }
    }
	
	public void setImage(Image p_image) {
		image = p_image;
	}
	public void setImageDimension(Dimension p_dim) {
		dimension = p_dim;
	}
	@Override
	public void paintComponent (Graphics g) 
    { 
        super.paintComponent (g);
        if (image != null) {
        	g.drawImage (image, 0, 0, (int)dimension.getWidth(), (int)dimension.getHeight(), null);
        }
        //If currentRect exists, paint a box on top.
        if (currentRect != null) {
            //Draw a rectangle on top of the image.
            g.setXORMode(Color.white); //Color of line varies
                                       //depending on image colors
            g.drawRect(rectToDraw.x, rectToDraw.y,
                       rectToDraw.width - 1, rectToDraw.height - 1);
        }
    } 
	
	private void updateDrawableRect(int compWidth, int compHeight) {
        int x = currentRect.x;
        int y = currentRect.y;
        int width = currentRect.width;
        int height = currentRect.height;
 
        //Make the width and height positive, if necessary.
        if (width < 0) {
            width = 0 - width;
            x = x - width + 1;
            if (x < 0) {
                width += x;
                x = 0;
            }
        }
        if (height < 0) {
            height = 0 - height;
            y = y - height + 1;
            if (y < 0) {
                height += y;
                y = 0;
            }
        }
 
        //The rectangle shouldn't extend past the drawing area.
        if ((x + width) > compWidth) {
            width = compWidth - x;
        }
        if ((y + height) > compHeight) {
            height = compHeight - y;
        }
       
        //Update rectToDraw after saving old value.
        if (rectToDraw != null) {
            previousRectDrawn.setBounds(
                        rectToDraw.x, rectToDraw.y,
                        rectToDraw.width, rectToDraw.height);
            rectToDraw.setBounds(x, y, width, height);
        } else {
            rectToDraw = new Rectangle(x, y, width, height);
        }
    }
}
