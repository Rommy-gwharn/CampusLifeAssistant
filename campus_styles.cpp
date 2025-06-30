#include "campus_styles.h"
#include <QCryptographicHash>

// 校园活力主题色定义
const QString CampusStyles::PRIMARY_COLOR = "#4A90E2";      // 活力蓝
const QString CampusStyles::SECONDARY_COLOR = "#FF6B35";    // 阳光橙
const QString CampusStyles::SUCCESS_COLOR = "#7ED321";     // 青春绿
const QString CampusStyles::WARNING_COLOR = "#F5A623";     // 温暖黄
const QString CampusStyles::DANGER_COLOR = "#D0021B";      // 活力红
const QString CampusStyles::BACKGROUND_COLOR = "#F8FAFE";  // 清新背景
const QString CampusStyles::CARD_COLOR = "#FFFFFF";        // 纯净白

// 留言区域多彩底色方案（12种柔和的校园色彩）
const QStringList CampusStyles::MESSAGE_COLORS = {
    "#E3F2FD",  // 浅蓝色 - 天空
    "#F3E5F5",  // 浅紫色 - 薰衣草
    "#E8F5E8",  // 浅绿色 - 嫩芽
    "#FFF3E0",  // 浅橙色 - 暖阳
    "#FCE4EC",  // 浅粉色 - 樱花
    "#E0F2F1",  // 浅青色 - 薄荷
    "#FFF8E1",  // 浅黄色 - 柠檬
    "#F1F8E9",  // 浅草绿 - 新叶
    "#E8EAF6",  // 浅靛色 - 晚霞
    "#FFEBEE",  // 浅红色 - 微醺
    "#E4F7F7",  // 浅蓝绿 - 湖水
    "#FDF2F8"   // 浅玫瑰 - 花瓣
};

QString CampusStyles::getMessageBackgroundColor(const QString& username) {
    // 使用用户名的哈希值来确保同一用户总是得到相同的颜色
    QByteArray hash = QCryptographicHash::hash(username.toUtf8(), QCryptographicHash::Md5);
    int colorIndex = qAbs(hash.at(0)) % MESSAGE_COLORS.size();
    return MESSAGE_COLORS.at(colorIndex);
}

QString CampusStyles::getLoginWindowStyle() {
    return QString(R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #667eea, stop:0.5 #764ba2, stop:1 #f093fb);
            color: #2C3E50;
            font-family: 'Microsoft YaHei UI', '微软雅黑', Arial, sans-serif;
        }

        QWidget#loginCard {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 20px;
            border: 2px solid rgba(255, 255, 255, 0.3);
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
        }

        QLabel#titleLabel {
            color: #2C3E50;
            font-size: 32px;
            font-weight: bold;
            margin: 25px 0;
            text-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }

        QLabel {
            color: #34495E;
            font-size: 16px;
            margin: 5px 0;
        }

        QLabel[text="用户名:"] {
            font-weight: bold;
        }

        QLabel[text="密码:"] {
            font-weight: bold;
        }

        QLineEdit {
            background: #FFFFFF;
            border: 3px solid #E8E8E8;
            border-radius: 12px;
            padding: 14px 18px;
            font-size: 16px;
            color: #2C3E50;
            margin: 8px 0;
        }

        QLineEdit:focus {
            border-color: %1;
            background: #FFFFFF;
            box-shadow: 0 0 0 3px rgba(74, 144, 226, 0.1);
        }

        QLineEdit:hover {
            border-color: #D5D5D5;
        }

        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 %1, stop:1 #357ABD);
            color: white;
            border: none;
            border-radius: 12px;
            padding: 14px 28px;
            font-size: 16px;
            font-weight: 600;
            margin: 10px 6px;
            min-width: 100px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #5BA3F5, stop:1 #4A90E2);
            transform: translateY(-2px);
            box-shadow: 0 4px 12px rgba(74, 144, 226, 0.3);
        }

        QPushButton:pressed {
            background: #357ABD;
            transform: translateY(0px);
        }

        QPushButton#registerBtn {
            background: transparent;
            color: %1;
            border: 3px solid %1;
        }

        QPushButton#registerBtn:hover {
            background: %1;
            color: white;
            box-shadow: 0 4px 12px rgba(74, 144, 226, 0.3);
        }

        QLabel#statusLabel {
            color: %2;
            font-size: 14px;
            padding: 8px 12px;
            border-radius: 8px;
            background: rgba(208, 2, 27, 0.1);
            border: 1px solid rgba(208, 2, 27, 0.2);
        }
    )").arg(PRIMARY_COLOR, DANGER_COLOR);
}


