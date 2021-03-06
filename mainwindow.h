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
#include "export_and_import.h"

#include "codeeditor.h"
#include "myhighlighter.h"
#include "typedef.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void schemaTreeInit();
	void manageTreeInit();

private:
	QMenuBar *menuBar1;
	//菜单栏
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *queryMenu;
	QMenu *databaseMenu;
	QMenu *serverMenu;
	QMenu *toolsMenu;
	QMenu *scriptingMenu;
	QMenu *helpMenu;
	//file菜单动作
	QAction *newModel;
	QAction *newQueryTab;
	QAction *openModel;
	QAction *saveModel;
	QAction *closeConnectionTab;
	QAction *closeTab;
	QAction *exit;
	//edit菜单动作
	QAction *undoSql;
	QAction *redoSql;
	QAction *cutSql;
	QAction *copySql;
	QAction *pasteSql;
	QAction *deleteSql;
	QAction *selectAll;
	QMenu *find;
	QAction *findOnly;
	QAction *findAndReplace;
	//view菜单动作
	QAction *home;
	QMenu *panels;
	QAction *hideSidebar;
	QAction *hideSecondarySidebar;
	QAction *hideOutputArea;
	QAction *output;
	QAction *selectNextMainTab;
	QAction *selectPreviousMainTab;
	QMenu *lineNumbers;
	QAction *hideLineNumbers;
	QAction *showLineNumbers;
	//query菜单动作
	QAction *executeAOS;//all or selection
	QAction *executeAOSText;
	QAction *executeCS; //current statement
	QAction *executeCSVTO; //vertical to output
	QAction *reconnectToServer;
	QAction *newTabToCurrentServer;
	//database菜单动作
	QAction *connectToDatabase;
	//server菜单动作
	QAction *serverStatus;
	QAction *clientConnection;
	QAction *usersAndPrivileges;
	QAction *statusAndSystemVariables;
	QAction *dataExport;
	QAction *dataImport;
	QAction *startupShutdown;
	QAction *serverLogs;
	//tool菜单动作
	//scripting菜单动作
	//help菜单动作
	QAction *locateLogFiles;
	QAction *showLogFile;
    /***********************************************/
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
    Export_and_Import *ExportImport;
    
    //void initConnection();
	void CreateMenu();
	void CreateAction();
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

	void initManageInfo(QTreeWidgetItem *temp_root, int temp);
	void initOneDBInfo(QTreeWidgetItem *temp_root, int temp);
};

#endif // MAINWINDOW_H
