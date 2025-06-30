#include "socialwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>
#include <QSplitter>
#include <QUuid>
#include "campus_styles.h" // 包含样式头文件

SocialWidget::SocialWidget(const QString &username, QWidget *parent)
    : QWidget(parent), currentUser(username)
{
    setupUI();
    loadMessages();
    refreshMessages();
}

SocialWidget::~SocialWidget()
{
    saveMessages();
}

void SocialWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 搜索栏
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *searchLabel = new QLabel("搜索留言:");
    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("输入关键词搜索...");
    searchBtn = new QPushButton("搜索");
    refreshBtn = new QPushButton("刷新");

    // 应用样式
    searchInput->setStyleSheet(CampusStyles::getLineEditStyle());
    searchBtn->setStyleSheet(CampusStyles::getPrimaryButtonStyle());
    refreshBtn->setStyleSheet(CampusStyles::getPrimaryButtonStyle());

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(searchBtn);
    searchLayout->addWidget(refreshBtn);
    searchLayout->addStretch();

    // 使用分割器
    QSplitter *splitter = new QSplitter(Qt::Vertical);

    // 留言列表
    messageList = new QListWidget();
    messageList->setSelectionMode(QAbstractItemView::SingleSelection);
    messageList->setStyleSheet(CampusStyles::getListStyle()); // 应用列表样式

    // 输入区域
    QWidget *inputWidget = new QWidget();
    QVBoxLayout *inputLayout = new QVBoxLayout(inputWidget);

    QLabel *inputLabel = new QLabel("发布新留言:");
    messageInput = new QTextEdit();
    messageInput->setMaximumHeight(100);
    messageInput->setPlaceholderText("在这里输入留言内容...");
    messageInput->setStyleSheet(CampusStyles::getTextEditStyle()); // 应用文本编辑样式

    QHBoxLayout *btnLayout = new QHBoxLayout();
    postBtn = new QPushButton("发布留言");
    deleteBtn = new QPushButton("删除选中");
    replyBtn = new QPushButton("回复选中");

    // 应用按钮样式
    postBtn->setStyleSheet(CampusStyles::getPrimaryButtonStyle());
    deleteBtn->setStyleSheet(CampusStyles::getDangerButtonStyle());
    replyBtn->setStyleSheet(CampusStyles::getSecondaryButtonStyle());

    btnLayout->addWidget(postBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addWidget(replyBtn);
    btnLayout->addStretch();

    inputLayout->addWidget(inputLabel);
    inputLayout->addWidget(messageInput);
    inputLayout->addLayout(btnLayout);

    splitter->addWidget(messageList);
    splitter->addWidget(inputWidget);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(splitter);

    // 连接信号槽
    connect(postBtn, &QPushButton::clicked, this, &SocialWidget::onPostMessage);
    connect(deleteBtn, &QPushButton::clicked, this, &SocialWidget::onDeleteMessage);
    connect(replyBtn, &QPushButton::clicked, this, &SocialWidget::onReplyMessage);
    connect(searchBtn, &QPushButton::clicked, this, &SocialWidget::onSearch);
    connect(refreshBtn, &QPushButton::clicked, [this]() {
        searchInput->clear();
        searchKeyword.clear();
        refreshMessages();
    });
    connect(searchInput, &QLineEdit::returnPressed, this, &SocialWidget::onSearch);

    // 选中项变化时更新回复按钮状态
    connect(messageList, &QListWidget::itemSelectionChanged, this, [this]() {
        replyBtn->setEnabled(!messageList->selectedItems().isEmpty());
    });

    // 初始化回复按钮状态
    replyBtn->setEnabled(false);
}

void SocialWidget::onPostMessage()
{
    QString content = messageInput->toPlainText().trimmed();

    bool isReply = false;
    QString replyToId = "";
    QString replyToUser = "";

    if (content.startsWith("@")) {
        int spacePos = content.indexOf(' ');
        if (spacePos > 0) {
            isReply = true;
            replyToUser = content.mid(1, spacePos - 1);
            content = content.mid(spacePos + 1).trimmed();

            // 尝试查找被回复消息的ID
            QList<QListWidgetItem*> selectedItems = messageList->selectedItems();
            if (!selectedItems.isEmpty()) {
                QJsonObject originalMsg = getMessageFromItem(selectedItems.first());
                if (!originalMsg.isEmpty()) {
                    replyToId = originalMsg["id"].toString();
                }
            }
        }
    }

    if (content.isEmpty()) {
        QMessageBox::warning(this, "警告", "留言内容不能为空！");
        return;
    }

    QJsonObject message;
    message["id"] = generateUniqueId();
    message["author"] = currentUser;
    message["content"] = content;
    message["time"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    message["type"] = "normal";

    if (isReply) {
        message["replyToId"] = replyToId;
        message["replyToUser"] = replyToUser;
        message["replyType"] = "reply";
    } else {
        message["replyToId"] = "";
        message["replyToUser"] = "";
        message["replyType"] = "original";
    }

    messages.prepend(message);
    saveMessages();
    refreshMessages();

    messageInput->clear();
    QMessageBox::information(this, "成功", "留言发布成功！");
}

void SocialWidget::onDeleteMessage()
{
    int row = messageList->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "警告", "请选择要删除的留言！");
        return;
    }

    QListWidgetItem *item = messageList->currentItem();
    QJsonObject msg = getMessageFromItem(item);
    QString author = msg["author"].toString();
    QString msgId = msg["id"].toString();

    if (author != currentUser) {
        QMessageBox::warning(this, "警告", "只能删除自己发布的留言！");
        return;
    }

    if (QMessageBox::question(this, "确认", "确定要删除这条留言吗？") == QMessageBox::Yes) {
        // 使用ID精确查找要删除的留言
        for (int i = 0; i < messages.size(); i++) {
            if (messages[i].toObject()["id"].toString() == msgId) {
                messages.removeAt(i);
                break;
            }
        }
        saveMessages();
        refreshMessages();
    }
}

