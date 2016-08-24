#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    
    mainMenu = new QMenu(0);
    QPushButton *createDB = new QPushButton("create database");
	/****************创建菜单**************************/
	CreateAction();
	CreateMenu();
	/****************************************************************/
	centralwidget = new QWidget();
    centralwidget->setObjectName(QStringLiteral("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    
    mainTabWidg=new QTabWidget(centralwidget);
    mainTabWidg->setObjectName("mainTabWidg");
    QSizePolicy mainTabPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainTabPolicy.setHorizontalStretch(0);
    mainTabPolicy.setVerticalStretch(0);
    mainTabPolicy.setHeightForWidth(mainTabWidg->sizePolicy().hasHeightForWidth());
    mainTabWidg->setSizePolicy(mainTabPolicy);
    
    leftTabWidg=new QTabWidget(mainTabWidg);
    leftTabWidg->setObjectName(QStringLiteral("leftTabWidg"));
    leftTabWidg->setMaximumWidth(250);
    
    QWidget *mainTab=new QWidget();
    mainTab->setObjectName("mainTab");
    mainTabWidg->addTab(mainTab,QString("Main Tab"));
    QWidget *mainBlankTab=new QWidget();
    mainBlankTab->setObjectName("mainBlankTab");
    mainTabWidg->addTab(mainBlankTab, "Main Blank Tab");
    
    tabGridLayout=new QGridLayout(mainTab);
    tabGridLayout->setObjectName("tabGridLayout");
    
    QSizePolicy leftTabPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftTabPolicy.setHorizontalStretch(0);
    leftTabPolicy.setVerticalStretch(0);
    leftTabPolicy.setHeightForWidth(leftTabWidg->sizePolicy().hasHeightForWidth());
    leftTabWidg->setSizePolicy(leftTabPolicy);
    
    manageTab = new QWidget();
    manageTab->setObjectName(QStringLiteral("ManageTab"));
    QVBoxLayout *manageLayout=new QVBoxLayout(manageTab);
    manageTree=new QTreeWidget();
    manageTree->setObjectName(QStringLiteral("leftTreeWidg"));
    manageTree->setMaximumWidth(210);
    manageLayout->addWidget(manageTree);
    manageTab->setLayout(manageLayout);
    
    schemaTab = new QWidget();
    schemaTab->setObjectName(QStringLiteral("schemaTab"));
    QVBoxLayout *schemaLayout=new QVBoxLayout(schemaTab);
    schemaTree=new QTreeWidget();
    schemaTree->setObjectName(QStringLiteral("schemaTree"));
    schemaTree->setMaximumWidth(210);
    //设置菜单种类
    schemaTree->setContextMenuPolicy(Qt::CustomContextMenu);
    schemaLayout->addWidget(schemaTree);
    schemaTab->setLayout(schemaLayout);

    leftTabWidg->addTab(manageTab, QString("Manage"));
    leftTabWidg->addTab(schemaTab, QString("Schema"));
    
    rightTabWidg=new QTabWidget(mainTabWidg);
    rightTabWidg->setObjectName(QStringLiteral("rightTabWidg"));
    rightTabWidg->setMinimumSize(400, 450);
    
    QSizePolicy rightTabPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightTabPolicy.setHorizontalStretch(0);
    rightTabPolicy.setVerticalStretch(0);
    rightTabPolicy.setHeightForWidth(rightTabWidg->sizePolicy().hasHeightForWidth());
    rightTabWidg->setSizePolicy(rightTabPolicy);
    
    rightWidget=new QWidget();
    QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(rightWidget->sizePolicy().hasHeightForWidth());
    rightWidget->setSizePolicy(sizePolicy2);
    
    QTableView *dataView;
    dataView=new QTableView(mainTabWidg);
    dataView->setObjectName(QStringLiteral("dataView"));
    dataView->setMinimumHeight(300);
    
    rightLayout=new QVBoxLayout();
    rightLayout->addWidget(dataView);
    rightWidget->setLayout(rightLayout);
    
    QVBoxLayout *rightViceLayout=new QVBoxLayout();
    CodeEditor *configEditor = new CodeEditor();
    configEditor->setMode(EDIT);
    gridLayout->addWidget(configEditor);
    MyHighLighter *highlighter = new MyHighLighter(configEditor->document());
    rightViceLayout->addWidget(configEditor);
    
    
    QWidget *rightBlankTab=new QWidget();
    rightBlankTab->setObjectName("rightBlankTab");
    rightBlankTab->setLayout(rightViceLayout);
    
    rightTabWidg->addTab(rightWidget, "Right Tab");
    rightTabWidg->addTab(rightBlankTab, "Right Blank Tab");
    
    tabGridLayout->addWidget(leftTabWidg, 0, 0, 1, 1);
    tabGridLayout->addWidget(rightTabWidg, 0, 1, 1, 1);
    
	//gridLayout->addWidget(panelMenu,0,0);
    gridLayout->addWidget(mainTabWidg,1,0);
   

    this->setCentralWidget(centralwidget);
    
    //menuBar->setGeometry(0,0,this->width(),30);
    resize(950, 650);
    
    ManageTreeInit();
    SchemaTreeInit();

    connect(manageTree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(InitManageInfo(QTreeWidgetItem*,int)));
    connect(schemaTree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(InitOneDBInfo(QTreeWidgetItem*,int)));
    connect(schemaTree,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(ShowMenu(const QPoint&)));
    //connect(createDB,SIGNAL(clicked()),this,SLOT(CreateSchema()));

    QMetaObject::connectSlotsByName(this);
}

MainWindow::~MainWindow()
{

}

//建立菜单
void MainWindow::CreateMenu()
{
	fileMenu = menuBar()->addMenu(tr("File"));
	fileMenu->addAction(newQueryTab);
	fileMenu->addSeparator();
	fileMenu->addAction(newModel);
	fileMenu->addAction(openModel);
	fileMenu->addAction(saveModel);
	fileMenu->addSeparator();
	fileMenu->addAction(closeConnectionTab);
	fileMenu->addAction(closeTab);
	fileMenu->addSeparator();
	fileMenu->addAction(exit);
	/**************************************/
	editMenu = menuBar()->addMenu(tr("Edit"));
	editMenu->addAction(undoSql);
	editMenu->addAction(redoSql);
	editMenu->addSeparator();
	editMenu->addAction(cutSql);
	editMenu->addAction(copySql);
	editMenu->addAction(pasteSql);
	editMenu->addAction(deleteSql);
	editMenu->addSeparator();
	editMenu->addAction(selectAll);
	editMenu->addSeparator();
	find = editMenu->addMenu(tr("Find"));
	find->addAction(findOnly);
	find->addAction(findAndReplace);
	/******************************************/
	viewMenu = menuBar()->addMenu(tr("View"));
	viewMenu->addAction(home);
	viewMenu->addSeparator();
	panels = viewMenu->addMenu(tr("Panels"));
	panels->addAction(hideSidebar);
	panels->addAction(hideSecondarySidebar);
	panels->addAction(hideOutputArea);
	viewMenu->addAction(output);
	viewMenu->addSeparator();
	viewMenu->addAction(selectNextMainTab);
	viewMenu->addAction(selectPreviousMainTab);
	viewMenu->addSeparator();
	lineNumbers = viewMenu->addMenu(tr("Line Numbers"));
	lineNumbers->addAction(showLineNumbers);
	lineNumbers->addAction(hideLineNumbers);
	/**********************************************/
	queryMenu = menuBar()->addMenu(tr("Query"));
	queryMenu->addAction(executeAOS);
	queryMenu->addAction(executeAOSText);
	queryMenu->addSeparator();
	queryMenu->addAction(executeCS);
	queryMenu->addAction(executeCSVTO);
	queryMenu->addSeparator();
	queryMenu->addAction(reconnectToServer);
	queryMenu->addSeparator();
	queryMenu->addAction(newTabToCurrentServer);
	/*************************************************/
	databaseMenu = menuBar()->addMenu(tr("Database"));
	databaseMenu->addAction(connectToDatabase);
	/************************************************/
	serverMenu = menuBar()->addMenu(tr("Server"));
	serverMenu->addAction(serverStatus);
	serverMenu->addAction(clientConnection);
	serverMenu->addAction(usersAndPrivileges);
	serverMenu->addAction(statusAndSystemVariables);
	serverMenu->addSeparator();
	serverMenu->addAction(dataExport);
	serverMenu->addAction(dataImport);
	serverMenu->addSeparator();
	serverMenu->addAction(startupShutdown);
	serverMenu->addAction(serverLogs);
	/*****************************************************/
	//工具菜单
	toolsMenu = menuBar()->addMenu(tr("Tools"));
	//脚本处理菜单
	scriptingMenu = menuBar()->addMenu(tr("Scripting"));
	//帮助菜单
	helpMenu = menuBar()->addMenu(tr("Help"));
	helpMenu->addAction(locateLogFiles);
	helpMenu->addAction(showLogFile);
}
//槽函数

//建立菜单动作
void MainWindow::CreateAction()
{
	newModel = new QAction(tr("New Model"), this);
	newModel->setShortcut(Qt::CTRL | Qt::Key_N);
	newQueryTab = new QAction(tr("New Query Tab"), this);
	newQueryTab->setShortcut(Qt::CTRL|Qt::Key_T);
	openModel = new QAction(tr("Open Model"), this);
	openModel->setShortcut(Qt::CTRL|Qt::Key_O);
	saveModel = new QAction(tr("Save Model"), this);
	saveModel->setShortcut(Qt::CTRL|Qt::Key_S);
	closeConnectionTab = new QAction(tr("Close Connection Tab"), this);
	closeConnectionTab->setShortcut(Qt::CTRL | Qt::SHIFT|Qt::Key_F4);
	closeTab = new QAction(tr("Close Tab"), this);
	closeTab->setShortcut(Qt::CTRL | Qt::Key_W);
	exit = new QAction(tr("Exit"), this);
	exit->setShortcut(Qt::ALT | Qt::Key_F4);
	/****************************************/
	undoSql = new QAction(tr("Undo"), this);
	undoSql->setShortcut(Qt::CTRL | Qt::Key_Z);
	redoSql = new QAction(tr("Redo"), this);
	redoSql->setShortcut(Qt::CTRL | Qt::Key_Y);
	cutSql = new QAction(tr("Cut"), this);
	cutSql->setShortcut(Qt::CTRL | Qt::Key_X);
	copySql = new QAction(tr("Copy"), this);
	copySql->setShortcut(Qt::CTRL | Qt::Key_C);
	pasteSql = new QAction(tr("Paste"), this);
	pasteSql->setShortcut(Qt::CTRL | Qt::Key_V);
	deleteSql = new QAction(tr("Delete"), this);
	selectAll = new QAction(tr("Select All"), this);
	selectAll->setShortcut(Qt::CTRL | Qt::Key_A);
	findOnly = new QAction(tr("Find"), this);
	findOnly->setShortcut(Qt::CTRL | Qt::Key_F);
	findAndReplace = new QAction(tr("Find And Replace"), this);
	findAndReplace->setShortcut(Qt::CTRL | Qt::Key_H);
	/*************************************************/
	home = new QAction(tr("Home"), this);
	
	hideSidebar = new QAction(tr("Hide Sidebar"), this);
	hideSecondarySidebar = new QAction(tr("Hide Secondary Sidebar"), this);
	hideOutputArea = new QAction(tr("Hide Output Area"), this);
	output = new QAction(tr("Output"), this);
	output->setShortcut(Qt::CTRL | Qt::Key_F2);
	selectNextMainTab = new QAction(tr("Select Next Main Tab"), this);
	selectNextMainTab->setShortcut(Qt::ALT | Qt::SHIFT | Qt::Key_Right);
	selectPreviousMainTab = new QAction(tr("Select Previous Main Tab"), this);
	selectPreviousMainTab->setShortcut(Qt::ALT | Qt::SHIFT|Qt::Key_Left);
	hideLineNumbers = new QAction(tr("Hide Line Numbers"), this);
	showLineNumbers = new QAction(tr("Show Line Numbers"), this);
	/**********************************************************/
	executeAOS = new QAction(tr("Execute (All or Selection)"), this);
	executeAOS->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Enter);
	executeAOSText = new QAction(tr("Execute (All or Selection) to Text"), this);
	executeCS = new QAction(tr("Execute Current Statement"), this);
	executeCS->setShortcut(Qt::CTRL|Qt::Key_Enter);
	executeCSVTO = new QAction(tr("Execute Current Statement (Vertical Text Output)"), this);
	executeCSVTO->setShortcut(Qt::CTRL | Qt::ALT | Qt::Key_Enter);
	reconnectToServer = new QAction(tr("Reconnect To Server"), this);
	newTabToCurrentServer = new QAction(tr("New Tab To Current Server"), this);
	newTabToCurrentServer->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_T);
	/**********************************************************/
	connectToDatabase = new QAction(tr("Connect To Database"), this);
	connectToDatabase->setShortcut(Qt::CTRL|Qt::Key_U);
	/**********************************************************/
	serverStatus = new QAction(tr("Server Status"), this);
	clientConnection = new QAction(tr("Client Connection"), this);
	usersAndPrivileges = new QAction(tr("Users And Privileges"), this);
	statusAndSystemVariables = new QAction(tr("Status And System Variables"), this);
	dataExport = new QAction(tr("Data Export"), this);
	dataImport = new QAction(tr("Data Import"), this);
	startupShutdown = new QAction(tr("Startup/Shutdown"), this);
	serverLogs = new QAction(tr("Server Logs"), this);
	/*************************************************************/
	locateLogFiles = new QAction(tr("Locate Log Files"), this);
	showLogFile = new QAction(tr("Show Log File"), this);
}
//新建数据库
void MainWindow::CreateSchema()
{
    bool isOK;
    QString text = QInputDialog::getText(NULL, "CreateDatabase","insert a new name",
                                         QLineEdit::Normal, "", &isOK);
    if(isOK) {
        std::string res=CreateDBTest(text);
        if(res.compare("success")==0)
        {
            allDB<<text;
            QTreeWidgetItem *temp_root = new QTreeWidgetItem(schemaTree,QStringList(allDB[allDB.length()-1]));
        }
        else
        {
            QMessageBox::information(this, "ERROR!", QString::fromStdString(res));
        }
    }
}

