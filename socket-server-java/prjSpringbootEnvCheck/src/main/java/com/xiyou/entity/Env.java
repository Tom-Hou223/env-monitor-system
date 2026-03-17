package com.xiyou.entity;

import java.util.Date;
import java.io.Serializable;

/**
 * (Env)实体类
 *
 * @author makejava
 * @since 2026-03-15 23:27:54
 */
public class Env implements Serializable {
    private static final long serialVersionUID = -73013002207160989L;
/**
     * 主键自增
     */
    private Integer id;
/**
     * 温度值
     */
    private String tmp;
/**
     * 湿度值
     */
    private String him;
/**
     * 气味值
     */
    private String gan;
/**
     * 日期
     */
    private Date cdate;


    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getTmp() {
        return tmp;
    }

    public void setTmp(String tmp) {
        this.tmp = tmp;
    }

    public String getHim() {
        return him;
    }

    public void setHim(String him) {
        this.him = him;
    }

    public String getGan() {
        return gan;
    }

    public void setGan(String gan) {
        this.gan = gan;
    }

    public Date getCdate() {
        return cdate;
    }

    public void setCdate(Date cdate) {
        this.cdate = cdate;
    }

}

