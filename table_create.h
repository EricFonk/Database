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
    void initAllTables(QStringList, QString);
public:
    QTableWidget *tableOpt;
    QLineEdit *newTableName;//���������
    QLabel *dbNameLabel;//����ģʽ��ǩ
    QString dbName;//���ݿ���
    int columns_num;//����
    void setDBName(QString);//�������ݿ�
    void loadTableInfo(QString);
signals:
    void createTableSuccess(QString tableName);
    public slots:
    void applyCreate(bool b=false);//�ύ������
    void insertCol();//��������
};

#endif /* table_create_h */
