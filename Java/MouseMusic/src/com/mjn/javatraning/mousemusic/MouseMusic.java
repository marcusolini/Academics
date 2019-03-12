package com.mjn.javatraning.mousemusic;

import org.jfugue.player.Player;

import javax.swing.*;

public class MouseMusic {

    public static void main(String[] args) {
        SwingUtilities.invokeLater( () -> {
            Ui ui= new Ui();
            ui.createUiWindow();
        });

        //SwingUtilities.invokeLater(new Runnable() {
        //    @Override
        //    public void run() {
        //        SwingTest test = new SwingTest();
        //        //test.createSimpleSwingWindow();
        //        test.createSwingWindow();
        //    }
        //});

        Player player = new Player();
        player.play("V0 I[Guitar] E A D G B E");
    }
}
