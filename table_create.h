//
//  table_create.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#ifndef table_create_h
#define table_create_h

#include <QtWidgets>
#include <QtGui>
#include <QString>
#include <QStringList>

class TableCrt : public QWidget
{
    Q_OBJECT
    
public:
    TableCrt();
    void InitAllTables(QStringList, QString);
public:
    QTableWidget *tableOpt;
    QLineEdit *newTableName;//表名输入框
    QLabel *dbNameLabel;//所属模式标签
    QString dbName;//数据库名
    int columns_num;//列名
    void SetDBName(QString);//设置数据库
    void LoadTableInfo(QString);
signals:
    void CreateTableSuccess(QString tableName);
    public slots:
    void ApplyCreate(bool b=false);//提交建表动作
    void InsertCol();//插入新列
};

#endif /* table_create_h */
