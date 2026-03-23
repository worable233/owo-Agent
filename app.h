#pragma once
// 防止头文件被多次包含
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include "single_include/nlohmann/json.hpp"
#define CPPHTTPLIB_OPENSSL_SUPPORT // 用于启用 cpp-httplib 库中的 OpenSSL 支持功能
#include "include/httplib/httplib.h"

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
    {"models", {
        "model_1", {
        {"name", "Deepseek"},
        {"base_url", "https://api.deepseek.com"},
        {"api_key", "YOUR_API_KEY_HERE"}}},
        "model_2", {
        {"name", "Kimi"},
        {"base_url", "https://api.moonshot.cn/v1/chat/completions"},
        {"api_key", "YOUR_API_KEY_HERE"}}},
    {"settings", {
        {"timeout", 30},
        {"max_tokens", 100},
        {"retry", 3},
        {"default_model", "deepseek-chat"},
        {"stream", true}
    }}
    };

    // 读取的 config
    json config;
    // 默认的model
    json model;

    // 构造函数，防止初始化前日志的debug读不到报错
    app() : config(default_config) {}

    // 静态函数
    int run(int argc, char* argv[]);                      // 主程序
    // 函数
    bool loadConfig();                                        // 读取配置文件
    bool saveConfig(int status);                        // 保存配置文件
    int log(int title, const std::string& text);      // 日志
    int check();                                                    // 初始化
    // openai 主函数
    int openai(const std::string& text);                                                  // openai接口相关
    // openai 子函数
    void createClient(httplib::SSLClient& client, const std::string& host, const std::string& api_key, int timeout);// 
    json buildRequestBody(const std::string& model_name, const std::string& text, int max_tokens); // 构建请求体
    httplib::Result sendRequest(httplib::SSLClient& client, const json& body, int retry); // 发送请求
    std::string parseResponse(const httplib::Result& res); // 解析响应

};