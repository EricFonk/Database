//
//  qViewTest.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#ifndef qViewTest_h
#define qViewTest_h

#include <QtWidgets>
#include <QtGui>
#include <QString>

class QViewTest : public QWidget
{
    Q_OBJECT
public:
    QViewTest();
    void ResetText(QString view_info);
    void SetAllName(QString db_name,QString view_name);
    void SetText(QString info);
private:
    QPushButton *pushButton;
    QTextEdit *textInfo;
    QString info,viewName,dbName;
    private slots:
    void ApplyView();
    void DropTheView();
    void AlterView();
};

#endif /* qViewTest_h */
