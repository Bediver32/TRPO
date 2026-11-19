TEMPLATE = app
TARGET = primelist_tests
QT = core testlib
CONFIG += console testcase c++17
CONFIG -= app_bundle

SOURCES += tst_primelist.cpp

include(../primelist_lib/primelist_link.pri)
