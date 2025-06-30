#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QString>

class CourseWidget;
class TaskWidget;
class SocialWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &username, QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString currentUser;        // 当前登录用户
    QTabWidget *tabWidget;      // 标签页控件

    CourseWidget *courseWidget; // 课程表模块
    TaskWidget *taskWidget;     // 任务管理模块
    SocialWidget *socialWidget; // 社交模块

    void setupUI();             // 设置界面
    void setupMenuBar();        // 设置菜单栏
};

#endif // MAINWINDOW_H
