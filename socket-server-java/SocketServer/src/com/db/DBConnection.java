package com.db;
import java.io.InputStream;
import java.util.Properties;

public class DBConnection {
    public static Properties getConfig() {
        Properties prop = new Properties();
        try {
            InputStream in = DBConnection.class.getClassLoader().getResourceAsStream("dbconn.properties");
            prop.load(in);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return prop;
    }
}