//右键事件监听 右键菜单项设置
void MainWindow::ShowMenu(const QPoint&)
{
    //判断是否选中根节点 弹出数据库右键菜单
    if(schemaTree->currentItem()->parent()==NULL)
    {
        mainMenu->clear();
        QAction *deleteSchema = new QAction("delete this schema",this);
        mainMenu->addAction(deleteSchema);
        connect(deleteSchema,SIGNAL(triggered()),this,SLOT(DeleteOneSchema()));
        mainMenu->exec(QCursor::pos());
        
    }
    //弹出表右键菜单
    else if(schemaTree->currentItem()->text(0).compare("Tables")==0)
    {
        mainMenu->clear();
        QAction *createTable = new QAction("create new table",this);
        mainMenu->addAction(createTable);
        connect(createTable,SIGNAL(triggered()),this,SLOT(CreateOneTable()));
        mainMenu->exec(QCursor::pos());
    }
    //弹出函数右键菜单
    else if(schemaTree->currentItem()->text(0).compare("function")==0)
    {
        mainMenu->clear();
        QAction *createFunc = new QAction("create new function",this);
        mainMenu->addAction(createFunc);
        connect(createFunc,SIGNAL(triggered()),this,SLOT(CreateOneFunc()));
        mainMenu->exec(QCursor::pos());
    }
    //弹出过程右键函数
    else if(schemaTree->currentItem()->text(0).compare("procedure")==0)
    {
        mainMenu->clear();
        QAction *createPro = new QAction("create new procedure",this);
        mainMenu->addAction(createPro);
        connect(createPro,SIGNAL(triggered()),this,SLOT(CreateOnePro()));
        mainMenu->exec(QCursor::pos());
    }
    //弹出视图右键菜单
    else if(schemaTree->currentItem()->text(0).compare("Views")==0)
    {
        mainMenu->clear();
        QAction *createView = new QAction("create new view",this);
        mainMenu->addAction(createView);
        connect(createView,SIGNAL(triggered()),this,SLOT(CreateOneView()));
        mainMenu->exec(QCursor::pos());
    }
    //弹出单表右键菜单
    else if(schemaTree->currentItem()->parent()->text(0).compare("Tables")==0)
    {
        mainMenu->clear();
        QAction *tableAction1 = new QAction("select all",this);
        mainMenu->addAction(tableAction1);
        QAction *tableAction2 = new QAction("alter table",this);
        mainMenu->addAction(tableAction2);
        
        connect(tableAction1,SIGNAL(triggered()),this,SLOT(SelectAllofOneTable()));
        connect(tableAction2,SIGNAL(triggered()),this,SLOT(AlterOneTable()));
        mainMenu->exec(QCursor::pos());
        //connect(addAction,SIGNAL(triggered()),this,SLOT(testadd()));
    }
    //弹出单函数右键菜单
    else if(schemaTree->currentItem()->parent()->text(0).compare("function")==0)
    {
        mainMenu->clear();
        QAction *funcAction1 = new QAction("alter function",this);
        mainMenu->addAction(funcAction1);
        connect(funcAction1,SIGNAL(triggered()),this,SLOT(InitOneFunction()));
        mainMenu->exec(QCursor::pos());
    }
    //弹出单过程右键菜单
    else if(schemaTree->currentItem()->parent()->text(0).compare("procedure")==0)
    {
        mainMenu->clear();
        QAction *proAction1 = new QAction("alter procedure",this);
        mainMenu->addAction(proAction1);
        connect(proAction1,SIGNAL(triggered()),this,SLOT(InitOneProcedure()));
        mainMenu->exec(QCursor::pos());
    }
    //弹出单视图右键菜单
    else if(schemaTree->currentItem()->parent()->text(0).compare("Views")==0)
    {
        mainMenu->clear();
        QAction *viewAction1 = new QAction("alter view",this);
        mainMenu->addAction(viewAction1);
        connect(viewAction1,SIGNAL(triggered()),this,SLOT(InitOneView()));
        mainMenu->exec(QCursor::pos());
    }
}

