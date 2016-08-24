//
//  qProcedureTest.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#include "qProcedureTest.h"
#include "mysqlQuery.h"

QProcedureTest::QProcedureTest()
{
    QPushButton *pushButton = new QPushButton(tr("apply new procedure"),this);
    pushButton->setGeometry(QRect(70,40,75,23));
    QPushButton *dropButton = new QPushButton(tr("drop one procedure"),this);
    dropButton->setGeometry(QRect(70,40,75,23));
    QPushButton *alterButton = new QPushButton(tr("alter one procedure"),this);
    alterButton->setGeometry(QRect(70,40,75,23));
    textInfo = new QTextEdit(this);
    textInfo->setPlainText("CREATE PROCEDURE `new_procedure`()\nBEGIN\n\nEND");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(textInfo);
    mainLayout->addWidget(pushButton);
    mainLayout->addWidget(dropButton);
    mainLayout->addWidget(alterButton);
    this->setLayout(mainLayout);
    
    connect(pushButton,SIGNAL(clicked()),this,SLOT(ApplyProcedure()));
    connect(dropButton,SIGNAL(clicked()),this,SLOT(DropProcedure()));
    connect(alterButton,SIGNAL(clicked()),this,SLOT(AlterProcedure()));
}

void QProcedureTest::ApplyProcedure()
{
    QString info;
    string res;
    info = textInfo->toPlainText();
    res=CreateProcedure(dbName,info);
    if(res.compare("success")==0)
    {
        QMessageBox::information(this,"SUCCESS!","create procedure success");
    }else
    {
        QMessageBox::information(this, "ERROR!", QString::fromStdString(res));
    }
    //qDebug("%s",info);
}

void QProcedureTest::DropTheProcedure()
{
    string res;
    res = DropProcedure(dbName,proName);
    if(res.compare("success")==0)
    {
        QMessageBox::information(this,"SUCCESS!","drop procedure success");
    }
    else
    {
        QMessageBox::information(this, "ERROR!", QString::fromStdString(res));
    }
}

void QProcedureTest::AlterProcedure()
{
    QString res;
    res = GetProcedure(dbName,proName);
    textInfo->setPlainText(res);
}

void QProcedureTest::SetAllName(QString db_name, QString pro_name)
{
    if(db_name!=NULL&&pro_name!=NULL)
    {
        dbName = db_name;
        proName = pro_name;
    }
}

void QProcedureTest::SetText(QString info)
{
    textInfo->setPlainText(info);
}
