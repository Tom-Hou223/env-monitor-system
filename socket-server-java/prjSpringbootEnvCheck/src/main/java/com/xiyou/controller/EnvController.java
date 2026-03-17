package com.xiyou.controller;

import com.xiyou.entity.Env;
import com.xiyou.service.EnvService;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import jakarta.annotation.Resource;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("env")
public class EnvController {
    @Resource
    private EnvService envService;

    @RequestMapping("/findPageAll")
    public Map<String,Object> queryByPage(Env env, Integer page,Integer pagesize) {
        page=page==null||page<1?1:page;
        pagesize=pagesize==null||pagesize<1?5:pagesize;

        PageRequest pageRequest=PageRequest.of(page-1,pagesize);
        Page<Env> pageEnv=envService.queryByPage(env,pageRequest);
        long totalCount=pageEnv.getTotalElements();
        int totalPage=pageEnv.getTotalPages();
        List<Env> data=pageEnv.getContent();

        Map<String,Object> map=new HashMap<>();
        map.put("page",page);
        map.put("pagesize",pagesize);
        map.put("totalCount",totalCount);
        map.put("totalPage",totalPage);
        map.put("data",data);
        return map;
    }

    @RequestMapping("/getEnvListTop5")
    public List<Map<String,Object>> getEnvListTop5() {
        List<Env> envList = envService.getEnvListTop5();
        List<Map<String,Object>> list=new ArrayList<>();

        Map<String,Object> tmp=new HashMap<>();
        Map<String,Object> him=new HashMap<>();
        Map<String,Object> gan=new HashMap<>();

        tmp.put("name","温度");
        tmp.put("type","bar");
        him.put("name","湿度");
        him.put("type","bar");
        gan.put("name","气味");
        gan.put("type","bar");

        List<String> tmpData=new ArrayList<>();
        List<String> himData=new ArrayList<>();
        List<String> ganData=new ArrayList<>();

        for(Env e : envList){
            tmpData.add(e.getTmp());
            himData.add(e.getHim());
            ganData.add(Integer.parseInt(e.getGan())/10+"");
        }

        tmp.put("data",tmpData);
        him.put("data",himData);
        gan.put("data",ganData);
        list.add(tmp);
        list.add(him);
        list.add(gan);
        return list;
    }

    @RequestMapping("/findAvgEnvData")
    public List<Map<String,Object>> findAvgEnvData(){
        List<Map<String,Object>> dbData=envService.findAvgEnvData();
        List<Map<String,Object>> res=new ArrayList<>();

        Map<String,Object> tmp=new HashMap<>();
        Map<String,Object> him=new HashMap<>();
        Map<String,Object> gan=new HashMap<>();

        for(Map<String,Object> map : dbData){
            tmp.put("name","温度");
            him.put("name","湿度");
            gan.put("name","气味");
            tmp.put("value",map.get("avgtmp"));
            him.put("value",map.get("avghim"));
            gan.put("value",map.get("avggan"));
        }
        res.add(tmp);
        res.add(him);
        res.add(gan);
        return res;
    }
}