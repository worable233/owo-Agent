#include <iostream>
// 文件操作
#include <filesystem>
#include <fstream>
#include "single_include/nlohmann/json.hpp"
#include "app.h"

// 成员函数：初始化
int app::check() {
    // 配置文件所在路径
    std::path dir_path = app.path / "config";

    // 如果不存在
    if (!std::filesystem::exists(dir_path)) {
        // 创建路径
        std::filesystem::create_directory(dir_path);
    }

    if (std::filesystem::exists(dir_path / "config.json")) {
        std::cout << "[正常] 文件存在" << std::endl;
    }
    else {
        std::cout << "[警告] 文件不存在!" << std::endl;
        // 写入文件
        std::ofstream file(dir_path / "config.json");
        if (file.is_open()) {
            std::cout << "[正常] config.json 创建成功" << std::endl;
            file << app.config.dump(4); // 格式化输出，缩进4空格
            std::cout << "[正常] config.json 写入成功" << std::endl;
            std::cout << "[正常] config.json 路径" << dir_path / "config.json" << std::endl;
        }
        else {
            std::cerr << "[错误] 无法创建文件" << std::endl;
            return 1;
        }

    }
    return 0;
}