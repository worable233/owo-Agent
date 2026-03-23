#include "app.h"

int main(int argc, char* argv[]) {
    // 先使用默认配置
    app owo;
    // 平台相关的初始化
    int status = owo.check();
    // 检测初始化是否成功
    if (status == 0) {
        // 将控制权交给主程序
        return owo.run(argc,argv);
    } else {
        // 失败就结束运行
        std::cerr << "[致命错误] 有一个严重错误导致程序意外结束运行。请前往本项目的“Github” - “问题追踪” 提交 issuse，我们将会修复。" << std::endl;
        std::cerr << "status错误代码：" << status << std::endl;
        return status;
    }
}