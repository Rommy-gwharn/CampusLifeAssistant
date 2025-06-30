QT += core gui widgets

CONFIG += c++11

TARGET = CampusLifeAssistant
TEMPLATE = app

SOURCES += \
    campus_styles.cpp \
    main.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    coursewidget.cpp \
    taskwidget.cpp \
    socialwidget.cpp

HEADERS += \
    campus_styles.h \
    loginwindow.h \
    mainwindow.h \
    coursewidget.h \
    taskwidget.h \
    socialwidget.h

FORMS += \
    loginwindow.ui

DISTFILES += \
    CampusLifeAssistant.pro.user \
    CampusLifeAssistant.pro.user.5907f6f \
    CampusLifeAssistant.pro.user.793b53e

TRANSLATIONS += \
    CampusLifeAssistant_zh_CN.ts
