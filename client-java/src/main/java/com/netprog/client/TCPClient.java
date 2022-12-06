package com.netprog.client;


import java.io.*;
import java.net.Socket;
import java.util.Arrays;

public class TCPClient implements Runnable{

    private Socket socket;
    private PrintWriter writer;
    private InputStreamReader inputStreamReader;
    private BufferedReader inFromServer;

    public TCPClient() {
        this("127.0.0.1", 5500);
    }

    public TCPClient(String host, int port) {
        try {
            socket = new Socket(host, port);
            OutputStream outputStream = socket.getOutputStream();
            writer = new PrintWriter(outputStream, true);
            InputStream inputStream = socket.getInputStream();
            inputStreamReader = new InputStreamReader(inputStream);
            inFromServer = new BufferedReader(inputStreamReader);
            System.out.println("Connect successfully!");
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }

    @Override
    public void run() {
        while (socket != null) {
            char[] frame = receive();
            // TODO: handle command with c code (event/action)
        }
    }

    private char[] receive () {
        char[] buffer = new char[1024];

        try {
            int recvBytes = inFromServer.read(buffer, 0, 13);
            buffer[recvBytes] = '\0';
            System.out.println("Recv: " + recvBytes + " bytes");
            System.out.println("Data: " + Arrays.toString(buffer));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return buffer;
    }

    public void send(String s) {
        writer.println(s);
    }
}