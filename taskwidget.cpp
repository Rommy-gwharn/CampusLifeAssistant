#include "taskwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTimeEdit>
#include <QDateTime>
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include <algorithm>
#include <QList>
#include <QLineEdit>
#include "campus_styles.h" // 包含样式头文件

TaskWidget::TaskWidget(const QString &username, QWidget *parent)
    : QWidget(parent), currentUser(username), showCompleted(true)
{
    setupUI();
    loadTasks();
    refreshList();

    // 设置定时器检查截止时间
    deadlineTimer = new QTimer(this);
    connect(deadlineTimer, &QTimer::timeout, this, &TaskWidget::checkDeadlines);
    deadlineTimer->start(60000); // 每分钟检查一次
}

TaskWidget::~TaskWidget()
{
    saveTasks();
}

void TaskWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建任务列表
    taskList = new QListWidget(this);
    taskList->setSelectionMode(QAbstractItemView::SingleSelection);
    taskList->setStyleSheet(CampusStyles::getListStyle()); // 应用列表样式

    // 创建按钮
    QHBoxLayout *btnLayout = new QHBoxLayout();
    addBtn = new QPushButton("添加任务");
    addBtn->setStyleSheet(CampusStyles::getPrimaryButtonStyle()); // 应用主要按钮样式

    deleteBtn = new QPushButton("删除任务");
    deleteBtn->setStyleSheet(CampusStyles::getDangerButtonStyle()); // 应用危险按钮样式

    completeBtn = new QPushButton("标记完成/未完成");
    completeBtn->setStyleSheet(CampusStyles::getSecondaryButtonStyle()); // 应用次要按钮样式

    showAllBtn = new QPushButton("显示所有");
    showAllBtn->setStyleSheet(CampusStyles::getPrimaryButtonStyle()); // 应用主要按钮样式

    showActiveBtn = new QPushButton("仅显示未完成");
    showActiveBtn->setStyleSheet(CampusStyles::getPrimaryButtonStyle()); // 应用主要按钮样式

    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addWidget(completeBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(showActiveBtn);
    btnLayout->addWidget(showAllBtn);

    mainLayout->addWidget(taskList);
    mainLayout->addLayout(btnLayout);

    // 连接信号槽
    connect(addBtn, &QPushButton::clicked, this, &TaskWidget::onAddTask);
    connect(deleteBtn, &QPushButton::clicked, this, &TaskWidget::onDeleteTask);
    connect(completeBtn, &QPushButton::clicked, this, &TaskWidget::onToggleComplete);
    connect(showAllBtn, &QPushButton::clicked, [this]() {
        showCompleted = true;
        refreshList();
    });
    connect(showActiveBtn, &QPushButton::clicked, [this]() {
        showCompleted = false;
        refreshList();
    });
}

void TaskWidget::onAddTask()
{
    // 创建任务输入对话框
    QDialog dialog(this);
    dialog.setWindowTitle("添加新任务");

    QFormLayout *layout = new QFormLayout(&dialog);

    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setStyleSheet(CampusStyles::getLineEditStyle()); // 应用输入框样式

    QDateTimeEdit *deadlineEdit = new QDateTimeEdit(QDateTime::currentDateTime().addDays(7));
    deadlineEdit->setCalendarPopup(true);
    deadlineEdit->setStyleSheet(CampusStyles::getLineEditStyle()); // 应用输入框样式

    QComboBox *priorityCombo = new QComboBox();
    priorityCombo->addItems({"低", "中", "高"});
    priorityCombo->setCurrentIndex(1);

    layout->addRow("任务名称:", nameEdit);
    layout->addRow("截止时间:", deadlineEdit);
    layout->addRow("优先级:", priorityCombo);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (nameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "警告", "任务名称不能为空！");
            return;
        }

        QJsonObject task;
        task["name"] = nameEdit->text();
        task["deadline"] = deadlineEdit->dateTime().toString(Qt::ISODate);
        task["priority"] = priorityCombo->currentText();
        task["completed"] = false;
        task["createTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);

        tasks.append(task);
        saveTasks();
        refreshList();
    }
}

