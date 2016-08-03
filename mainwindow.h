#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTableView>
#include <QMenuBar>

#include "mysqlQuery.h"
#include "tablewidget.h"
#include "table_create.h"
#include "table_alter.h"
#include "schemas_test.h"
#include "qFunctionTest.h"
#include "qProcedureTest.h"
#include "qViewTest.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void treeInit();
    
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
    
    //未处理
    QMenu *mainMenu;
//    SchemasTest *schemaTable;
//    TableWidget *selectTable;
//    TableCrt *tableCreate;
//    TableAlt *tableAlter;
    QAction *addAction;
    QAction *openAction;
    QStringList allDB;
    bool hasCrtTable;
    
    void open();
    void initTables();
    void initViews();
    void initRoutines();
    
private slots:
    void createSchema();
    void showMenu(const QPoint&);
    void deleteOneSchema();
    void createOneTable();
    void createOneView();
    void createOneFunc();
    void createOnePro();
    void selectAllofOneTable();
    void alterOneTable();
    void initOneFunction();
    void initOneProcedure();
    void initOneView();
    void initOneDBInfo(QTreeWidgetItem *temp_root,int temp);
    
};

#endif // MAINWINDOW_H
