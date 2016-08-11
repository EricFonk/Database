//
//  createConnectWindow.h
//  WorkBench
//
//  Created by Vaaaas on 16/8/10.
//
//

#ifndef createConnectWindow_h
#define createConnectWindow_h

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>

#include "connectwindow.h"

class createConnectWindow : public QDialog
{
    Q_OBJECT
    
public:
    createConnectWindow();
    
    QLabel *ConnNameLbl;
    QLineEdit *ConnNameBox;
    QLabel *HintLbl1;
    QLabel *HostNameLbl;
    QLineEdit *HostNameBox;
    QLabel *PortLbl;
    QLineEdit *PortBox;
    QLabel *UserNameLbl;
    QLineEdit *UserNameBox;
    QPushButton *TestBtn;
    QPushButton *SubmitBtn;
    QPushButton *CancelBtn;
    QLabel *PswdLbl;
    QLineEdit *PswdBox;
    
    QString ConnName;
    QString host;
    QString port;
    QString userName;
    QString pswd;
    
    ~createConnectWindow();
public slots:
    void setConnName(QString ConnName);
    void setHost(QString host);
    void setPort(QString port);
    void setUserName(QString userName);
    void setPswd(QString pswd);
    void TestConnection();
    void saveOptions();
    
private:
    void setupUI();
};

#endif /* createConnectWindow_h */