QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    dialog.cpp \
    ../primelist/primelist.cpp

HEADERS += \
    dialog.h \
    ../primelist/primelist.h

FORMS += \
    dialog.ui
