package com.example.SwingTest;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;
import java.awt.image.ColorConvertOp;
import java.io.File;
import java.io.IOException;

public class SwingTest {

    private static final String IMAGE_FILENAME = "testimage.jpg";
    private boolean warnAboutReload = true;

    public static void main(String[] args) {
        SwingUtilities.invokeLater( () -> {
            SwingTest test = new SwingTest();
            test.createSwingWindow();
        });
        //SwingUtilities.invokeLater(new Runnable() {
        //    @Override
        //    public void run() {
        //        SwingTest test = new SwingTest();
        //        //test.createSimpleSwingWindow();
        //        test.createSwingWindow();
        //    }
        //});
    }

    public JFrame createSimpleSwingWindow() {
        JFrame frame = new JFrame("Swing Test");
        frame.setLayout(new BoxLayout(frame.getContentPane(), BoxLayout.Y_AXIS));
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JLabel label = new JLabel("This is our test Swing application");
        JButton button = new JButton("Click Me!");

        frame.add(label);
        frame.add(button);

        frame.pack();
        frame.setVisible(true);
        return frame;
    }

    public JFrame createSwingWindow() {
        JFrame frame = new JFrame("Test Swing Test");
        frame.setLayout(new BoxLayout(frame.getContentPane(), BoxLayout.Y_AXIS));
        //frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                //super.windowClosing(e);
                int shouldClose = JOptionPane.showConfirmDialog(frame, "Close?", "Confirm close", JOptionPane.YES_NO_CANCEL_OPTION);
                if (shouldClose == JOptionPane.YES_OPTION) {
                    System.exit(0);
                }
            }
        });

        JLabel label = new JLabel("This is our test Swing application");
        JButton button = new JButton("Click Me!");

        ImageIcon icon = new ImageIcon(IMAGE_FILENAME);
        JLabel iconLabel = new JLabel(icon);
        iconLabel.setPreferredSize(new Dimension(icon.getIconWidth(), icon.getIconHeight())); // Stop field boxes from resizing

        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.X_AXIS));

        JLabel pctSizeLabel = new JLabel("Percent Size: ");
        JTextField pctSizeField = new JTextField();

        JLabel rotationLabel = new JLabel("Degree Rotation: ");
        JTextField rotationField = new JTextField();

        button.addActionListener(
                (event) -> adjustImage(pctSizeField, rotationField, iconLabel)
                // (event) -> System.out.println("You clicked a button")
        );
        //button.addActionListener(new ActionListener() {
        //    @Override
        //    public void actionPerformed(ActionEvent e) {
        //        System.out.println("You clicked a button");
        //    }
        //});


        panel.add(pctSizeLabel);
        panel.add(pctSizeField);
        panel.add(rotationLabel);
        panel.add(rotationField);

        frame.add(label);
        frame.add(iconLabel);
        frame.add(panel);

        //frame.add(button);
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));
        buttonPanel.add(button);

        JButton reloadButton = new JButton("Reload Image");
        reloadButton.addActionListener(event -> reloadImage(iconLabel));
        buttonPanel.add(reloadButton);

        JButton saveButton = new JButton("Save Image");
        saveButton.addActionListener(event -> saveImageToFile(iconLabel));
        buttonPanel.add(saveButton);

        frame.add(buttonPanel);


        label.setAlignmentX(Component.CENTER_ALIGNMENT);
        iconLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        button.setAlignmentX(Component.CENTER_ALIGNMENT);

        frame.pack();
        frame.setVisible(true);
        return frame;
    }

    private void adjustImage(JTextField pctField, JTextField degreeField, JLabel imageLabel) {
        Icon icon = imageLabel.getIcon();
        ImageIcon imageIcon = (ImageIcon)icon;
        Image image = imageIcon.getImage();

        BufferedImage bi = convertToBufferedImage(image);

        bi = resizeImage(bi, getIntFromTextField(pctField, 100));
        bi = rotateImageBetter(bi, getIntFromTextField(degreeField, 0));
        //bi = rotateImage(bi, getIntFromTextField(degreeField, 0));

        imageLabel.setIcon(new ImageIcon(bi));
    }

    private BufferedImage convertToBufferedImage(Image image) {
        if (image instanceof BufferedImage) {
            return (BufferedImage)image;
        }

        BufferedImage bi = new BufferedImage(image.getWidth(null), image.getHeight(null), BufferedImage.TYPE_INT_ARGB);
        Graphics g = bi.getGraphics();
        g.drawImage(image, 0, 0, null);
        g.dispose();

        return bi;
    }

    private BufferedImage resizeImage(BufferedImage bi, int percent) {
        double scale = percent / 100.0;
        AffineTransform resize = AffineTransform.getScaleInstance(scale, scale);
        AffineTransformOp op = new AffineTransformOp(resize, AffineTransformOp.TYPE_BICUBIC);
        return ((AffineTransformOp) op).filter(bi, null);
    }

    private BufferedImage rotateImageBetter(BufferedImage bi, int degrees) {
        double radians = Math.toRadians(degrees);
        AffineTransform rotate = AffineTransform.getRotateInstance(radians);

        // Recenter image...
        int height = bi.getHeight();
        int width = bi.getWidth();
        Rectangle newSize = rotate.createTransformedShape(new Rectangle(width, height)).getBounds();

        rotate = new AffineTransform();
        rotate.translate(newSize.width*0.5, newSize.height*.05);
        rotate.rotate(radians);
        rotate.translate(-width*0.5, -height*0.5);

        AffineTransformOp op = new AffineTransformOp(rotate, AffineTransformOp.TYPE_BILINEAR);
        return op.filter(bi, null);
    }

    private BufferedImage rotateImage(BufferedImage bi, int degrees) {
        double radians = Math.toRadians(degrees);
        AffineTransform rotate = AffineTransform.getRotateInstance(radians);
        AffineTransformOp op = new AffineTransformOp(rotate, AffineTransformOp.TYPE_BILINEAR);
        return op.filter(bi, null);
    }

    private int getIntFromTextField(JTextField pctField, int defaultValue) {
        try {
            int num = Integer.parseInt(pctField.getText());
            return num;
        } catch (NumberFormatException e) {
            return defaultValue;
        }
    }

    private void saveImage(Image image, File file) throws IOException {
        BufferedImage bi = convertToBufferedImage(image);

        // Use color convert op to correct colors.
        BufferedImage rgbImage = new BufferedImage(bi.getWidth(), bi.getHeight(), BufferedImage.TYPE_INT_RGB);
        ColorConvertOp toRgb = new ColorConvertOp(null);
        toRgb.filter(bi, rgbImage);

        ImageIO.write(bi, "png", file);
    }

    private void reloadImage(JLabel imageLabel) {
        if (warnAboutReload) {
            JLabel message = new JLabel("Warning, this image will reload");
            JCheckBox noReloadWarn = new JCheckBox("Don't tell me this again");
            JComponent[] controls = new JComponent[] {message, noReloadWarn};
            JOptionPane.showMessageDialog(imageLabel.getParent(), controls, "Image Will Reload", JOptionPane.PLAIN_MESSAGE);
            warnAboutReload = !noReloadWarn.isSelected();
        }

        imageLabel.setIcon(new ImageIcon(IMAGE_FILENAME));
    }

    private void saveImageToFile(JLabel imageLabel) {
        JFileChooser chooser = new JFileChooser();
        FileNameExtensionFilter filter = new FileNameExtensionFilter("PNG Image", "png");
        chooser.setFileFilter(filter);
        int returnVal = chooser.showSaveDialog(imageLabel.getParent());

        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = chooser.getSelectedFile();

            try {
                ImageIcon icon = (ImageIcon)imageLabel.getIcon();
                saveImage(icon.getImage(), file);

                JOptionPane.showMessageDialog(
                        imageLabel.getParent(),
                        "File save to: " + file,
                        "File Saved Successfully",
                        JOptionPane.PLAIN_MESSAGE
                );

            } catch (IOException e) {
                JOptionPane.showMessageDialog(
                        imageLabel.getParent(),
                        "Error saving file: " + e.getMessage(),
                        "Error",
                        JOptionPane.WARNING_MESSAGE
                );
            }
        }
    }
}
