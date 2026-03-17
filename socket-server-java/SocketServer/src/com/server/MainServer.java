package com.server;
import com.service.EnvService;
import java.net.ServerSocket;
import java.net.Socket;

public class MainServer {
    private ServerSocket serverSocket;
    private Socket socket;

    public MainServer() {
        try {
            serverSocket = new ServerSocket(5001);
            System.out.println("服务器已经启动请勿关闭......");
            while (true) {
                socket = serverSocket.accept();
                EnvService envService = new EnvService(socket);
                new Thread(envService).start();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        new MainServer();
    }
}