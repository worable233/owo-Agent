#include <iostream>
#include <fstream>
#include <filesystem> 
#include "single_include/nlohmann/json.hpp"
#include "app.h"

// 成员函数：加载配置文件
bool app::loadConfig() {
    // 判断配置文件是否存在
    if (std::filesystem::exists(this->config_path)) {
        log(0,"文件存在");
    }
    else {
        log(1,"文件不存在!");
        return false;
    }

    std::ifstream file(this->config_path);

    // 再次检查文件是否打开成功 (防止权限问题等)
    if (file.is_open()) {
        // 尝试加载文件
        try {
            // 加载文件
            this->config = json::parse(file);
            log(0, "配置文件加载成功: " + config_path.string());
            
            // 判断版本
            if (config["version"] != version) {
                log(1, "配置文件版本 (" + config["version"].get<std::string>() + ") 与程序版本 (" + version + ") 不一致");
        }
        return true;
        } catch(const std::exception& this_error)  {
            // e.what() 可以获取具体的错误描述
            log(3, "读取配置文件失败: " + std::string(this_error.what()) );

            // 出错回退到默认配置
            this->config = this->default_config;
            return false;
        }
        return true;
    } else {
        // 如果不可以打开
        log(3, "无法打开配置文件 ，权限不足或路径错误");
        this->config = this->default_config;
        return false;
    }
}