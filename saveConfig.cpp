#include <iostream>
#include "single_include/nlohmann/json.hpp"
#include "app.h"

// 成员函数：保存配置文件
bool app::saveConfig(this,) {
    // 打开文件
    std::ofstream file(this.config_path);
    if (!file.is_open()) {
        std::cerr << "[错误] 无法写入配置文件: " << config_path << std::endl;
        return false;
    }
    file << config_data.dump(4);  // 格式化保存
    return true;
}