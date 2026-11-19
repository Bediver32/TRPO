TEMPLATE = app
TARGET = primelist_gui_tests
QT = widgets testlib
CONFIG += console testcase c++17
CONFIG -= app_bundle

SOURCES += tst_primelist_gui.cpp ../primelist_gui/dialog.cpp
HEADERS += ../primelist_gui/dialog.h
FORMS += ../primelist_gui/dialog.ui
DISTFILES += .astylerc
INCLUDEPATH += ../primelist_gui

include(../primelist_lib/primelist_link.pri)
