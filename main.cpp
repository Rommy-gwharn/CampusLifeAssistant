#include <QApplication>
#include "loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("校园生活助手");
    app.setOrganizationName("闪光菜苗组");

    // 创建并显示登录窗口
    LoginWindow loginWindow;
    loginWindow.show();

    return app.exec();
}
