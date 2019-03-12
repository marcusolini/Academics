package com.example.java;

import java.io.*;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.nio.file.Path;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.time.format.DateTimeFormatter;
import java.util.*;

public class Main {

    public static void main(String[] args) throws UnknownHostException {

        String sourcePath = ".";
        String extension = ".txt";
        File[] files;
        List<Path> fileList = new ArrayList<>();
        Map<Date, Float> barMap = new LinkedHashMap<>();
        List<Weather> weatherList = new ArrayList<>();

        File root = new File(sourcePath);

        files = root.listFiles((d,s) -> { return s.endsWith(extension); });
        //files = root.listFiles();


        if (null == files) {
            System.out.println("No files");
        } else {
            for (File file : files) {
                if (file.isFile()) {
                    fileList.add(file.toPath());
                }
            }

            for (Path path : fileList) {
                String filePath;
                filePath = path.toString();

                try (FileReader fileReader = new FileReader(filePath);
                     BufferedReader bufferedReader = new BufferedReader(fileReader);
                ) {
                    System.out.println(filePath);

                    String sLine = null;
                    boolean bSkipHeader = true;
                    String[] splits;
                    SimpleDateFormat sdf = new SimpleDateFormat("yyyy_MM_dd HH:mm:ss");

                    while (true) {
                        sLine = bufferedReader.readLine();
                        if (true == bSkipHeader) {
                            bSkipHeader = false;
                            continue;
                        }
                        if (null == sLine) {
                            break;
                        }

                        splits = sLine.split("\\s+");
                        //for (String string : splits) {
                        //    System.out.println(string);
                        //}

                        String dateTime = splits[0] + " " + splits[1];
                        Date date = sdf.parse(dateTime);
                        Float pressure = Float.parseFloat(splits[3]);

                        Weather weather = new Weather();
                        weather.date = date;
                        weather.pressure = pressure;
                        weatherList.add(weather);

                        barMap.put(date, pressure);
                    }
                } catch (FileNotFoundException | ParseException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            //System.out.println(barMap);
            //barMap.forEach((k,v)->{
            //    System.out.println(k);
            //    System.out.println(v);
            //});

            Iterator<Map.Entry<Date, Float>> iterator = barMap.entrySet().iterator();
            while (iterator.hasNext()) {
                Map.Entry<Date, Float> entry = iterator.next();
                if (iterator.hasNext()) {
                    System.out.println("1Key :" + entry.getKey() + " Value :" + entry.getValue());
                    Map.Entry<Date, Float> entry2 = iterator.next();
                    System.out.println("2Key :" + entry.getKey() + " Value :" + entry.getValue());
                }
            }

            ListIterator<Weather> weatherIterator = weatherList.listIterator();

            while (weatherIterator.hasNext()) {
                Weather entry = weatherIterator.next();
                if (weatherIterator.hasNext()) {
                    System.out.println("1Date :" + entry.date + " Value :" + entry.pressure);
                    entry = weatherIterator.next();
                    System.out.println("2Date :" + entry.date + " Value :" + entry.pressure);
                }
            }
        }
    }
}