void MainWindow::SelectAllofOneTable()
{
    QStringList real_head,real_data,schemas,tables,req_allDB;
    int row,col,mainPos;
    QString mainKey;
    string resflag;
    bool flag1,flag2;
    
    resflag = ExecuteWithQuery(1,"use "+schemaTree->currentItem()->parent()->parent()->text(0)+";");
    if(resflag.compare("success")==0)
    {
        flag1 = GetMainKey(mainKey,schemaTree->currentItem()->text(0));
        flag2 = SelectTest(schemaTree->currentItem()->text(0),real_head,real_data,row,col,mainKey,mainPos);
        
        if(flag1&&flag2){
            QWidget *temp = rightLayout->itemAt(0)->widget();
            rightLayout->removeWidget(temp);
            delete(temp);
            selectTable = new TableWidget();
            //st = new SchemasTest();
            rightLayout->addWidget(selectTable);
            selectTable->SetHeaders(schemaTree->currentItem()->text(0),col,real_head,mainKey,mainPos);
            selectTable->InitTableData(real_data,row,col);
        }
    }
    
}

void MainWindow::CreateOneTable()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    tableCreate = new TableCrt();
    rightLayout->addWidget(tableCreate);
    hasCrtTable = true;
    tableCreate->SetDBName(schemaTree->currentItem()->parent()->text(0));
    
	connect(tableCreate,SIGNAL(CreateTableSuccess(QString)),this,SLOT(AddTableToTree(QString)));
}

