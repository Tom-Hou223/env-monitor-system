package com.xiyou.service;

import com.xiyou.entity.Env;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import java.util.List;
import java.util.Map;

public interface EnvService {
    Page<Env> queryByPage(Env env, PageRequest pageRequest);
    List<Env> getEnvListTop5();
    List<Map<String,Object>> findAvgEnvData();
}