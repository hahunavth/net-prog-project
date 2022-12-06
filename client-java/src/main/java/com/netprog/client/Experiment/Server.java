package com.netprog.client.Experiment;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
    public static void main (String[] args) throws IOException {

        String sentence_from_client;
        String sentence_to_client;

        try (ServerSocket serverSocket = new ServerSocket(5500)) {

            while (true) {
                Socket socket = serverSocket.accept();
                InputStream inputStream = socket.getInputStream();
                InputStreamReader inputStreamReader = new InputStreamReader(inputStream);

                BufferedReader inFromClient =
                        new BufferedReader(new
                                InputStreamReader(socket.getInputStream()));

                DataOutputStream outToClient =
                        new DataOutputStream(socket.getOutputStream());


                while (true) {
                    //Đọc thông tin từ socket
                    sentence_from_client = inFromClient.readLine();

                    sentence_to_client = sentence_from_client +" (Server accepted!)" + '\n';
                    //ghi dữ liệu ra socket
                    outToClient.writeBytes(sentence_to_client);

                    System.out.println(sentence_from_client);
                }
//                return;
                //
//                int character;
//                StringBuffer data = new StringBuffer();
//                while ((character = inputStreamReader.read()) != -1) {
//                    data.append((char) character);
//                }
//                System.out.println(data);
            }

        } catch (IOException e) {
e.printStackTrace();
            System.out.println(e.getMessage());
        }
    }
}
