package com.example.filesearch;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.regex.Pattern;
import java.util.zip.ZipOutputStream;

public class Main {

    String path;
    String regex;
    String zipFileName;
    Pattern pattern;
    List<File> zipFies = new ArrayList<>();

    public static void main(String[] args) {
        Main app = new Main();

        switch(Math.min(args.length, 3)) {
            case 0:
                System.out.println("Usage: App path [regex] [zip] ");
                return;
            case 3: app.setZipFileName(args[2]);
            case 2: app.setRegex(args[1]);
            case 1: app.setPath(args[0]);
        }

        try {
            System.out.println("Java6:");
            app.walkDirectoryJava6(app.getPath());
            //System.out.println("Java7:");
            //app.walkDirectoryJava7(app.getPath());
            //System.out.println("Java8:");
            //app.walkDirectoryJava8(app.getPath());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void walkDirectoryJava6(String path) {
        File dir = new File(path);
        File[] files = dir.listFiles();

        for(File file : files) {
            if (file.isDirectory()) {
                walkDirectoryJava6(file.getAbsolutePath());
            } else {
                processFile(file);
            }
        }
    }

    public void walkDirectoryJava7(String path) throws IOException {
        Files.walkFileTree(Paths.get(path), new SimpleFileVisitor<Path>() {
            public FileVisitResult visitFie(Path file, BasicFileAttributes attrs)
                    throws IOException {
                processFile(file.toFile());
                return FileVisitResult.CONTINUE;
            }
        });
    }

    public void walkDirectoryJava8(String path) throws IOException {
        Files.walk(Paths.get(path))
                .forEach(f -> processFile(f.toFile()));
    }

    public void processFile(File file) {
        System.out.println("processFile: " + file);
        try {
            if (searchFile(file))
            {
                addFileToZip(file);
            }
        } catch (IOException| UncheckedIOException e) {
            System.out.println("Error processing file: " + file);
        }
    }

    public boolean searchFile(File file) throws IOException {
        System.out.println("searchFile: " + file);
        return searchFileJava8(file);
    }

    public boolean searchFileJava6(File file) throws FileNotFoundException {
        boolean found = false;
        Scanner scanner = new Scanner(file, "UTF-8");
        while (scanner.hasNextLine()){
            found = searchText(scanner.nextLine());
            if (found) { break; }
        }
        scanner.close();
        return true;
    }

    public boolean searchFileJava7(File file) throws IOException {
        List<String> lines = Files.readAllLines(file.toPath(), StandardCharsets.UTF_8);
        for (String line : lines){
            if (searchText(line)) { return true; }
        }
        return false;
    }

    public boolean searchFileJava8(File file) throws IOException {
        return Files.lines(file.toPath(), StandardCharsets.UTF_8)
                .anyMatch(t -> searchText(t));
    }


    public boolean searchText(String text) {
        return (this.getRegex() == null) ? true :
                this.pattern.matcher(text).matches();
                //this.pattern.matcher(text).find();

        //if (this.getRegex() == null){
        //    return true;
        //}
        //return text.toLowerCase().contains(this.getRegex().toLowerCase());
    }

    public void addFileToZip(File file) {
        if (getZipFileName() != null) {
            zipFies.add(file);
        }
    }

    public void zipFilesJava6() throws IOException {
        ZipOutputStream out = null;
//        try {
//            out = new ZipOutputStream(new FileOutputStream(getZipFileName()));
//            File baseDir = new File(getPath());
//
//        }
    }

    public String getPath() {
        return path;
    }

    public void setPath(String path) {
        this.path = path;
    }

    public String getRegex() {
        return regex;
    }

    public void setRegex(String regex) {
        this.regex = regex;
        this.pattern = Pattern.compile(regex);
    }

    public String getZipFileName() {
        return zipFileName;
    }

    public void setZipFileName(String zipFileName) {
        this.zipFileName = zipFileName;
    }
}
