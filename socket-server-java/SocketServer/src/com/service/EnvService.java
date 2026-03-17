package com.service;
import com.dao.EnvDao;
import com.entity.Env;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Date;

public class EnvService implements Runnable {
    private Socket socket;
    private InputStream in;
    private OutputStream os;
    private EnvDao envDao;

    public EnvService(Socket socket) {
        this.socket = socket;
        try {
            in = socket.getInputStream();
            os = socket.getOutputStream();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        try {
            byte[] b = new byte[1024];
            in.read(b);
            String msg = new String(b).trim();
            System.out.println("客户端传递的信息: " + msg);

            String[] ss = msg.split(",");
            Env env = new Env(ss[0], ss[1], ss[2], new Date());
            envDao = new EnvDao();
            envDao.save(env);

            String rst = "serverok";
            os.write(rst.getBytes(StandardCharsets.UTF_8));
            os.flush();

            socket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}