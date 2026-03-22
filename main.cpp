#include "app.h"

int main(int argc, char* argv[]) {
    // 平台相关的初始化
    int status = app::check("这个传参毫无意义（）");
    // 检测初始化是否成功
    if (status == 0) {
        // 将控制权交给主程序
        return app::run(argc, argv);
    }
    else
    {
        // 失败就结束运行
        std::cout << "[错误] 有一个严重错误导致程序意外结束运行。请前往本项目的“Github” - “问题追踪” 提交 issuse，我们将会修复。" << std::endl;
        return status;
    }
}