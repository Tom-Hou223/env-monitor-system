package com.db;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.util.Properties;

public class DBHelper {
    static Properties prop = DBConnection.getConfig();

    public static int executeUpdate(String sql, Object[] objs) {
        try {
            Class.forName(prop.getProperty("classDriver"));
            Connection conn = DriverManager.getConnection(
                    prop.getProperty("url"),
                    prop.getProperty("username"),
                    prop.getProperty("password")
            );
            PreparedStatement ps = conn.prepareStatement(sql);
            for (int i = 0; i < objs.length; i++) {
                ps.setObject(i + 1, objs[i]);
            }
            int code = ps.executeUpdate();
            ps.close();
            conn.close();
            return code;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }
}