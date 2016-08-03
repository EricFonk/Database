//
//  qViewTest.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#include "qViewTest.h"
#include "mysqlQuery.h"

qViewTest::qViewTest()
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
    
    connect(pushButton,SIGNAL(clicked()),this,SLOT(applyView()));
    connect(dropButton,SIGNAL(clicked()),this,SLOT(dropView()));
    connect(alterButton,SIGNAL(clicked()),this,SLOT(alterView()));
}

void qViewTest::applyView()
{
    QString info;
    string res;
    info = textInfo->toPlainText();
    //新建视图
    res = create_view("xfxf",info);
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

void qViewTest::dropView()
{
    QString my_dbName,my_viewName;
    string res;
    my_dbName = "xfxf";
    my_viewName = "new_view";
    //删除视图
    res = drop_view(my_dbName,my_viewName);
    if(res.compare("success")==0)
    {
        QMessageBox::information(this,"SUCCESS!","drop view success");
    }
    else
    {
        QMessageBox::information(this,"ERROR!",QString::fromStdString(res));
    }
}

void qViewTest::alterView()
{
    QString res;
    //修改视图
    res = get_view("xfxf","new_view");
    textInfo->setPlainText(res);
}

void qViewTest::setText(QString info)
{
    textInfo->setPlainText(info);
}

void qViewTest::setAllName(QString db_name,QString view_name)
{
    if(db_name!=NULL&&view_name!=NULL)
    {
        dbName = db_name;
        viewName = view_name;
    }
}
