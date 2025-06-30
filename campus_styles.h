#ifndef CAMPUS_STYLES_H
#define CAMPUS_STYLES_H

#include <QString>
#include <QStringList>

class CampusStyles {
public:
    // 校园活力主题色
    static const QString PRIMARY_COLOR;      // 活力蓝
    static const QString SECONDARY_COLOR;    // 阳光橙
    static const QString SUCCESS_COLOR;      // 青春绿
    static const QString WARNING_COLOR;      // 温暖黄
    static const QString DANGER_COLOR;       // 活力红
    static const QString BACKGROUND_COLOR;   // 清新背景
    static const QString CARD_COLOR;         // 纯净白

    // 留言区域多彩底色方案
    static const QStringList MESSAGE_COLORS;

    // 获取留言底色（根据用户ID/用户名哈希值）
    static QString getMessageBackgroundColor(const QString& username);

    // 样式表方法
    static QString getLoginWindowStyle();
    static QString getMainWindowStyle();
    static QString getTabStyle();
    static QString getPrimaryButtonStyle();
    static QString getSecondaryButtonStyle();
    static QString getDangerButtonStyle();
    static QString getLineEditStyle();
    static QString getTextEditStyle();
    static QString getTableStyle();
    static QString getListStyle();
    static QString getMessageAreaStyle();
    static QString getCardStyle();
};

#endif // CAMPUS_STYLES_H
