//
//  table_alter.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#ifndef table_alter_h
#define table_alter_h

#include <QtWidgets>
#include <QtGui>
#include <QString>
#include <QStringList>

class TableAlt:public QWidget
{
    Q_OBJECT
    
public:
    TableAlt();
    QTableWidget *tableOpt;
    QLineEdit *newTableName;//表名输入框
    QLabel *dbNameLabel;//所属模式标签
    
    QString dbName;//数据库名
    QString tName;//数据表名
    int columns_num;//列数量
    int onInsertFlag;//是否正在进行插入操作的标志
    QStringList col_name;//列名list
    QStringList col_default;//默认值list
    QStringList col_isNull;//空约束list
    QStringList col_type;//列数据类型list
    QStringList col_key;//键信息list
    QStringList col_extra;//
    void setDTName(QString dbname, QString tname);
    void loadTableInfo();
    
    void renameTable();
    void updateAllInfoAfter(QStringList colNumList,int optType);
    private slots:
    void onDataChange(const QModelIndex &topLeft, QModelIndex &buttomRight);
    void insertCol();
    void confirmInsertCol();
    void dropCol();
    void applyAlter(bool);
};

#endif /* table_alter_h */