void MainWindow::AddTableToTree(QString newTableName)
{
	QTreeWidgetItem *table_leaf = new QTreeWidgetItem(schemaTree->currentItem(),QStringList(newTableName));
	schemaTree->currentItem()->addChild(table_leaf);
}

void MainWindow::AlterOneTable()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    tableAlter = new TableAlt();
    rightLayout->addWidget(tableAlter);
    hasCrtTable = true;
    tableAlter->SetDTName(schemaTree->currentItem()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
    tableAlter->LoadTableInfo();
}

void MainWindow::CreateOneFunc()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    QFunctionTest *funcTable = new QFunctionTest();
    rightLayout->addWidget(funcTable);
    funcTable->SetAllName(schemaTree->currentItem()->parent()->parent()->text(0),"xx");
}

void MainWindow::InitOneFunction()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    QFunctionTest *funcTable = new QFunctionTest();
    rightLayout->addWidget(funcTable);
    QString res;
    res = GetFunction(schemaTree->currentItem()->parent()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
    funcTable->SetText(res);
    funcTable->SetAllName(schemaTree->currentItem()->parent()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
}

void MainWindow::CreateOnePro()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    QProcedureTest *proTable = new QProcedureTest();
    rightLayout->addWidget(proTable);
    proTable->SetAllName(schemaTree->currentItem()->parent()->parent()->text(0),"");
}

