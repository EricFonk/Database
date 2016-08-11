//
//  connectwindow.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/10.
//
//

#ifndef connectwindow_h
#define connectwindow_h

#include <QWidget>
#include <QGridLayout>
#include <QTableView>
#include <QPushButton>

#include "createConnectWindow.h"

class connectwindow : public QWidget
{
    Q_OBJECT
    
public:
    connectwindow();
    ~connectwindow();
    
    QVBoxLayout *gridLayout;
    QTableView *dataView;
    QWidget* bottomWidget;
    QHBoxLayout* bottomLayout;
    QPushButton* newBtn;
    QPushButton* deleteBtn;
    
    QModelIndex* selected;
    //createConnectWindow createConD;
    
    static QStringList getConnections();
    
public slots:
    void Login();
    void Refresh(QModelIndex index);
    void openNewConn();
    void RemoveOneConn();
    
private:
    
    
    void setupUI();
};

#endif /* connectwindow_h */
