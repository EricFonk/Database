#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTableView>
#include <QMenuBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    
    QTabWidget *leftTabWidg;
    QWidget *manageTab;
    QGridLayout *manageLayout;
    QTreeWidget *manageTree;
    QWidget *schemaTab;
    QGridLayout *schemaLayout;
    QTreeWidget *schemaTree;
    
    QWidget *rightWidget;
    QVBoxLayout *rightLayout;
    
    QTableView *dataView;
    
};

#endif // MAINWINDOW_H
