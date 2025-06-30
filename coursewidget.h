#ifndef COURSEWIDGET_H
#define COURSEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QJsonArray>

struct Course {
    QString name;       // 课程名
    QString teacher;    // 教师
    QString location;   // 地点
    int dayOfWeek;     // 星期几 (1-7)
    int startTime;     // 开始节次
    int duration;      // 持续节次
};

class CourseWidget : public QWidget
{
    Q_OBJECT

public:
    CourseWidget(const QString &username, QWidget *parent = nullptr);
    ~CourseWidget();

private slots:
    void onAddCourse();     // 添加课程
    void onEditCourse();    // 编辑课程
    void onDeleteCourse();  // 删除课程
    void refreshTable();    // 刷新表格

private:
    QString currentUser;
    QTableWidget *courseTable;  // 课程表格
    QPushButton *addBtn;
    QPushButton *editBtn;
    QPushButton *deleteBtn;

    QJsonArray courses;         // 课程数据

    void setupUI();
    void loadCourses();         // 加载课程数据
    void saveCourses();         // 保存课程数据
    void initTable();           // 初始化表格
};

#endif // COURSEWIDGET_H
