#include "coursewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <QBrush>
#include "campus_styles.h" // 添加头文件

CourseWidget::CourseWidget(const QString &username, QWidget *parent)
    : QWidget(parent), currentUser(username)
{
    setupUI();
    loadCourses();
    refreshTable();
}

CourseWidget::~CourseWidget()
{
    saveCourses();
}

void CourseWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建课程表格
    courseTable = new QTableWidget(this);
    initTable();

    // 设置表格样式
    courseTable->setStyleSheet(CampusStyles::getTableStyle()); // 添加样式表

    // 创建按钮
    QHBoxLayout *btnLayout = new QHBoxLayout();
    addBtn = new QPushButton("添加课程");
    editBtn = new QPushButton("编辑课程");
    deleteBtn = new QPushButton("删除课程");

    // 设置按钮样式
    addBtn->setStyleSheet(CampusStyles::getPrimaryButtonStyle());
    editBtn->setStyleSheet(CampusStyles::getSecondaryButtonStyle());
    deleteBtn->setStyleSheet(CampusStyles::getDangerButtonStyle());

    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addStretch();

    mainLayout->addWidget(courseTable);
    mainLayout->addLayout(btnLayout);

    // 连接信号槽
    connect(addBtn, &QPushButton::clicked, this, &CourseWidget::onAddCourse);
    connect(editBtn, &QPushButton::clicked, this, &CourseWidget::onEditCourse);
    connect(deleteBtn, &QPushButton::clicked, this, &CourseWidget::onDeleteCourse);
}

void CourseWidget::initTable()
{
    // 设置表格属性
    courseTable->setRowCount(12);  // 12节课
    courseTable->setColumnCount(8); // 周一到周日 + 节次

    // 设置表头
    QStringList headers;
    headers << "节次" << "周一" << "周二" << "周三" << "周四" << "周五" << "周六" << "周日";
    courseTable->setHorizontalHeaderLabels(headers);

    // 设置节次列
    for (int i = 0; i < 12; i++) {
        QTableWidgetItem *item = new QTableWidgetItem(QString("第%1节").arg(i + 1));
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        courseTable->setItem(i, 0, item);
    }

    // 设置列宽调整模式
    QHeaderView* horizontalHeader = courseTable->horizontalHeader();
    for (int i = 0; i < courseTable->columnCount(); ++i) {
        horizontalHeader->setSectionResizeMode(i, QHeaderView::Stretch);
    }

    courseTable->verticalHeader()->hide();
}

void CourseWidget::onAddCourse()
{
    // 简单的输入对话框
    bool ok;
    QString courseName = QInputDialog::getText(this, "添加课程", "课程名称:",
                                               QLineEdit::Normal, "", &ok);
    if (!ok || courseName.isEmpty()) return;

    QString teacher = QInputDialog::getText(this, "添加课程", "教师姓名:",
                                            QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString location = QInputDialog::getText(this, "添加课程", "上课地点:",
                                             QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QStringList days = {"周一", "周二", "周三", "周四", "周五", "周六", "周日"};
    QString day = QInputDialog::getItem(this, "添加课程", "星期:", days, 0, false, &ok);
    if (!ok) return;

    int dayIndex = days.indexOf(day) + 1;

    int startTime = QInputDialog::getInt(this, "添加课程", "开始节次 (1-12):",
                                         1, 1, 12, 1, &ok);
    if (!ok) return;

    int duration = QInputDialog::getInt(this, "添加课程", "持续节数:",
                                        2, 1, 4, 1, &ok);
    if (!ok) return;

    // 添加到数据
    QJsonObject course;
    course["name"] = courseName;
    course["teacher"] = teacher;
    course["location"] = location;
    course["day"] = dayIndex;
    course["startTime"] = startTime;
    course["duration"] = duration;

    courses.append(course);
    saveCourses();
    refreshTable();
}

void CourseWidget::onEditCourse()
{
    QMessageBox::information(this, "提示", "请选择要编辑的课程单元格，双击进行编辑。");
}

void CourseWidget::onDeleteCourse()
{
    int row = courseTable->currentRow();
    int col = courseTable->currentColumn();

    if (row < 0 || col < 1) {
        QMessageBox::warning(this, "警告", "请选择要删除的课程！");
        return;
    }

    QTableWidgetItem *item = courseTable->item(row, col);
    if (!item || item->text().isEmpty()) {
        return;
    }

    if (QMessageBox::question(this, "确认", "确定要删除该课程吗？") == QMessageBox::Yes) {
        // 查找并删除对应课程
        for (int i = courses.size() - 1; i >= 0; i--) {
            QJsonObject course = courses[i].toObject();
            if (course["day"].toInt() == col &&
                course["startTime"].toInt() == row + 1) {
                courses.removeAt(i);
            }
        }
        saveCourses();
        refreshTable();
    }
}

void CourseWidget::refreshTable()
{
    // 清空表格（除了节次列）
    for (int i = 0; i < 12; i++) {
        for (int j = 1; j < 8; j++) {
            QTableWidgetItem *item = new QTableWidgetItem("");
            courseTable->setItem(i, j, item);
        }
    }

    // 填充课程
    for (const auto &value : courses) {
        QJsonObject course = value.toObject();
        int day = course["day"].toInt();
        int startTime = course["startTime"].toInt() - 1;
        int duration = course["duration"].toInt();

        QString text = QString("%1\n%2\n%3")
                           .arg(course["name"].toString())
                           .arg(course["teacher"].toString())
                           .arg(course["location"].toString());

        for (int i = 0; i < duration && startTime + i < 12; i++) {
            QTableWidgetItem *item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignCenter);
            item->setBackground(QBrush(QColor(200, 220, 255)));
            courseTable->setItem(startTime + i, day, item);
        }
    }

    // 调整列宽以适应内容
    courseTable->resizeColumnsToContents();
    // 调整行高以适应内容
    courseTable->resizeRowsToContents();
}


void CourseWidget::loadCourses()
{
    QString filename = QString("courses_%1.json").arg(currentUser);
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        courses = doc.array();
    }
}

void CourseWidget::saveCourses()
{
    QString filename = QString("courses_%1.json").arg(currentUser);
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(courses);
        file.write(doc.toJson());
        file.close();
    }
}
