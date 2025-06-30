#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

// 前向声明
class QPushButton;
class QLineEdit;
class QLabel;

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginClicked();      // 登录按钮点击
    void onRegisterClicked();   // 注册按钮点击

private:
    // UI控件
    QLineEdit *usernameEdit;    // 用户名输入框
    QLineEdit *passwordEdit;    // 密码输入框
    QPushButton *loginBtn;      // 登录按钮
    QPushButton *registerBtn;   // 注册按钮
    QLabel *statusLabel;        // 状态提示标签

    void setupUI();             // 设置界面
    bool verifyLogin(const QString &username, const QString &password);  // 验证登录
    bool registerUser(const QString &username, const QString &password); // 注册用户
};

#endif // LOGINWINDOW_H
