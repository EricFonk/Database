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
    QLineEdit *newTableName;//���������
    QLabel *dbNameLabel;//����ģʽ��ǩ
    QString dbName;//���ݿ���
    int columns_num;//����
    void SetDBName(QString);//�������ݿ�
    void LoadTableInfo(QString);
signals:
    void CreateTableSuccess(QString tableName);
    public slots:
    void ApplyCreate(bool b=false);//�ύ������
    void InsertCol();//��������
};

#endif /* table_create_h */
