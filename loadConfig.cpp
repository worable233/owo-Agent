#include <iostream>
#include "single_include/nlohmann/json.hpp"
#include "app.h"

// 成员函数：加载配置文件
bool app::loadConfig() {
    // 判断配置文件是否存在
    if (std::filesystem::exists(app.path / "config" / "config.json")) {
        std::cout << "[正常] 文件存在" << std::endl;
    }
    else {
        std::cout << "[警告] 文件不存在!" << std::endl;
        return false;
    }

	return true;
}