#ifndef SOCIALWIDGET_H
#define SOCIALWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonArray>
#include <QUuid>

class SocialWidget : public QWidget
{
    Q_OBJECT

public:
    SocialWidget(const QString &username, QWidget *parent = nullptr);
    ~SocialWidget();

private slots:
    void onPostMessage();       // 发布留言
    void onDeleteMessage();     // 删除留言
    void onSearch();            // 搜索留言
    void refreshMessages();     // 刷新留言列表
    void onReplyMessage();      // 回复留言

private:
    QString currentUser;
    QListWidget *messageList;   // 留言列表
    QTextEdit *messageInput;    // 留言输入框
    QLineEdit *searchInput;     // 搜索框
    QPushButton *postBtn;       // 发布按钮
    QPushButton *deleteBtn;     // 删除按钮
    QPushButton *searchBtn;     // 搜索按钮
    QPushButton *refreshBtn;    // 刷新按钮
    QPushButton *replyBtn;      // 回复按钮

    QJsonArray messages;        // 留言数据
    QString searchKeyword;      // 搜索关键词

    void setupUI();
    void loadMessages();        // 加载留言数据
    void saveMessages();        // 保存留言数据
    QJsonObject getMessageFromItem(QListWidgetItem *item);
    QJsonObject findOriginalMessage(const QString &originalId); // 修改参数为原消息ID
    QString generateUniqueId(); //生成唯一ID
};

#endif // SOCIALWIDGET_H
