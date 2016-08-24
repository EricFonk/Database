//
//  qViewTest.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#include "qViewTest.h"
#include "mysqlQuery.h"

QViewTest::QViewTest()
{
    QPushButton *pushButton = new QPushButton(tr("apply new view"));
    pushButton->setGeometry(QRect(70,40,75,23));
    QPushButton *dropButton =new QPushButton(tr("drop one view"));
    dropButton->setGeometry(QRect(70,40,75,23));
    QPushButton *alterButton = new QPushButton(tr("alter one view"));
    alterButton->setGeometry(QRect(70,40,75,23));
    textInfo = new QTextEdit(this);
    textInfo->setPlainText("CREATE VIEW `one_database`.`new_view` AS");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(textInfo);
    mainLayout->addWidget(pushButton);
    mainLayout->addWidget(dropButton);
    mainLayout->addWidget(alterButton);
    this->setLayout(mainLayout);
    
    connect(pushButton,SIGNAL(clicked()),this,SLOT(ApplyView()));
    connect(dropButton,SIGNAL(clicked()),this,SLOT(DropTheView()));
    connect(alterButton,SIGNAL(clicked()),this,SLOT(AlterView()));
}

void QViewTest::ApplyView()
{
    QString info;
    string res;
    info = textInfo->toPlainText();
    //新建视图
    res = CreateView("xfxf",info);
    if(res.compare("success")==0)
    {
        QMessageBox::information(this,"SUCCESS","create view success");
    }
    else
    {
        QMessageBox::information(this,"ERROR",QString::fromStdString(res));
    }
    //qDebug("%s",info);
}

void QViewTest::DropTheView()
{
    QString my_dbName,my_viewName;
    string res;
    my_dbName = "xfxf";
    my_viewName = "new_view";
    //删除视图
    res = DropView(my_dbName,my_viewName);
    if(res.compare("success")==0)
    {
        QMessageBox::information(this,"SUCCESS!","drop view success");
    }
    else
    {
        QMessageBox::information(this,"ERROR!",QString::fromStdString(res));
    }
}

void QViewTest::AlterView()
{
    QString res;
    //修改视图
    res = GetView("xfxf","new_view");
    textInfo->setPlainText(res);
}

void QViewTest::SetText(QString info)
{
    textInfo->setPlainText(info);
}

void QViewTest::SetAllName(QString db_name, QString view_name)
{
    if(db_name!=NULL&&view_name!=NULL)
    {
        dbName = db_name;
        viewName = view_name;
    }
}
