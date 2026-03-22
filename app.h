#pragma once
// 防止头文件被多次包含
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include "single_include/nlohmann/json.hpp"

// 简化命名空间
using json = nlohmann::json;

// 声明 app 类
class app {
public:
    // 每次发布更新需要修改这个版本号
    std::string version = "1.0.0";

    // 程序所在路径
    std::filesystem::path path = std::filesystem::current_path();
    // config 的路径
    std::filesystem::path config_path = path / "config" / "config.json";

    // 默认的 config
    json default_config = {
    {"version", version},
    {"debug", true},
    {"model", {
        {"name", "Deepseek"},
        {"base_url", "https://api.deepseek.com"},
        {"api_key", 114514}
    }},
    {"settings", {
        {"timeout", 30},
        {"retry", 3},
        {"model", "deepseek-chat"},
        {"stream", true}
    }}
    };

    // 读取的 config
    json config;

    // 静态函数
    static int run(int argc, char* argv[]); // 主程序
    static int check(std::string dir_path); // 初始化
    static int openai();                    // openai接口相关
    // 函数
    bool loadConfig();            // 读取配置文件
    bool saveConfig();            // 保存配置文件

};