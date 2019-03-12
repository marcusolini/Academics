package com.mjn.javatraining.util;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class InputHelper {

    public static String getInput(String prompt) {
        InputStreamReader inStream = new InputStreamReader(System.in);
        BufferedReader stdin = new BufferedReader(inStream);

        System.out.print(prompt);
        System.out.flush();

        try {
            return stdin.readLine();
        } catch (IOException e) {
            return "Error: " + e.getMessage();
        }

    }

    public static int getIntegerInput(String prompt) throws NumberFormatException {
        String input = getInput(prompt);
        return Integer.parseInt(input);
    }

    public static double getDoubleInput(String prompt) throws NumberFormatException {
        String input = getInput(prompt);
        return Double.parseDouble(input);
    }
}
