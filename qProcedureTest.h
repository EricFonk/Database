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

class qProcedureTest : public QWidget
{
    Q_OBJECT
public:
    qProcedureTest();
    void setAllName(QString db_name,QString pro_name);
    void setText(QString info);
private:
    QPushButton *pushButton;
    QTextEdit *textInfo;
    QString info,proName,dbName;
    private slots:
    void applyProcedure();
    void dropProcedure();
    void alterProcedure();
};

#endif /* qProcedureTest_h */
