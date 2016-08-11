#include "mainwindow.h"
#include "connectwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    ConnectDatabase();
    
    QApplication a(argc, argv);
    
    MainWindow w;
    w.show();
    
//    connectwindow cw;
//    cw.show();

    return a.exec();
}
