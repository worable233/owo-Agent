#include <iostream>
#include <thread>
#include <chrono>
#include <cctype>
#include <string>
#define CPPHTTPLIB_OPENSSL_SUPPORT // 用于启用 cpp-httplib 库中的 OpenSSL 支持功能
#include "include/httplib/httplib.h"
#include "single_include/nlohmann/json.hpp"
#include "app.h"

using json = nlohmann::json;

/*

	本部分我不会，于是使用ai生成

*/




// 子函数：创建 HTTP 客户端 
void app::createClient(httplib::SSLClient& client, const std::string& host, const std::string& api_key, int timeout) {
    // 注意：client 对象已经在外部构造好了，这里只进行配置
    client.set_connection_timeout(timeout);
    client.set_read_timeout(timeout);
    client.set_write_timeout(timeout);

    // 设置 Authorization 头
    client.set_default_headers({
        {"Authorization", "Bearer " + api_key}
    });
}

// 子函数：构建请求体
json app::buildRequestBody(const std::string& model_name, const std::string& text, int max_tokens) {
	// 这样就可以做到顶级控制了（强调）
    return {
        {"model", model_name},
        {"messages", {
            {"role", "user"},
            {"content", text}
        }},
        {"max_tokens", max_tokens}
    };
}

// 子函数：发送请求
httplib::Result app::sendRequest(httplib::SSLClient& client, const json& body, int retry) {
    httplib::Result res;
    
    for (int attempt = 1; attempt <= retry; attempt++) {
        log(0, "[请求] 尝试 " + std::to_string(attempt) + "/" + std::to_string(retry));
        
        res = client.Post("/v1/chat/completions", body.dump(), "application/json");
        
        if (res && res->status == 200) {
            log(0, "[请求] 请求成功");
            break;
        }
        
        log(3, "[请求] 尝试 " + std::to_string(attempt) + " 失败: ");
        if (res) {
            log(0, "状态码 " + std::to_string(res->status));
            // 429 或 5xx 才重试
            if (res->status == 429 || res->status >= 500) {
                log(1, "[请求] 可重试错误，等待后重试...");
            } else {
                log(1, "[请求] 不可重试错误，停止重试");
                break;
            }
        } else {
            log(3, "无响应 (网络错误)");
        }
        
        if (attempt < retry) {
            log(0, "[请求] 等待 2 秒后重试...");
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
    
    return res;
}

// 子函数：解析响应
std::string app::parseResponse(const httplib::Result& res) {
    if (!res) {
        throw std::runtime_error("无响应");
    }
    
    if (res->status != 200) {
        throw std::runtime_error("HTTP 错误: " + std::to_string(res->status));
    }
    
    try {
        json response = json::parse(res->body);
        
        // 安全访问（避免崩溃）
        if (!response.contains("choices") || !response["choices"].is_array() || response["choices"].empty()) {
            throw std::runtime_error("响应中缺少有效的 choices 数组");
        }
        
        const auto& first = response["choices"][0];
        if (!first.contains("message") || !first["message"].contains("content")) {
            throw std::runtime_error("响应中缺少 message.content 字段");
        }
        
        return first["message"]["content"].get<std::string>();
        
    } catch (const json::exception& e) {
        throw std::runtime_error("解析 JSON 失败: " + std::string(e.what()));
    }
}

// 主函数：openai
int app::openai(const std::string& text) {
    try {
		log(0,"[配置] 读取配置...");
		// 从模型文件获取模型相关的
		std::string base_url = this->model["base_url"];  // 如：https://api.openai.com
		std::string api_key = this->model["api_key"];     // 如：sk-xxx
		std::string model_name = this->model["name"];     // 如：gpt-4o-mini
		// 从配置文件获取其他值
		int timeout = this->config["settings"]["timeout"];     // 如：30
		int max_tokens = this->config["settings"]["max_tokens"]; // 如：100
		int	retry = this->config["settings"]["retry"]; // 如：100
		
		// 提取主机名（去掉 https://）（这里ai写的）
		std::string host = base_url;
		size_t pos = host.find("://");
		if (pos != std::string::npos) {
			host = host.substr(pos + 3);
		}

		// 去掉可能的路径部分
		pos = host.find('/');
		if (pos != std::string::npos) {
			host = host.substr(0, pos);
		}

        // 创建客户端
        log(0,"[客户端] 创建 HTTP 客户端...");
		httplib::SSLClient client(host);
		 // 配置客户端
		log(0,"[客户端] 配置 HTTP 客户端...");
        createClient(client, host, api_key, timeout);
        // 构建请求体
        log(0,"[请求体] 构建请求体...");
        json body = buildRequestBody(model_name, text, max_tokens);
        
        // 发送请求
        log(0,"[请求] 发送请求...");
        httplib::Result res = sendRequest(client, body, retry);
        
        // 解析响应
		log(0,"[解析] 解析响应...");
        std::string content = parseResponse(res);
        
        // 输出结果
        std::cout << "\n========== AI 回复 ==========" << std::endl;
        std::cout << content << std::endl;
        std::cout << "============================" << std::endl;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "\n[错误] " << e.what() << std::endl;
        return 1;
    }
}