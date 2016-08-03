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

class qViewTest : public QWidget
{
    Q_OBJECT
public:
    qViewTest();
    void resetText(QString view_info);
    void setAllName(QString db_name,QString view_name);
    void setText(QString info);
private:
    QPushButton *pushButton;
    QTextEdit *textInfo;
    QString info,viewName,dbName;
    private slots:
    void applyView();
    void dropView();
    void alterView();
};

#endif /* qViewTest_h */
