package com.netprog.client.Experiment;

import java.io.*;
import java.net.*;

public class Client {
    public static void main(String[] args) {
        try (Socket socket = new Socket("127.0.0.1", 5500)) {

            OutputStream outputStream = socket.getOutputStream();
            PrintWriter writer = new PrintWriter(outputStream, true);
            writer.println("Hello");

//
//            InputStream inputStream = socket.getInputStream();
//            InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
//
//            int character;
//            StringBuffer data = new StringBuffer();
//
//            while ((character = inputStreamReader.read()) != -1) {
//                data.append((char) character);
//            }
//
//            System.out.println(data);

        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}
