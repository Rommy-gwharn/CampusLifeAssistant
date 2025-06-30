#include "mainwindow.h"
#include "coursewidget.h"
#include "taskwidget.h"
#include "socialwidget.h"
#include "campus_styles.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), currentUser(username)
{
    setupUI();
    setupMenuBar();
    setWindowTitle("校园生活助手 - " + currentUser);
    resize(800, 600);

    // 应用主窗口样式
    setStyleSheet(CampusStyles::getMainWindowStyle());
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // 创建标签页控件
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    // 创建各功能模块
    courseWidget = new CourseWidget(currentUser, this);
    taskWidget = new TaskWidget(currentUser, this);
    socialWidget = new SocialWidget(currentUser, this);

    // 添加到标签页
    tabWidget->addTab(courseWidget, "课程表");
    tabWidget->addTab(taskWidget, "任务管理");
    tabWidget->addTab(socialWidget, "校园社交");

    // 应用标签页样式
    tabWidget->setStyleSheet(CampusStyles::getTabStyle());
}

void MainWindow::setupMenuBar()
{
    // 创建菜单栏
    QMenuBar *menuBar = this->menuBar();

    // 文件菜单
    QMenu *fileMenu = menuBar->addMenu("文件");

    QAction *logoutAction = new QAction("注销", this);
    connect(logoutAction, &QAction::triggered, [this]() {
        if (QMessageBox::question(this, "确认", "确定要注销吗？") == QMessageBox::Yes) {
            qApp->quit();
        }
    });
    fileMenu->addAction(logoutAction);

    QAction *exitAction = new QAction("退出", this);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    fileMenu->addAction(exitAction);

    // 帮助菜单
    QMenu *helpMenu = menuBar->addMenu("帮助");

    QAction *aboutAction = new QAction("关于", this);
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "关于",
                           "校园生活助手 v1.0\n\n"
                           "开发团队：闪光菜苗组\n"
                           "功能：课程管理、任务提醒、校园社交");
    });
    helpMenu->addAction(aboutAction);
}
