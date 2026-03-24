#include <functional>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <vector>
#include "single_include/nlohmann/json.hpp"
#include "include/curl/curl.h"
#include "app.h"

// 简化命名空间
using json = nlohmann::json;

// 入口点
int app::run(int argc, char* argv[]) {
	std::string username = this->config["username"];

	// 清空终端
	clearScreen();

	// 判断是不是第一次使用
	if (username == "")
	{
		// 引导使用的临时变量
		bool a = false;
		std::string b = "";

		// 进入欢迎引导
		std::cout << std::string(5, '\n') << std::endl;
		std::cout << "Welcome To owoAgent" << std::endl;
		std::cout << "欢迎使用" << std::endl;
		std::cout << std::string(2, '\n') << std::endl;
		std::cout << "已自动为您创建了配置文件," << std::endl;
		std::cout << "请跟随提示进行初步的配置;" << std::endl;
		std::cout << "待配置完成后可以再次修改." << std::endl;
		std::cout << std::string(2, '\n') << std::endl;

		// 设置用户名
		while (a != true){
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << "Phase 1 / Step 1" << std::endl;
			std::cout <<"请设置您的用户名:" << std::endl;
			std::cin >> username;
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << "Phase 1 / Step 2" << std::endl;
			std::cout <<"您的用户名为 " << username << "\n您确定使用此用户名吗?( 确定请输入 y )"<< std::endl;
			std::cin >> b;
			if (b == "y" ) {
				a = true;
			}
		};

		this->config["username"] = username;
		a = false; // 重置状态
		std::cout << std::string(1, '\n') << std::endl;
		while (a != true){
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << "Phase 2 / Step 1" << std::endl;
			std::cout <<"接下来配置您的第一个模型" << std::endl;
			std::cout << std::string(1, '\n') << std::endl;

			// 创建空对象
			this->config["models"]["model_3"] = json::object(); 

			std::cout <<"请输入您的配置名称:" << std::endl;
			std::cin >> username;
			this->config["models"]["model_3"]["name"] = username;

			std::cout <<"请输入您的模型名称:" << std::endl;
			std::cin >> username;
			this->config["settings"]["use_model"] = username;

			std::cout <<"请输入您的API Base URL:" << std::endl;
			std::cin >> username;
			this->config["models"]["model_3"]["base_url"] = username;

			std::cout <<"请输入您的API Key(API 密钥):" << std::endl;
			std::cin >> username;
			this->config["models"]["model_3"]["api_key"] = username;

			std::cout <<"已保存,请勿泄露您的API密钥!" << std::endl; // 提示
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << "Phase 2 / Step 2" << std::endl;
			std::cout <<"正在检测此配置是否可用( 不会浪费您的Token )( 待开发 )" << std::endl;
			// TODO: 检测
			set_model("3");
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << "Phase 2 / Step 3" << std::endl;
			std::cout <<"您的" << this->model["name"] << "配置为:\n" << "Base URL - " <<model["base_url"] << "\nAPI Key - " << this->model["api_key"] << "\n您确定使用此配置吗?( 确定请输入 y )"<< std::endl;
			std::cin >> b;
			if (b == "y" ) {
				a = true;
			}
		};
		// 结束阶段
		std::cout << std::string(2, '\n') << std::endl;
		std::cout << "Phase 3 / Step 1" << std::endl;
		std::cout << "正在保存您的配置" << std::endl;
		saveConfig(1);
		std::cout << std::string(2, '\n') << std::endl;
		std::cout << "Phase 3 / Step 2" << std::endl;
		std::cout << "您的配置已保存" << std::endl;
		std::cout << std::string(2, '\n') << std::endl;
		std::cout << "Phase 3 / Step 3" << std::endl;
		std::cout << "您已经完成了所有的初步的配置" << std::endl;
		std::cout << "接下来可以正式使用 owo_Agent 了" << std::endl;
		std::cout << std::string(1, '\n') << std::endl;
		std::cout << "按下任意键继续...";
		std::cin >> b;
		// 清空终端
		clearScreen();
	}

	// 完成初步配置
	// 死循环菜单
	set_model("3");
	std::string c = "";
	int success = 0;
	while (true) {
		std::cout << std::string(10, '=') << std::endl;
		std::cout << "/ 菜单 " << std::endl;
		std::cout << "1.对话\n2.管理模型\n3.对话设置\n4.debug\n5.退出程序" << std::endl;
		std::cout << std::string(1, '\n') << std::endl;
		std::cout << "您的选择 - ";
		std::cin >> c;
		std::cout << std::string(1, '\n') << std::endl;
		if (c == "1") {
			// TODO: 没做对话
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << std::string(10, '=') << std::endl;
			std::cout << "/ 菜单 / 对话" << std::endl;
			std::cout << "请输入您的问题  - ";
			std::cin >> c;
			std::cout << "回复内容：" << std::endl;
			openai(c);
			std::cout << "请按下任意键继续...";
			std::cin >> c;
			std::cout << std::string(1, '\n') << std::endl;
		} else if (c == "2") {
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << std::string(10, '=') << std::endl;
			std::cout << "/ 菜单 / 管理模型" << std::endl;
			list_models(); // 列出所有模型
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << "您的选择 - ";
			std::cin >> c;
			int success = set_model(c); 
			if (success == 2) {
				std::cout << "输入模型id有误" << std::endl;
			} else if (success == 0){
				std::cout << "设置成功！" << std::endl;
			} else {
				std::cout << "其他错误，请开启调试模式查看日志。" << std::endl;
			}
		} else if (c == "3") {
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << std::string(10, '=') << std::endl;
			std::cout << "/ 菜单 / 对话设置" << std::endl;
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << "没做这个功能" << std::endl;
			std::cout << "请按下任意键继续...";
			std::cin >> c;
		}else if (c == "4") {
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << std::string(10, '=') << std::endl;
			std::cout << "/ 菜单 / debug" << std::endl;
			std::cout << std::string(1, '\n') << std::endl;
			if (this->config["debug"] == true) {
				std::cout << "已关闭调试模式！"<< std::endl;
			} else {
				std::cout << "已开启调试模式！"<< std::endl;
			}
			this->config["debug"] = !this->config["debug"];
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << "您的选择 - ";
			std::cin >> c;
		} else if (c == "5") {
			// 退出循环
			break;
		} else {
			std::cout << std::string(1, '\n') << std::endl;
			std::cout << "您的选择有误!";
		}
		std::cout << std::string(2, '\n') << std::endl;
	}
	return 0;
}