QString CampusStyles::getMainWindowStyle() {
    return QString(R"(
        QMainWindow {
            background: %1;
            color: #2C3E50;
            font-family: 'Microsoft YaHei UI', '微软雅黑', Arial, sans-serif;
        }

        QMenuBar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 %2, stop:1 %3);
            color: white;
            border: none;
            padding: 6px;
            font-weight: 600;
        }

        QMenuBar::item {
            background: transparent;
            padding: 12px 16px;
            border-radius: 8px;
            margin: 0 4px;
        }

        QMenuBar::item:selected {
            background: rgba(255, 255, 255, 0.2);
            color: white;
        }

        QMenu {
            background: white;
            border: 2px solid #E8E8E8;
            border-radius: 12px;
            padding: 12px 0;
            box-shadow: 0 8px 24px rgba(0,0,0,0.1);
        }

        QMenu::item {
            padding: 12px 24px;
            margin: 4px 12px;
            border-radius: 8px;
            font-size: 14px;
            color: black; /* 将文字颜色改为黑色 */
            font-weight: bold; /* 加粗文字 */
        }

        QMenu::item:selected {
            background: %2;
            color: white;
        }

        QStatusBar {
            background: white;
            border-top: 2px solid #E8E8E8;
            padding: 8px;
            font-size: 14px;
            color: #7F8C8D;
        }
    )").arg(BACKGROUND_COLOR, PRIMARY_COLOR, SECONDARY_COLOR);
}

QString CampusStyles::getTabStyle() {
    return QString(R"(
        QTabWidget::pane {
            border: 2px solid #E8E8E8;
            background: white;
            border-radius: 12px;
            margin-top: -2px;
        }

        QTabWidget::tab-bar {
            alignment: center;
        }

        QTabBar::tab {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #F8F9FA, stop:1 #E9ECEF);
            color: #6C757D;
            padding: 14px 28px;
            margin-right: 4px;
            border-top-left-radius: 12px;
            border-top-right-radius: 12px;
            border: 2px solid #E8E8E8;
            border-bottom: none;
            font-weight: 600;
            font-size: 14px;
            min-width: 120px;
        }

        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 white, stop:1 #F8FAFE);
            color: %1;
            border-color: %1;
            border-bottom: 3px solid %1;
        }

        QTabBar::tab:hover:!selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #F0F8FF, stop:1 #E6F3FF);
            color: %1;
            border-color: %1;
        }

        QTabBar::tab:first {
            margin-left: 0;
        }
    )").arg(PRIMARY_COLOR);
}

QString CampusStyles::getPrimaryButtonStyle() {
    return QString(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 %1, stop:1 #357ABD);
            color: white;
            border: none;
            border-radius: 10px;
            padding: 12px 24px;
            font-size: 14px;
            font-weight: 600;
            margin: 6px 4px;
            min-width: 90px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #5BA3F5, stop:1 #4A90E2);
            transform: translateY(-2px);
            box-shadow: 0 4px 12px rgba(74, 144, 226, 0.3);
        }

        QPushButton:pressed {
            background: #357ABD;
            transform: translateY(0px);
        }

        QPushButton:disabled {
            background: #BDC3C7;
            color: #7F8C8D;
        }
    )").arg(PRIMARY_COLOR);
}

QString CampusStyles::getSecondaryButtonStyle() {
    return QString(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 %1, stop:1 #E8510E);
            color: white;
            border: none;
            border-radius: 10px;
            padding: 12px 24px;
            font-size: 14px;
            font-weight: 600;
            margin: 6px 4px;
            min-width: 90px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #FF8A65, stop:1 #FF6B35);
            transform: translateY(-2px);
            box-shadow: 0 4px 12px rgba(255, 107, 53, 0.3);
        }

        QPushButton:pressed {
            background: #E8510E;
            transform: translateY(0px);
        }
    )").arg(SECONDARY_COLOR);
}

