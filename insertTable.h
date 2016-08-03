//
//  insertTable.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#ifndef insertTable_h
#define insertTable_h

#include <QtWidgets>
#include <QtGui>
#include <QString>
//#include <QDialog>

class InsertTable : public QWidget
{
    Q_OBJECT
public:
    InsertTable(QString table_name);
    void SetHeaders(int,QStringList);
private:
    //QDialog *tableview;
    QTableWidget *table;
    QStringList qHead;
    QString tableName;
    int qSumCol;
    private slots:
    void ApplyInsert();
};

#endif /* insertTable_h */
