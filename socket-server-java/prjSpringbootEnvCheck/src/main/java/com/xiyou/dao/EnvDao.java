package com.xiyou.dao;

import com.xiyou.entity.Env;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.springframework.data.domain.Pageable;
import java.util.List;
import java.util.Map;

@Mapper
public interface EnvDao {
    Env queryById(Integer id);
    // 注意：这里有 @Param("env") 和 @Param("pageable")
    List<Env> queryAllByLimit(@Param("env") Env env, @Param("pageable") Pageable pageable);
    // 注意：count 这里也要加 @Param("env")
    long count(@Param("env") Env env);
    int insert(Env env);
    int update(Env env);
    int deleteById(Integer id);

    List<Env> getEnvListTop5();
    List<Map<String,Object>> findAvgEnvData();
}