void MainWindow::InitOneProcedure()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    QProcedureTest *proTable = new QProcedureTest();
    rightLayout->addWidget(proTable);
    QString res;
    res = GetProcedure(schemaTree->currentItem()->parent()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
    proTable->SetText(res);
    proTable->SetAllName(schemaTree->currentItem()->parent()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
}

void MainWindow::CreateOneView()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    QViewTest *viewTable = new QViewTest();
    rightLayout->addWidget(viewTable);
    viewTable->SetAllName(schemaTree->currentItem()->parent()->text(0),"");
}

void MainWindow::InitOneView()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    QViewTest *viewTable = new QViewTest();
    rightLayout->addWidget(viewTable);
    QString res;
    res = GetView(schemaTree->currentItem()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
    viewTable->SetText(res);
    viewTable->SetAllName(schemaTree->currentItem()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
}

void MainWindow::DeleteOneSchema()
{
    QString drop = "are you sure to drop database "+schemaTree->currentItem()->text(0)+"  ?";
    QMessageBox::StandardButton rb = QMessageBox::question(NULL,"Drop",drop,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        string res = DropDBTest(schemaTree->currentItem()->text(0));
        if(res.compare("success")==0)
        {
            delete(schemaTree->currentItem());
        }
        else
        {
            QMessageBox::information(this, "ERROR!", QString::fromStdString(res));
        }
    }
}
//命名规范
void MainWindow::SchemaTreeInit()
{
    QStringList initAllDB;
    string res;
    res = GetAllDatabases(initAllDB);
    allDB = initAllDB;
    for(int i=0;i<initAllDB.length();i++)
    {
        QTreeWidgetItem *temp_root = new QTreeWidgetItem(schemaTree,QStringList(allDB[i]));
    }
}

void MainWindow::ManageTreeInit()
{
    QTreeWidgetItem *temp_root = new QTreeWidgetItem(manageTree,QStringList("Variables"));
    QTreeWidgetItem *temp_root2 = new QTreeWidgetItem(manageTree,QStringList("Connections"));
    QTreeWidgetItem *temp_root3 = new QTreeWidgetItem(manageTree,QStringList("Users and Privileges"));
}

void MainWindow::InitManageInfo(QTreeWidgetItem *temp_root, int temp)
{
    QTableView *dataView;
    dataView=new QTableView(mainTabWidg);
    dataView->setObjectName(QStringLiteral("dataView"));
    dataView->setMinimumHeight(300);
    //QMessageBox::information(this, "ERROR!", QString(item->text(0)));
    //item->columnCount();
    if(manageTree->currentItem()->text(0).compare("Connections")==0)
    {
        QWidget *temp = rightLayout->itemAt(0)->widget();
        rightLayout->removeWidget(temp);
        delete(temp);
        
        QStandardItemModel *model = GetConnections(model);
        
        rightLayout->addWidget(dataView);
        dataView->setModel(model);
        dataView->update();
    }else if(manageTree->currentItem()->text(0).compare("Variables")==0)
    {
        QWidget *temp = rightLayout->itemAt(0)->widget();
        rightLayout->removeWidget(temp);
        delete(temp);

        QStandardItemModel *model = new QStandardItemModel(13, 1);
        model->setHeaderData(0, Qt::Horizontal, QVariant("Value"));
        std::string str[13] = { "hostname", "socket", "port","version","version_comment","version_compile_os","version_compile_machine","basedir","datadir","plugin_dir","tmpdir","log_error","slow_query_log_file" };
        
        for (int i = 0; i <= 12; i++)
        {
            model->setHeaderData(i, Qt::Vertical, QVariant(str[i].c_str()));
            QModelIndex index = model->index(i, 0, QModelIndex());
            model->setData(index, QVariant(GetOneVariable(str[i])));
        }
        
        rightLayout->addWidget(dataView);
        dataView->setModel(model);
        //dataView->update();
    }else if(manageTree->currentItem()->text(0).compare("Users and Privileges")==0){
        QWidget *temp = rightLayout->itemAt(0)->widget();
        rightLayout->removeWidget(temp);
        delete(temp);
        
        UserPanel=new Panel_UserPrivileges();
        rightLayout->addWidget(UserPanel);
        
    }
    //QMessageBox::information(this, "ERROR!", mysql_error(&mysqlObj.mysql));
}


void MainWindow::InitOneDBInfo(QTreeWidgetItem *temp_root, int temp)
{
    if(schemaTree->currentItem()->parent()==NULL&&schemaTree->currentItem()->childCount()==0)
    {
        QString temp2 = temp_root->text(0);
        //qDebug("%s",temp2);
        InitTables();
        InitViews();
        InitRoutines();
    }
}

void MainWindow::InitTables()
{
    QTreeWidgetItem *root = new QTreeWidgetItem(schemaTree->currentItem(),QStringList(QString("Tables")));
    QString dbName = schemaTree->currentItem()->text(0);
    string res;
    QStringList table_list;
    res = GetAllTables(table_list,dbName);
    if(res.compare("success")==0&&table_list.length()!=0)
    {
		for(int t=0;t<table_list.length();t++)
		{
	        QTreeWidgetItem *table_leaf = new QTreeWidgetItem(root,QStringList(table_list[t]));
	        schemaTree->currentItem()->addChild(table_leaf);
		}
    }
}

void MainWindow::InitViews()
{
    QTreeWidgetItem *root = new QTreeWidgetItem(schemaTree->currentItem(),QStringList(QString("Views")));
    QString dbName = schemaTree->currentItem()->text(0);
    string res;
    QStringList view_list;
    res = FindViews(dbName,view_list);
    if(res.compare("success")==0&&view_list.length()!=0)
    {
        QTreeWidgetItem *view_leaf = new QTreeWidgetItem(root,view_list);
        schemaTree->currentItem()->addChild(view_leaf);
        //qDebug("%s",view_list[0]);
    }
    //qDebug("%s",schemaTree->currentItem()->text(0));
    qDebug("ok");
}

void MainWindow::InitRoutines()
{
    QTreeWidgetItem *root = new QTreeWidgetItem(schemaTree->currentItem(),QStringList(QString("Routines")));
    QTreeWidgetItem *leaf1 = new QTreeWidgetItem(root,QStringList(QString("procedure")));
    QTreeWidgetItem *leaf2 = new QTreeWidgetItem(root,QStringList(QString("function")));
    QString dbName = schemaTree->currentItem()->text(0);
    string res;
    QStringList pro_list,func_list;
    res = FindPros(dbName,pro_list);
    if(res.compare("success")==0&&pro_list.length()!=0)
    {
        QTreeWidgetItem *pro_leaf = new QTreeWidgetItem(leaf1,pro_list);
        schemaTree->currentItem()->addChild(pro_leaf);
    }
    res = FindFuncs(dbName,func_list);
    if(res.compare("success")==0&&func_list.length()!=0)
    {
        QTreeWidgetItem *func_leaf = new QTreeWidgetItem(leaf2,func_list);
        schemaTree->currentItem()->addChild(func_leaf);
    }
    //qDebug("%s",schemaTree->currentItem()->text(0));
    qDebug("ok");
}