QString CampusStyles::getDangerButtonStyle() {
    return QString(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 %1, stop:1 #B71C1C);
            color: white;
            border: none;
            border-radius: 10px;
            padding: 12px 24px;
            font-size: 14px;
            font-weight: 600;
            margin: 6px 4px;
            min-width: 90px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #EF5350, stop:1 #D0021B);
            transform: translateY(-2px);
            box-shadow: 0 4px 12px rgba(208, 2, 27, 0.3);
        }

        QPushButton:pressed {
            background: #B71C1C;
            transform: translateY(0px);
        }
    )").arg(DANGER_COLOR);
}

QString CampusStyles::getLineEditStyle() {
    return QString(R"(
        QLineEdit {
            background: white;
            border: 2px solid #E8E8E8;
            border-radius: 10px;
            padding: 12px 16px;
            font-size: 14px;
            color: #2C3E50;
            margin: 4px 0;
        }

        QLineEdit:focus {
            border-color: %1;
            background: white;
            box-shadow: 0 0 0 3px rgba(74, 144, 226, 0.1);
        }

        QLineEdit:hover {
            border-color: #D5D5D5;
        }

        QLineEdit:disabled {
            background: #F8F9FA;
            color: #6C757D;
            border-color: #E9ECEF;
        }
    )").arg(PRIMARY_COLOR);
}

QString CampusStyles::getTextEditStyle() {
    return QString(R"(
        QTextEdit {
            background: white;
            border: 2px solid #E8E8E8;
            border-radius: 10px;
            padding: 12px;
            font-size: 14px;
            color: #2C3E50;
            font-family: 'Microsoft YaHei UI', '微软雅黑', Arial, sans-serif;
            line-height: 1.6;
        }

        QTextEdit:focus {
            border-color: %1;
            box-shadow: 0 0 0 3px rgba(74, 144, 226, 0.1);
        }

        QTextEdit:hover {
            border-color: #D5D5D5;
        }
    )").arg(PRIMARY_COLOR);
}

QString CampusStyles::getTableStyle() {
    return QString(R"(
        QTableWidget {
            background: white;
            border: 2px solid #E8E8E8;
            border-radius: 12px;
            gridline-color: #F1F3F4;
            font-size: 14px;
            selection-background-color: rgba(74, 144, 226, 0.2);
            alternate-background-color: #F8FAFE;
            color: black; /* 设置文字颜色为黑色 */
            outline: none; /* 去除选中时的外边框 */
        }

        QTableWidget::item {
            padding: 12px;
            border-bottom: 1px solid #F1F3F4;
            color: black; /* 设置文字颜色为黑色 */
        }

        QTableWidget::item:selected {
            background: rgba(74, 144, 226, 0.2);
            color: black; /* 选中时文字颜色为黑色 */
            border: none; /* 去除选中时的边框 */
            padding: 12px; /* 确保选中时的边距一致 */
        }

        QTableWidget::item:hover {
            background: rgba(74, 144, 226, 0.1);
            color: black; /* 悬停时文字颜色为黑色 */
        }

        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 %1, stop:1 #357ABD);
            color: white;
            padding: 14px;
            border: none;
            font-weight: 600;
            font-size: 14px;
        }

        QHeaderView::section:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #5BA3F5, stop:1 #4A90E2);
        }

        QScrollBar:vertical {
            background: #F8F9FA;
            width: 14px;
            border-radius: 7px;
            margin: 2px;
        }

        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 %1, stop:1 %2);
            border-radius: 7px;
            min-height: 30px;
        }

        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #5BA3F5, stop:1 #FF8A65);
        }
    )").arg(PRIMARY_COLOR, SECONDARY_COLOR);
}



QString CampusStyles::getListStyle() {
    return QString(R"(
        QListWidget {
            background: white;
            border: 2px solid #E8E8E8;
            border-radius: 12px;
            padding: 8px;
            font-size: 14px;
            selection-background-color: rgba(74, 144, 226, 0.2);
        }

        QListWidget::item {
            padding: 12px 16px;
            margin: 3px 0;
            border-radius: 8px;
            border: 1px solid transparent;
        }

        QListWidget::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(74, 144, 226, 0.2), stop:1 rgba(255, 107, 53, 0.2));
            color: #2C3E50;
            border-color: %1;
        }

        QListWidget::item:hover:!selected {
            background: rgba(74, 144, 226, 0.1);
            border-color: rgba(74, 144, 226, 0.3);
        }

        QScrollBar:vertical {
            background: #F8F9FA;
            width: 14px;
            border-radius: 7px;
        }

        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 %1, stop:1 %2);
            border-radius: 7px;
            min-height: 30px;
        }

        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #5BA3F5, stop:1 #FF8A65);
        }
    )").arg(PRIMARY_COLOR, SECONDARY_COLOR);
}

