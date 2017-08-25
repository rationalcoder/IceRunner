package team10;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;

import javax.swing.ImageIcon;

public class ColorChanger {
	
	public ColorChanger() {
		
	}
	
	public  ImageIcon colorImage(ImageIcon icon) {
		Image image = icon.getImage();
		BufferedImage buffered = toBufferedImage(image);
        int width = buffered.getWidth();
        int height = buffered.getHeight();

        for (int xx = 0; xx < width; xx++) {
            for (int yy = 0; yy < height; yy++) {
                Color originalColor = new Color(buffered.getRGB(xx, yy));
                System.out.println(xx + "|" + yy + " color: " + originalColor.toString() + "alpha: "
                        + originalColor.getAlpha());
              
                    buffered.setRGB(xx, yy, Color.BLUE.getRGB());      
            }
            icon = new ImageIcon(buffered); 
        }
        return icon;
    }
	
	public static BufferedImage toBufferedImage(Image img)
	{
	    if (img instanceof BufferedImage)
	    {
	        return (BufferedImage) img;
	    }

	    // Create a buffered image with transparency
	    BufferedImage bimage = new BufferedImage(img.getWidth(null), img.getHeight(null), BufferedImage.TYPE_INT_ARGB);

	    // Draw the image on to the buffered image
	    Graphics2D bGr = bimage.createGraphics();
	    bGr.drawImage(img, 0, 0, null);
	    bGr.dispose();

	    // Return the buffered image
	    return bimage;
	}

}
