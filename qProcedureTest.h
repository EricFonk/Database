//
//  qProcedureTest.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#ifndef qProcedureTest_h
#define qProcedureTest_h

#include <QtWidgets>
#include <QtGui>
#include <QString>

class QProcedureTest : public QWidget
{
    Q_OBJECT
public:
    QProcedureTest();
    void SetAllName(QString db_name,QString pro_name);
    void SetText(QString info);
private:
    QPushButton *pushButton;
    QTextEdit *textInfo;
    QString info,proName,dbName;
    private slots:
    void ApplyProcedure();
    void DropTheProcedure();
    void AlterProcedure();
};

#endif /* qProcedureTest_h */
