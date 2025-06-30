#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QJsonArray>
#include <QTimer>

class TaskWidget : public QWidget
{
    Q_OBJECT

public:
    TaskWidget(const QString &username, QWidget *parent = nullptr);
    ~TaskWidget();

private slots:
    void onAddTask();           // 添加任务
    void onDeleteTask();        // 删除任务
    void onToggleComplete();    // 切换完成状态
    void checkDeadlines();      // 检查截止时间
    void refreshList();         // 刷新列表

private:
    QString currentUser;
    QListWidget *taskList;      // 任务列表
    QPushButton *addBtn;
    QPushButton *deleteBtn;
    QPushButton *completeBtn;
    QPushButton *showAllBtn;    // 显示所有
    QPushButton *showActiveBtn; // 显示未完成

    QJsonArray tasks;           // 任务数据
    QTimer *deadlineTimer;      // 定时器
    bool showCompleted;         // 是否显示已完成

    void setupUI();
    void loadTasks();           // 加载任务数据
    void saveTasks();           // 保存任务数据
    QString getPriorityColor(const QString &priority); // 获取优先级颜色
};

#endif // TASKWIDGET_H
