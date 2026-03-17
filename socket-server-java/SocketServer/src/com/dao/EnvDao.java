package com.dao;
import com.db.DBHelper;
import com.entity.Env;

public class EnvDao {
    public int save(Env env) {
        String sql = "insert into env(tmp,him,gan,cdate) values(?,?,?,?)";
        int code = DBHelper.executeUpdate(sql, new Object[]{
                env.getTmp(), env.getHim(), env.getGan(), env.getCdate()
        });
        if (code > 0) {
            System.out.println("传感器数据存储成功！");
        } else {
            System.out.println("数据存储失败！");
        }
        return code;
    }
}