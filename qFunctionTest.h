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

class QFunctionTest : public QWidget
{
    Q_OBJECT
public:
    QFunctionTest();
    void ResetText(QString func_info);
    void SetAllName(QString db_name,QString func_name);
    void SetText(QString info);
private:
    QPushButton *pushButton;
    QTextEdit *textInfo;
    QString info,funcName,dbName;
    private slots:
    void ApplyFunction();
	void DropTheFunction();
    void AlterFunction();
};

#endif /* qFunctionTest_h */
