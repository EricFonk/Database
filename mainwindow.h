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
#include "panel_userprivileges.h"
#include "qFunctionTest.h"
#include "qProcedureTest.h"
#include "qViewTest.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void schemaTreeInit();
    void manageTreeInit();
    
private:
    QMenuBar *menuBar;
    
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    
    QTabWidget *mainTabWidg;
    QGridLayout *tabGridLayout;
    
    QTabWidget *leftTabWidg;
    QWidget *manageTab;
    QGridLayout *manageLayout;
    QTreeWidget *manageTree;
    QWidget *schemaTab;
    QGridLayout *schemaLayout;
    QTreeWidget *schemaTree;
    
    QWidget *rightWidget;
    QTabWidget *rightTabWidg;
    QVBoxLayout *rightLayout;
    
    QMenu *mainMenu;
    Panel_UserPrivileges *UserPanel;
    
    //void initConnection();
    
    void open();
    void initTables();
    void initViews();
    void initRoutines();
    
    SchemasTest *schemaTable;
    TableWidget *selectTable;
    TableCrt *tableCreate;
    TableAlt *tableAlter;
    QAction *addAction;
    QAction *openAction;
    QStringList allDB;
    bool hasCrtTable;
    private slots:
	void addTableToTree(QString newTableName);
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
    
    void initManageInfo(QTreeWidgetItem *temp_root,int temp);
    void initOneDBInfo(QTreeWidgetItem *temp_root,int temp);
};

#endif // MAINWINDOW_H
