//
//  connectwindow.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/10.
//
//

#include "connectwindow.h"

connectwindow::connectwindow()
{
    setupUI();
}

connectwindow::~connectwindow()
{
    
}

void connectwindow::setupUI()
{
    //    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //    sizePolicy.setHorizontalStretch(0);
    //    sizePolicy.setVerticalStretch(0);
    //    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    //    this->setSizePolicy(sizePolicy);
    
    gridLayout = new QVBoxLayout(this);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    
    dataView=new QTableView();
    dataView->setMinimumWidth(300);
    dataView->setMinimumHeight(300);
    QSizePolicy tableSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tableSizePolicy.setHorizontalStretch(0);
    tableSizePolicy.setVerticalStretch(0);
    tableSizePolicy.setHeightForWidth(dataView->sizePolicy().hasHeightForWidth());
    dataView->setSizePolicy(tableSizePolicy);
    gridLayout->addWidget(dataView);
    
    bottomWidget=new QWidget(this);
    //    bottomWidget->setMinimumWidth(300);
    //    bottomWidget->setMinimumHeight(300);
    QSizePolicy bottomSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bottomSizePolicy.setHorizontalStretch(0);
    bottomSizePolicy.setVerticalStretch(0);
    bottomSizePolicy.setHeightForWidth(bottomWidget->sizePolicy().hasHeightForWidth());
    bottomWidget->setSizePolicy(bottomSizePolicy);
    bottomLayout=new QHBoxLayout(bottomWidget);
    
    newBtn=new QPushButton("新建连接");
    deleteBtn=new QPushButton("删除链接");
    
    bottomLayout->addWidget(newBtn);
    bottomLayout->addWidget(deleteBtn);
    gridLayout->addWidget(bottomWidget);
    
    resize(850, 450);
    
    QObject::connect(dataView, SIGNAL(clicked(QModelIndex)), this, SLOT(Refresh(QModelIndex)));
    QObject::connect(newBtn, SIGNAL(clicked()), this, SLOT(openNewConn()));
    QObject::connect(deleteBtn, SIGNAL(clicked()), this, SLOT(RemoveOneConn()));
}

QStringList connectwindow::getConnections()
{
    char buffer[256];
    QStringList messageList;
    
    FILE*fp = NULL;
    fp = fopen("/Users/vaaaas/Documents/Program/QT/Workbench_01/Connections.ini", "r");
    fgets(buffer, 256, fp);
    do
    {
        std::string myStr = buffer;
        QString st = QString::fromStdString(myStr);
        messageList.append(st);
    } while (fgets(buffer, 256, fp));
    fclose(fp);
    
    return messageList;
}

void connectwindow::Login()
{
    
}

void connectwindow::openNewConn()
{
    createConnectWindow* createConD=new createConnectWindow();
    createConD->show();
}

void connectwindow::Refresh(QModelIndex index)
{
    int a = dataView->currentIndex().row();
    QStringList conns = getConnections();
    QStringList finalResult;
    
    for(int i=0;i<=a;i++)
    {
        if (i == a)
        {
            finalResult = conns[i].split('|');
        }
    }
    
    std::string nowHost = finalResult[1].toStdString();
    unsigned int nowPort = finalResult[2].toInt();
    std::string nowUserName = finalResult[3].toStdString();
    std::string nowPswd = finalResult[4].toStdString();
    
    MYSQL mysql;
    
    if (mysql_init(&mysql) != NULL)
    {
        if (mysql_real_connect(&mysql, nowHost.c_str(), nowUserName.c_str(), nowPswd.c_str(), "workbenchdb", 0, NULL, 0) != NULL)
        {
            QMessageBox::information(NULL, "Connect Test", "Connect Success");
            MainWindow mw;
            mw.show();
            return;
        }
    }
    //QMessageBox::information(NULL, "Connect Test", "Connect Failed");
    //MainWindow mw;
    //mw.show();
}

void connectwindow::RemoveOneConn()
{
    int a = dataView->currentIndex().row();
    char buffer[256];
    QStringList messageList;
    
    
    //!!!!!!!!
    FILE*fp = NULL;//需要注意
    fp = fopen("/Users/vaaaas/Documents/Program/QT/Workbench/Connections", "r");
    
    FILE*fpTemp = NULL;
    fpTemp = fopen("/Users/vaaaas/Documents/Program/QT/Workbench/temp", "w");
    int i = 0;
    while (fgets(buffer, 256, fp))
    {
        if (i != a)
        {
            fwrite(buffer, sizeof(buffer), 1, fpTemp);
        }
        i++;
    }
    fclose(fp);
    fclose(fpTemp);
    remove("/Users/vaaaas/Documents/Program/QT/Workbench/Connections");
    rename("/Users/vaaaas/Documents/Program/QT/Workbench/temp", "/Users/vaaaas/Documents/Program/QT/Workbench/Connections");
}