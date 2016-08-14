#-------------------------------------------------
#
# Project created by QtCreator 2016-08-02T11:27:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorkBench
TEMPLATE = app

INCLUDEPATH += . \
                /usr/local/mysql-5.7.13-osx10.11-x86_64/include/
LIBS +=     . \
            /usr/local/mysql-5.7.13-osx10.11-x86_64/lib/


SOURCES += main.cpp\
    mainwindow.cpp\
    insertTable.cpp \
    mylistview.cpp \
    mysqlQuery.cpp \
    qFunctionTest.cpp \
    qProcedureTest.cpp \
    qViewTest.cpp \
    schemas_test.cpp \
    table_alter.cpp \
    table_create.cpp \
    tablewidget.cpp \
    connectwindow.cpp \
    createConnectWindow.cpp \
    panel_userprivileges.cpp


HEADERS  += mainwindow.h \
    insertTable.h \
    mylistview.h \
    mysqlQuery.h \
    qFunctionTest.h \
    qProcedureTest.h \
    qViewTest.h \
    schemas_test.h \
    table_alter.h \
    table_create.h \
    tablewidget.h \
    connectwindow.h \
    createConnectWindow.h \
    panel_userprivileges.h
