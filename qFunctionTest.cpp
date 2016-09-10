//
//  qFunctionTest.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#include "qFunctionTest.h"
#include "mysqlQuery.h"

qFunctionTest::qFunctionTest()
{
    QPushButton *pushButton = new QPushButton(tr("apply new function"),this);
    pushButton->setGeometry(QRect(70,40,75,23));
    QPushButton *dropButton = new QPushButton(tr("drop one function"),this);
    dropButton->setGeometry(QRect(70,40,75,23));
    QPushButton *alterButton = new QPushButton(tr("alter one function"),this);
    alterButton->setGeometry(QRect(70,40,75,23));
    textInfo = new QTextEdit(this);
    textInfo->setPlainText("DELIMITER $$ CREATE FUNCTION `new_function`() RETURNS INTEGER BEGIN RETURN 1;END$$ DELIMITER");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(textInfo);
    mainLayout->addWidget(pushButton);
    mainLayout->addWidget(dropButton);
    mainLayout->addWidget(alterButton);
    this->setLayout(mainLayout);
    
    connect(pushButton,SIGNAL(clicked()),this,SLOT(applyFunction()));
    connect(dropButton,SIGNAL(clicked()),this,SLOT(dropFunction()));
    connect(alterButton,SIGNAL(clicked()),this,SLOT(alterFunction()));
}

void qFunctionTest::applyFunction()
{
    QString info;
    string res;
    info = textInfo->toPlainText();
    res=create_function(dbName,info);
    if(res.compare("success")==0)
    {
        QMessageBox::information(this,"SUCCESS!","create function success");
    }else
    {
        QMessageBox::information(this, "ERROR!", QString::fromStdString(res));
    }
    //qDebug("%s",info);
}

void qFunctionTest::dropFunction()
{
    string res;
    res = drop_function(dbName,funcName);
    if(res.compare("success")==0)
    {
        QMessageBox::information(this,"SUCCESS!","drop function success");
    }
    else
    {
        QMessageBox::information(this, "ERROR!", QString::fromStdString(res));
    }
}

void qFunctionTest::alterFunction()
{
    QString res;
    res = get_function(dbName,funcName);
    textInfo->setPlainText(res);
}

void qFunctionTest::setText(QString info)
{
    textInfo->setPlainText(info);
}

void qFunctionTest::setAllName(QString db_name,QString func_name)
{
    if(db_name!=NULL&&func_name!=NULL)
    {
        dbName = db_name;
        funcName = func_name;
    }
}