void app::list_models() {
	// 遍历
    int i = 1;
    for (auto a = this->config["models"].begin(); a != config["models"].end(); ++a) {
		
		// 获取模型名称
        std::string model_name = a.value().value("name", "未知模型");
		
		// 直接输出
        std::cout << i << " " << model_name << std::endl;
        i++;
    }
}

// 设置模型
int app::set_model(const std::string& id) {
	std::string models_id = "model_"+id;
	// 检查 config 中是否有 "models" 对象
    if (!this->config.contains("models") || !this->config["models"].is_object()) {
        log(3, "config 中没有有效的 models 对象");
        return 1;
    }

    // 检查传入的 id 是否存在
    if (!this->config["models"].contains(models_id)) {
        log(3, "未找到模型 id: " + id);
        return 2; 
    }

    // 将选中的模型配置赋值给成员变量 model
	this->model["name"] = this->config["models"][models_id]["name"];
    this->model["api_key"] = this->config["models"][models_id]["api_key"];
	this->model["base_url"] = this->config["models"][models_id]["base_url"];

    // 可选：输出成功信息
    log(0, "[模型] 已切换到: " + this->config["models"][models_id].value("name", "unknown") + "也就是" +models_id );
    return 0;
}

void app::clearScreen() {
    // \033[2J  : 清除整个屏幕 (Clear Screen)
    // \033[1;1H: 将光标移动到第 1 行 第 1 列 (Home)
    // \033[3J  : 清除滚动缓冲区 (可选，防止按向上箭头看到历史内容)
    std::cout << "\033[2J\033[1;1H\033[3J" << std::flush;
}