void SocialWidget::onSearch()
{
    searchKeyword = searchInput->text().trimmed();
    refreshMessages();
}

void SocialWidget::refreshMessages()
{
    messageList->clear();

    for (const auto &value : messages) {
        QJsonObject msg = value.toObject();
        QString author = msg["author"].toString();
        QString content = msg["content"].toString();
        QString time = msg["time"].toString();
        QDateTime dateTime = QDateTime::fromString(time, Qt::ISODate);
        QString replyToId = msg["replyToId"].toString();
        QString replyToUser = msg["replyToUser"].toString();
        QString replyType = msg["replyType"].toString();

        if (!searchKeyword.isEmpty()) {
            if (!content.contains(searchKeyword, Qt::CaseInsensitive) &&
                !author.contains(searchKeyword, Qt::CaseInsensitive) &&
                !replyToUser.contains(searchKeyword, Qt::CaseInsensitive)) {
                continue;
            }
        }

        QString displayText;
        if (replyType == "reply") {
            QJsonObject originalMsg = findOriginalMessage(replyToId);
            QString originalContent = originalMsg["content"].toString();

            if (!originalContent.isEmpty()) {
                displayText = QString("[%1 回复 %2] %3\n%4\n原留言: %5\n%6\n回复内容: %7\n%8")
                                  .arg(author)
                                  .arg(replyToUser)
                                  .arg(dateTime.toString("yyyy-MM-dd hh:mm"))
                                  .arg("─────────────────────────")
                                  .arg(originalContent)
                                  .arg("─────────────────────────")
                                  .arg(content)
                                  .arg("─────────────────────────");
            } else {
                displayText = QString("[%1 回复 %2] %3\n%4\n回复内容: %5\n%6")
                                  .arg(author)
                                  .arg(replyToUser)
                                  .arg(dateTime.toString("yyyy-MM-dd hh:mm"))
                                  .arg("─────────────────────────")
                                  .arg(content)
                                  .arg("─────────────────────────");
            }
        } else {
            displayText = QString("[%1] %2\n%3\n%4")
            .arg(author)
                .arg(dateTime.toString("yyyy-MM-dd hh:mm"))
                .arg(content)
                .arg("─────────────────────────");
        }

        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, msg["id"].toString()); // 存储ID

        // 设置文本颜色和背景色
        QColor bgColor = Qt::white; // 默认背景色
        QColor textColor = Qt::black; // 默认文本色

        if (author == currentUser) {
            bgColor = QColor(240, 248, 255); // 浅蓝色背景
        }

        if (replyType == "reply") {
            item->setData(Qt::UserRole + 1, "reply");
            bgColor = QColor(250, 250, 250); // 浅灰色背景
        }

        // 特殊内容的文本颜色（保持原有逻辑）
        if (content.contains("失物招领")) {
            textColor = QColor(255, 140, 0); // 橙色文本
        } else if (content.contains("活动") || content.contains("比赛")) {
            textColor = QColor(34, 139, 34); // 绿色文本
        }

        // 应用样式
        item->setBackground(bgColor);
        item->setForeground(textColor);

        messageList->addItem(item);
    }

    if (!searchKeyword.isEmpty()) {
        setWindowTitle(QString("校园社交 - 找到 %1 条相关留言").arg(messageList->count()));
    } else {
        setWindowTitle("校园社交");
    }
}

QJsonObject SocialWidget::findOriginalMessage(const QString &originalId)
{
    if (originalId.isEmpty())
        return QJsonObject();

    for (const auto &value : messages) {
        QJsonObject msg = value.toObject();
        if (msg["id"].toString() == originalId) {
            return msg;
        }
    }

    return QJsonObject();
}

void SocialWidget::onReplyMessage()
{
    QList<QListWidgetItem*> selectedItems = messageList->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一条留言！");
        return;
    }

    QListWidgetItem *item = selectedItems.first();
    QJsonObject msg = getMessageFromItem(item);
    QString author = msg["author"].toString();

    QString replyPrefix = QString("@%1 ").arg(author);
    messageInput->setText(replyPrefix);
    messageInput->setFocus();
}

QJsonObject SocialWidget::getMessageFromItem(QListWidgetItem *item)
{
    QString msgId = item->data(Qt::UserRole).toString();

    for (const auto &value : messages) {
        QJsonObject msg = value.toObject();
        if (msg["id"].toString() == msgId) {
            return msg;
        }
    }

    return QJsonObject();
}

QString SocialWidget::generateUniqueId()
{
    return QUuid::createUuid().toString(QUuid::Id128);
}

void SocialWidget::loadMessages()
{
    QFile file("messages.json");

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        messages = doc.array();

        // 为现有消息添加ID（如果没有的话）
        for (int i = 0; i < messages.size(); i++) {
            QJsonObject msg = messages[i].toObject();
            if (!msg.contains("id") || msg["id"].toString().isEmpty()) {
                msg["id"] = generateUniqueId();
                messages[i] = msg;
            }
        }
    }
}

void SocialWidget::saveMessages()
{
    QFile file("messages.json");

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(messages);
        file.write(doc.toJson());
        file.close();
    }
}
