//
//  createConnectWindow.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/10.
//
//

#include <QMessageBox>

#include "createConnectWindow.h"

createConnectWindow::createConnectWindow()
{
    setupUI();
}

createConnectWindow::~createConnectWindow()
{
    
}

void createConnectWindow::setupUI()
{
    this->resize(787, 421);
    ConnNameLbl = new QLabel("连接名称",this);
    ConnNameLbl->setObjectName(QStringLiteral("ConnNameLbl"));
    ConnNameLbl->setGeometry(QRect(50, 80, 141, 41));
    QFont font;
    font.setPointSize(12);
    ConnNameLbl->setFont(font);
    ConnNameBox = new QLineEdit(this);
    ConnNameBox->setObjectName(QStringLiteral("ConnNameBox"));
    ConnNameBox->setGeometry(QRect(220, 90, 221, 20));
    HintLbl1 = new QLabel(this);
    HintLbl1->setObjectName(QStringLiteral("HintLbl1"));
    HintLbl1->setGeometry(QRect(480, 90, 251, 21));
    QFont font1;
    font1.setPointSize(11);
    HintLbl1->setFont(font1);
    HostNameLbl = new QLabel("主机名",this);
    HostNameLbl->setObjectName(QStringLiteral("HostNameLbl"));
    HostNameLbl->setGeometry(QRect(110, 190, 81, 41));
    HostNameLbl->setFont(font);
    HostNameBox = new QLineEdit(this);
    HostNameBox->setObjectName(QStringLiteral("HostNameBox"));
    HostNameBox->setGeometry(QRect(210, 200, 131, 20));
    PortLbl = new QLabel("端口",this);
    PortLbl->setObjectName(QStringLiteral("PortLbl"));
    PortLbl->setGeometry(QRect(370, 190, 71, 41));
    PortLbl->setFont(font);
    PortBox = new QLineEdit(this);
    PortBox->setObjectName(QStringLiteral("PortBox"));
    PortBox->setGeometry(QRect(470, 200, 131, 20));
    UserNameLbl = new QLabel("用户名",this);
    UserNameLbl->setObjectName(QStringLiteral("UserNameLbl"));
    UserNameLbl->setGeometry(QRect(110, 250, 91, 41));
    UserNameLbl->setFont(font);
    UserNameBox = new QLineEdit(this);
    UserNameBox->setObjectName(QStringLiteral("UserNameBox"));
    UserNameBox->setGeometry(QRect(210, 260, 131, 20));
    TestBtn = new QPushButton("测试连接",this);
    TestBtn->setObjectName(QStringLiteral("TestBtn"));
    TestBtn->setGeometry(QRect(480, 380, 121, 23));
    SubmitBtn = new QPushButton("确认",this);
    SubmitBtn->setObjectName(QStringLiteral("SubmitBtn"));
    SubmitBtn->setGeometry(QRect(700, 380, 41, 23));
    CancelBtn = new QPushButton("取消",this);
    CancelBtn->setObjectName(QStringLiteral("CancelBtn"));
    CancelBtn->setGeometry(QRect(620, 380, 61, 23));
    PswdLbl = new QLabel("密码",this);
    PswdLbl->setObjectName(QStringLiteral("PswdLbl"));
    PswdLbl->setGeometry(QRect(370, 250, 91, 41));
    PswdLbl->setFont(font);
    PswdBox = new QLineEdit(this);
    PswdBox->setObjectName(QStringLiteral("PswdBox"));
    PswdBox->setGeometry(QRect(470, 260, 131, 20));
    PswdBox->setEchoMode(QLineEdit::Password);
    
    HostNameBox->setText("localhost");
    PortBox->setText("3306");
    UserNameBox->setText("root");
    PswdBox->setText("19960612");
    ConnName = ConnNameBox->text();
    host = HostNameBox->text();
    port = PortBox->text();
    userName = UserNameBox->text();
    pswd = PswdBox->text();
    
    QObject::connect(CancelBtn, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(TestBtn, SIGNAL(clicked()), this, SLOT(TestConnection()));
    QObject::connect(HostNameBox, SIGNAL(textChanged(QString)), this, SLOT(setHost(QString)));
    QObject::connect(PortBox, SIGNAL(textChanged(QString)), this, SLOT(setPort(QString)));
    QObject::connect(UserNameBox, SIGNAL(textChanged(QString)), this, SLOT(setUserName(QString)));
    QObject::connect(PswdBox, SIGNAL(textChanged(QString)), this, SLOT(setPswd(QString)));
    QObject::connect(SubmitBtn, SIGNAL(clicked()), this, SLOT(saveOptions()));
    QObject::connect(ConnNameBox, SIGNAL(textChanged(QString)), this, SLOT(setConnName(QString)));
}

void createConnectWindow::setConnName(QString ConnName)
{
    createConnectWindow::ConnName = ConnName;
}

void createConnectWindow::setHost(QString host)
{
    createConnectWindow::host = host;
}

void createConnectWindow::setPort(QString port)
{
    createConnectWindow::port = port;
}

void createConnectWindow::setUserName(QString userName)
{
    createConnectWindow::userName = userName;
}

void createConnectWindow::setPswd(QString pswd)
{
    createConnectWindow::pswd = pswd;
}

void createConnectWindow::TestConnection()
{
    
    std::string nowHost = host.toStdString();
    unsigned int nowPort = port.toInt();
    std::string nowUserName = userName.toStdString();
    std::string nowPswd = pswd.toStdString();
    cout<<"\n";
    cout<<nowHost;
    cout<<nowUserName;
    cout<<nowPswd;
    cout<<"workbenchdb";
    
    MYSQL mysql;
    
    if (mysql_init(&mysql) != NULL)
    {
        if (mysql_real_connect(&mysql, nowHost.c_str(), nowUserName.c_str(), nowPswd.c_str(), "workbenchdb", 0, NULL, 0) != NULL)
        {
            QMessageBox::information(NULL, "Connect Test", "Connect Success");
            return;
        }
    }
    QMessageBox::information(NULL, "Connect Test", "Connect Failed");
}
void createConnectWindow::saveOptions()
{
    std::string nowConnName = ConnName.toStdString();
    std::string nowHost = host.toStdString();
    std::string nowPort = port.toStdString();
    std::string nowUserName = userName.toStdString();
    std::string nowPswd = pswd.toStdString();
    
    QStringList conns = connectwindow::getConnections();
    for (int i = 0; i<conns.length() - 1; i++)
    {
        QString str = ConnName;
        if (conns[i].split('|')[0] == str)
        {
            QMessageBox::information(NULL, "Save Connection", "Connection Name is not valid");
            return;
        }
    }
    
    //!!!!!!!!
    //ofstream saveFile("/Users/vaaaas/Documents/Program/QT/Workbench/Connections");
    ofstream saveFile;
    saveFile.open("/Users/vaaaas/Documents/Program/QT/Workbench/Connections",ios::out|ios::ate|ios::app);
    if(saveFile.is_open())
    {
        saveFile<<nowConnName;
        saveFile<<"|";
        saveFile<<nowHost;
        saveFile<<"|";
        saveFile<<nowPort;
        saveFile<<"|";
        saveFile<<nowUserName;
        saveFile<<"|";
        saveFile<<nowPswd;
        saveFile<<"\n";
        saveFile.close();
    }else{
        cout << "Error opening file";
    }
    
    this->hide();
}