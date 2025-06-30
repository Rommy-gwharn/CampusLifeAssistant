#include "loginwindow.h"
#include "mainwindow.h"
#include "campus_styles.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QCryptographicHash>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setWindowTitle("校园生活助手 - 登录");
    resize(300, 200);

    // 应用登录窗口样式
    this->setStyleSheet(CampusStyles::getLoginWindowStyle());
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::setupUI()
{
    // 创建控件
    QLabel *titleLabel = new QLabel("校园生活助手");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");  // 设置对象名以便样式表匹配

    QLabel *usernameLabel = new QLabel("用户名:");
    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("请输入用户名");

    QLabel *passwordLabel = new QLabel("密码:");
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("请输入密码");

    loginBtn = new QPushButton("登录");
    registerBtn = new QPushButton("注册");
    registerBtn->setObjectName("registerBtn");  // 设置对象名以便样式表匹配
    statusLabel = new QLabel("");
    statusLabel->setObjectName("statusLabel");  // 设置对象名以便样式表匹配

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);

    QHBoxLayout *userLayout = new QHBoxLayout();
    userLayout->addWidget(usernameLabel);
    userLayout->addWidget(usernameEdit);
    mainLayout->addLayout(userLayout);

    QHBoxLayout *passLayout = new QHBoxLayout();
    passLayout->addWidget(passwordLabel);
    passLayout->addWidget(passwordEdit);
    mainLayout->addLayout(passLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(loginBtn);
    btnLayout->addWidget(registerBtn);
    mainLayout->addLayout(btnLayout);

    mainLayout->addWidget(statusLabel);

    // 连接信号槽
    connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(registerBtn, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
}

void LoginWindow::onLoginClicked()
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("用户名和密码不能为空！");
        return;
    }

    if (verifyLogin(username, password)) {
        // 登录成功，打开主窗口
        MainWindow *mainWindow = new MainWindow(username);
        mainWindow->show();
        this->close();
    } else {
        statusLabel->setText("用户名或密码错误！");
    }
}

void LoginWindow::onRegisterClicked()
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("用户名和密码不能为空！");
        return;
    }

    if (registerUser(username, password)) {
        QMessageBox::information(this, "注册成功", "注册成功！请登录。");
        statusLabel->setText("");
    } else {
        statusLabel->setText("用户名已存在！");
    }
}

bool LoginWindow::verifyLogin(const QString &username, const QString &password)
{
    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();
    QJsonArray users = root["users"].toArray();

    // 密码加密
    QString hashedPassword = QString(QCryptographicHash::hash(password.toUtf8(),
                                                              QCryptographicHash::Sha256).toHex());

    for (const auto &value : users) {
        QJsonObject user = value.toObject();
        if (user["username"].toString() == username &&
            user["password"].toString() == hashedPassword) {
            return true;
        }
    }

    return false;
}

bool LoginWindow::registerUser(const QString &username, const QString &password)
{
    QFile file("users.json");
    QJsonObject root;
    QJsonArray users;

    // 读取现有用户
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        root = doc.object();
        users = root["users"].toArray();

        // 检查用户名是否已存在
        for (const auto &value : users) {
            QJsonObject user = value.toObject();
            if (user["username"].toString() == username) {
                return false;
            }
        }
    }

    // 添加新用户
    QJsonObject newUser;
    newUser["username"] = username;
    newUser["password"] = QString(QCryptographicHash::hash(password.toUtf8(),
                                                           QCryptographicHash::Sha256).toHex());
    users.append(newUser);
    root["users"] = users;

    // 保存到文件
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(root);
        file.write(doc.toJson());
        file.close();
        return true;
    }

    return false;
}
