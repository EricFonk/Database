//
//  qFunctionTest.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#ifndef qFunctionTest_h
#define qFunctionTest_h

#include <QtWidgets>
#include <QtGui>
#include <QString>

class qFunctionTest : public QWidget
{
    Q_OBJECT
public:
    qFunctionTest();
    void resetText(QString func_info);
    void setAllName(QString db_name,QString func_name);
    void setText(QString info);
private:
    QPushButton *pushButton;
    QTextEdit *textInfo;
    QString info,funcName,dbName;
    private slots:
    void applyFunction();
    void dropFunction();
    void alterFunction();
};

#endif /* qFunctionTest_h */
