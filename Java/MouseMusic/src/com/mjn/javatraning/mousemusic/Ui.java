package com.mjn.javatraning.mousemusic;

import org.jfugue.player.Player;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Ui implements MouseListener, MouseMotionListener {

    private JFrame frame = null;
    private Player player = null;

    private void addMusic() {
        player = new Player();
    }

    public void createUiWindow() {

        frame = new JFrame("Mouse Music");
        frame.setSize(900, 300);
        frame.setLayout(new FlowLayout());
        //frame.setLayout(new BoxLayout(frame.getContentPane(), BoxLayout.Y_AXIS));

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


        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.X_AXIS));

        frame.addMouseListener(this);
        frame.addMouseMotionListener(this);

        frame.add(panel);

        frame.pack();
        frame.setVisible(true);
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        System.out.println("Clicked" + e.getX() + " " + e.getY());
        player.play("V0 I[Guitar] E A D G B E");
    }

    @Override
    public void mousePressed(MouseEvent e) {
        System.out.println("Pressed" + e.getX() + " " + e.getY());
        player.play("V0 I[Guitar] E A D G B E");
    }

    @Override
    public void mouseReleased(MouseEvent e) {
        System.out.println("Released" + e.getX() + " " + e.getY());
    }

    @Override
    public void mouseEntered(MouseEvent e) {
        System.out.println("Entered" + e.getX() + " " + e.getY());
    }

    @Override
    public void mouseExited(MouseEvent e) {
        System.out.println("Exited" + e.getX() + " " + e.getY());
    }

    @Override
    public void mouseDragged(MouseEvent e) {
        System.out.println("Dragged" + e.getX() + " " + e.getY());
    }

    @Override
    public void mouseMoved(MouseEvent e) {
        System.out.println("Moved" + e.getX() + " " + e.getY());
    }
}
