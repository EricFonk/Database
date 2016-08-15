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
    
    QStringList ConnectionList = getConnections();
    QStandardItemModel* Listmodel = new QStandardItemModel(ConnectionList.length() - 1, 4);
    Listmodel->setHeaderData(0, Qt::Horizontal, QString("Conn Name"));
    Listmodel->setHeaderData(1, Qt::Horizontal, QString("Host"));
    Listmodel->setHeaderData(2, Qt::Horizontal, QString("Port"));
    Listmodel->setHeaderData(3, Qt::Horizontal, QString("User Name"));
    for (int i = 0; i<ConnectionList.length()-1; i++)
    {
        QStringList oneConn = ConnectionList[i].split('|');
        for (int j = 0; j<oneConn.length()-1; j++)
        {
            QModelIndex index = Listmodel->index(i, j, QModelIndex());
            Listmodel->setData(index, QVariant(oneConn[j]));
        }
    }
    dataView->setModel(Listmodel);
    dataView->resizeColumnToContents(4);
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
    
    connectBtn=new QPushButton("登录连接");
    newBtn=new QPushButton("新建连接");
    deleteBtn=new QPushButton("删除链接");
    
    bottomLayout->addWidget(connectBtn);
    bottomLayout->addWidget(newBtn);
    bottomLayout->addWidget(deleteBtn);
    gridLayout->addWidget(bottomWidget);
    
    resize(850, 450);
    
    QObject::connect(dataView, SIGNAL(clicked(QModelIndex)), this, SLOT(Refresh(QModelIndex)));
    QObject::connect(connectBtn, SIGNAL(clicked()),this,SLOT(loginConnect()));
    QObject::connect(newBtn, SIGNAL(clicked()), this, SLOT(openNewConn()));
    QObject::connect(deleteBtn, SIGNAL(clicked()), this, SLOT(RemoveOneConn()));
}

QStringList connectwindow::getConnections()
{
    char buffer[256];
    QStringList messageList;
    
    ifstream fp("/Users/vaaaas/Documents/Program/QT/Workbench/Connections");
    if(!fp.is_open()){
        cout<<"Error opening file Connections";
        exit(1);
    }else{
        while(!fp.eof())
        {
            fp.getline(buffer, 100);
            QString *st=new QString(buffer);
            messageList.append(*st);
        }
        fp.close();
    }
    
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
    
//    MYSQL mysql;
//    
//    if (mysql_init(&mysql) != NULL)
//    {
//        if (mysql_real_connect(&mysql, nowHost.c_str(), nowUserName.c_str(), nowPswd.c_str(), "workbenchdb", 0, NULL, 0) != NULL)
//        {
//            QMessageBox::information(NULL, "Connect Test", "Connect Success");
//            //mw=new MainWindow();
//            //mw->show();
//            return;
//        }
//    }
    //QMessageBox::information(NULL, "Connect Test", "Connect Failed");
    //MainWindow mw;
    //mw.show();
}

void connectwindow::loginConnect()
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
            mw=new MainWindow();
            mw->show();
            return;
        }
    }

}

void connectwindow::RemoveOneConn()
{
    int a = dataView->currentIndex().row();
    char buffer[256];
    QStringList messageList;
    
    ifstream fp("/Users/vaaaas/Documents/Program/QT/Workbench/Connections");
    if(!fp.is_open()){
        cout<<"Error opening file Connections";
        exit(1);
    }else
    {
        ofstream saveFile;
        saveFile.open("/Users/vaaaas/Documents/Program/QT/Workbench/temp",ios::out|ios::trunc);
        if(!saveFile.is_open()){
            cout<<"Error opening file temp";
            exit(1);
        }else
        {
            int i = 0;
            
            while(!fp.eof())
            {
                fp.getline(buffer, 256);
                if(i!=a)
                {
                    saveFile<<buffer;
                    saveFile<<"\n";
                }
                i++;
            }
            fp.close();
            saveFile.close();
        }
    }
    remove("/Users/vaaaas/Documents/Program/QT/Workbench/Connections");
    rename("/Users/vaaaas/Documents/Program/QT/Workbench/temp", "/Users/vaaaas/Documents/Program/QT/Workbench/Connections");
}