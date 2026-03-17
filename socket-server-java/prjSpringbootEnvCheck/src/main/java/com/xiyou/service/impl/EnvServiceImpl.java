package com.xiyou.service.impl;

import com.xiyou.entity.Env;
import com.xiyou.dao.EnvDao;
import com.xiyou.service.EnvService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageImpl;
import org.springframework.data.domain.PageRequest;
import java.util.List;
import java.util.Map;

@Service("envService")
public class EnvServiceImpl implements EnvService {
    @Resource
    private EnvDao envDao;

    @Override
    public Page<Env> queryByPage(Env env, PageRequest pageRequest) {
        long total = this.envDao.count(env);
        return new PageImpl<>(this.envDao.queryAllByLimit(env, pageRequest), pageRequest, total);
    }

    @Override
    public List<Env> getEnvListTop5() {
        return envDao.getEnvListTop5();
    }

    @Override
    public List<Map<String, Object>> findAvgEnvData() {
        return envDao.findAvgEnvData();
    }
}