QString CampusStyles::getMessageAreaStyle() {
    return QString(R"(
        /* 留言区域基础样式 */
        QScrollArea {
            background: white;
            border: 2px solid #E8E8E8;
            border-radius: 12px;
        }

        QWidget#messageContainer {
            background: white;
        }

        /* 留言项基础样式 */
        QWidget[messageType="message"] {
            background: #E3F2FD;
            border: 1px solid rgba(74, 144, 226, 0.2);
            border-radius: 12px;
            margin: 8px;
            padding: 12px;
        }

        /* 不同用户的留言底色样式类 */
        QWidget[messageColor="0"] { background: #E3F2FD; border-color: rgba(74, 144, 226, 0.3); }
        QWidget[messageColor="1"] { background: #F3E5F5; border-color: rgba(156, 39, 176, 0.3); }
        QWidget[messageColor="2"] { background: #E8F5E8; border-color: rgba(76, 175, 80, 0.3); }
        QWidget[messageColor="3"] { background: #FFF3E0; border-color: rgba(255, 152, 0, 0.3); }
        QWidget[messageColor="4"] { background: #FCE4EC; border-color: rgba(233, 30, 99, 0.3); }
        QWidget[messageColor="5"] { background: #E0F2F1; border-color: rgba(0, 150, 136, 0.3); }
        QWidget[messageColor="6"] { background: #FFF8E1; border-color: rgba(255, 235, 59, 0.3); }
        QWidget[messageColor="7"] { background: #F1F8E9; border-color: rgba(139, 195, 74, 0.3); }
        QWidget[messageColor="8"] { background: #E8EAF6; border-color: rgba(103, 58, 183, 0.3); }
        QWidget[messageColor="9"] { background: #FFEBEE; border-color: rgba(244, 67, 54, 0.3); }
        QWidget[messageColor="10"] { background: #E4F7F7; border-color: rgba(0, 188, 212, 0.3); }
        QWidget[messageColor="11"] { background: #FDF2F8; border-color: rgba(236, 64, 122, 0.3); }

        /* 留言内容样式 */
        QLabel[contentType="username"] {
            color: #2C3E50;
            font-weight: bold;
            font-size: 14px;
            margin-bottom: 4px;
        }

        QLabel[contentType="message"] {
            color: #34495E;
            font-size: 13px;
            line-height: 1.5;
            margin: 4px 0;
        }

        QLabel[contentType="timestamp"] {
            color: #7F8C8D;
            font-size: 11px;
            margin-top: 4px;
        }

        /* 留言项悬停效果 */
        QWidget[messageType="message"]:hover {
            transform: translateY(-1px);
            box-shadow: 0 4px 12px rgba(0,0,0,0.1);
        }

        /* 滚动条样式 */
        QScrollBar:vertical {
            background: #F8F9FA;
            width: 14px;
            border-radius: 7px;
            margin: 2px;
        }

        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 %1, stop:1 %2);
            border-radius: 7px;
            min-height: 30px;
        }

        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #5BA3F5, stop:1 #FF8A65);
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            border: none;
            background: none;
        }
    )").arg(PRIMARY_COLOR, SECONDARY_COLOR);
}

QString CampusStyles::getCardStyle() {
    return QString(R"(
        QWidget {
            background: white;
            border: 2px solid #E8E8E8;
            border-radius: 16px;
            margin: 10px;
            padding: 16px;
        }

        QWidget:hover {
            border-color: rgba(74, 144, 226, 0.5);
            box-shadow: 0 8px 24px rgba(74, 144, 226, 0.15);
            transform: translateY(-2px);
        }

        QWidget[cardType="highlight"] {
            border-color: %1;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(74, 144, 226, 0.05),
                stop:1 rgba(255, 107, 53, 0.05));
        }
    )").arg(PRIMARY_COLOR);
}
