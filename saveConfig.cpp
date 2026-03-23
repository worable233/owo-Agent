#include <iostream>
#include <fstream>
#include "single_include/nlohmann/json.hpp"
#include "app.h"

// 成员函数：保存配置文件
bool app::saveConfig(int status) {
    // 打开文件
    std::ofstream file(this->config_path);
    // 如果能正常打开
        if (file.is_open()) {
            // 判断是初始化还是保存
            if (status == 0) {
                file << this->default_config.dump(4); // 格式化输出，缩进4空格
                log(0,"config.json 创建成功");
            } else {
                file << this->config.dump(4);  // 格式化输出，缩进4空格
                log(0,"config.json 写入成功" );
            }
            log(0,"config.json 路径" + std::string(this->config_path ));
            return true;
        }
        else {
            // 判断是初始化还是保存
            if (status == 0) {
                log(3,"无法创建文件");
            } else {
                log(3,"无法写入文件");
            }
            return false;
    }
}