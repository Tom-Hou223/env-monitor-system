package com.entity;
import java.util.Date;

public class Env {
    private Integer id;
    private String tmp;
    private String him;
    private String gan;
    private Date cdate;

    public Env() {}

    public Env(String tmp, String him, String gan, Date cdate) {
        this.tmp = tmp;
        this.him = him;
        this.gan = gan;
        this.cdate = cdate;
    }

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