void TaskWidget::onDeleteTask()
{
    int row = taskList->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "警告", "请选择要删除的任务！");
        return;
    }

    if (QMessageBox::question(this, "确认", "确定要删除该任务吗？") == QMessageBox::Yes) {
        // 找到对应的任务并删除
        QString taskText = taskList->currentItem()->text();
        for (int i = tasks.size() - 1; i >= 0; i--) {
            QJsonObject task = tasks[i].toObject();
            if (taskText.contains(task["name"].toString())) {
                tasks.removeAt(i);
                break;
            }
        }
        saveTasks();
        refreshList();
    }
}

void TaskWidget::onToggleComplete()
{
    int row = taskList->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "警告", "请选择要操作的任务！");
        return;
    }

    QString taskText = taskList->currentItem()->text();
    for (int i = 0; i < tasks.size(); i++) {
        QJsonObject task = tasks[i].toObject();
        if (taskText.contains(task["name"].toString())) {
            task["completed"] = !task["completed"].toBool();
            tasks[i] = task;
            break;
        }
    }
    saveTasks();
    refreshList();
}

void TaskWidget::checkDeadlines()
{
    QDateTime now = QDateTime::currentDateTime();

    for (const auto &value : tasks) {
        QJsonObject task = value.toObject();
        if (!task["completed"].toBool()) {
            QDateTime deadline = QDateTime::fromString(task["deadline"].toString(), Qt::ISODate);
            qint64 hoursUntilDeadline = now.secsTo(deadline) / 3600;

            if (hoursUntilDeadline < 24 && hoursUntilDeadline > 0) {
                QMessageBox::warning(this, "任务提醒",
                                     QString("任务 \"%1\" 将在 %2 小时后到期！")
                                         .arg(task["name"].toString())
                                         .arg(hoursUntilDeadline));
            }
        }
    }
}

void TaskWidget::refreshList()
{
    taskList->clear();

    // 将QJsonArray转换为QList进行排序
    QList<QJsonObject> sortedTaskList;
    for (const auto &value : tasks) {
        sortedTaskList.append(value.toObject());
    }

    // 按截止时间排序
    std::sort(sortedTaskList.begin(), sortedTaskList.end(), [](const QJsonObject &a, const QJsonObject &b) {
        QDateTime dateA = QDateTime::fromString(a["deadline"].toString(), Qt::ISODate);
        QDateTime dateB = QDateTime::fromString(b["deadline"].toString(), Qt::ISODate);
        return dateA < dateB;
    });

    for (const auto &task : sortedTaskList) {
        bool completed = task["completed"].toBool();

        if (!showCompleted && completed) {
            continue;
        }

        QString priority = task["priority"].toString();
        QString name = task["name"].toString();
        QDateTime deadline = QDateTime::fromString(task["deadline"].toString(), Qt::ISODate);

        QString status = completed ? "[已完成] " : "[未完成] ";
        QString itemText = QString("%1%2 - 截止: %3 - 优先级: %4")
                               .arg(status)
                               .arg(name)
                               .arg(deadline.toString("yyyy-MM-dd hh:mm"))
                               .arg(priority);

        QListWidgetItem *item = new QListWidgetItem(itemText);

        // 设置颜色
        if (completed) {
            item->setForeground(Qt::gray);
        } else {
            // 检查是否临近截止
            qint64 hoursUntilDeadline = QDateTime::currentDateTime().secsTo(deadline) / 3600;
            if (hoursUntilDeadline < 24) {
                item->setBackground(QColor(255, 200, 200)); // 红色背景
            } else if (priority == "高") {
                item->setForeground(Qt::red);
            } else if (priority == "中") {
                item->setForeground(QColor(255, 140, 0)); // 橙色
            }
        }

        taskList->addItem(item);
    }
}

void TaskWidget::loadTasks()
{
    QString filename = QString("tasks_%1.json").arg(currentUser);
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        tasks = doc.array();
    }
}

void TaskWidget::saveTasks()
{
    QString filename = QString("tasks_%1.json").arg(currentUser);
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(tasks);
        file.write(doc.toJson());
        file.close();
    }
}

QString TaskWidget::getPriorityColor(const QString &priority)
{
    if (priority == "高") return "red";
    if (priority == "中") return "orange";
    return "green";
}
