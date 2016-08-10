#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    
    mainMenu = new QMenu(this);
    QPushButton *createDB = new QPushButton("create database");
    
    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QStringLiteral("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    
    leftTabWidg=new QTabWidget(centralwidget);
    leftTabWidg->setObjectName(QStringLiteral("leftTabWidg"));
    leftTabWidg->setMinimumSize(400, 450);
    leftTabWidg->setTabEnabled(0, true);
    leftTabWidg->setTabEnabled(1, true);
    leftTabWidg->setTabBarAutoHide(false);
    
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(leftTabWidg->sizePolicy().hasHeightForWidth());
    leftTabWidg->setSizePolicy(sizePolicy1);
    
    manageTab = new QWidget();
    manageTab->setObjectName(QStringLiteral("ManageTab"));
    QVBoxLayout *manageLayout=new QVBoxLayout(manageTab);
    manageTree=new QTreeWidget();
    manageTree->setObjectName(QStringLiteral("leftTreeWidg"));
    manageTree->setMinimumSize(300, 400);
    manageLayout->addWidget(manageTree);
    manageTab->setLayout(manageLayout);
    
    schemaTab = new QWidget(centralwidget);
    schemaTab->setObjectName(QStringLiteral("schemaTab"));
    QVBoxLayout *schemaLayout=new QVBoxLayout(schemaTab);
    schemaTree=new QTreeWidget();
    schemaTree->setObjectName(QStringLiteral("schemaTree"));
    schemaTree->setMinimumSize(300, 400);
    schemaLayout->addWidget(schemaTree);
    schemaTab->setLayout(schemaLayout);

    leftTabWidg->addTab(manageTab, QString("Manage"));
    leftTabWidg->addTab(schemaTab, QString("Schema"));
    
    gridLayout->addWidget(leftTabWidg, 0, 0, 1, 1);
    
    rightWidget=new QWidget();
    QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(rightWidget->sizePolicy().hasHeightForWidth());
    rightWidget->setSizePolicy(sizePolicy2);
    
    QTableView *dataView;
    dataView=new QTableView(centralwidget);
    dataView->setObjectName(QStringLiteral("dataView"));
    dataView->setMinimumHeight(300);
    
    rightLayout=new QVBoxLayout();
    rightLayout->addWidget(dataView);
    //rightLayout->addWidget(createDB);
    rightWidget->setLayout(rightLayout);
    
    gridLayout->addWidget(rightWidget, 0, 1, 1, 1);
    
    this->setCentralWidget(centralwidget);
    resize(850, 450);
    
    manageTreeInit();
    schemaTreeInit();
    //设置菜单种类
    //manageTree->setContextMenuPolicy(Qt::CustomContextMenu);
    schemaTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(manageTree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(initManageInfo(QTreeWidgetItem*,int)));
    connect(schemaTree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(initOneDBInfo(QTreeWidgetItem*,int)));
    connect(schemaTree,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showMenu(const QPoint&)));
    //connect(createDB,SIGNAL(clicked()),this,SLOT(createSchema()));
}

MainWindow::~MainWindow()
{

}

//新建数据库
void MainWindow::createSchema()
{
    bool isOK;
    QString text = QInputDialog::getText(NULL, "CreateDatabase","insert a new name",
                                         QLineEdit::Normal, "", &isOK);
    if(isOK) {
        std::string res=createDBTest(text);
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
void MainWindow::showMenu(const QPoint&)
{
    //判断是否选中根节点 弹出数据库右键菜单
    if(schemaTree->currentItem()->parent()==NULL)
    {
        mainMenu->clear();
        QAction *deleteSchema = new QAction("delete this schema",this);
        mainMenu->addAction(deleteSchema);
        connect(deleteSchema,SIGNAL(triggered()),this,SLOT(deleteOneSchema()));
        mainMenu->exec(QCursor::pos());
        
    }
    //弹出表右键菜单
    else if(schemaTree->currentItem()->text(0).compare("Tables")==0)
    {
        mainMenu->clear();
        QAction *createTable = new QAction("create new table",this);
        mainMenu->addAction(createTable);
        connect(createTable,SIGNAL(triggered()),this,SLOT(createOneTable()));
        mainMenu->exec(QCursor::pos());
    }
    //弹出函数右键菜单
    else if(schemaTree->currentItem()->text(0).compare("function")==0)
    {
        mainMenu->clear();
        QAction *createFunc = new QAction("create new function",this);
        mainMenu->addAction(createFunc);
        connect(createFunc,SIGNAL(triggered()),this,SLOT(createOneFunc()));
        mainMenu->exec(QCursor::pos());
    }
    //弹出过程右键函数
    else if(schemaTree->currentItem()->text(0).compare("procedure")==0)
    {
        mainMenu->clear();
        QAction *createPro = new QAction("create new procedure",this);
        mainMenu->addAction(createPro);
        connect(createPro,SIGNAL(triggered()),this,SLOT(createOnePro()));
        mainMenu->exec(QCursor::pos());
    }
    //弹出视图右键菜单
    else if(schemaTree->currentItem()->text(0).compare("Views")==0)
    {
        mainMenu->clear();
        QAction *createView = new QAction("create new view",this);
        mainMenu->addAction(createView);
        connect(createView,SIGNAL(triggered()),this,SLOT(createOneView()));
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
        
        connect(tableAction1,SIGNAL(triggered()),this,SLOT(selectAllofOneTable()));
        connect(tableAction2,SIGNAL(triggered()),this,SLOT(alterOneTable()));
        mainMenu->exec(QCursor::pos());
        //connect(addAction,SIGNAL(triggered()),this,SLOT(testadd()));
    }
    //弹出单函数右键菜单
    else if(schemaTree->currentItem()->parent()->text(0).compare("function")==0)
    {
        mainMenu->clear();
        QAction *funcAction1 = new QAction("alter function",this);
        mainMenu->addAction(funcAction1);
        connect(funcAction1,SIGNAL(triggered()),this,SLOT(initOneFunction()));
        mainMenu->exec(QCursor::pos());
    }
    //弹出单过程右键菜单
    else if(schemaTree->currentItem()->parent()->text(0).compare("procedure")==0)
    {
        mainMenu->clear();
        QAction *proAction1 = new QAction("alter procedure",this);
        mainMenu->addAction(proAction1);
        connect(proAction1,SIGNAL(triggered()),this,SLOT(initOneProcedure()));
        mainMenu->exec(QCursor::pos());
    }
    //弹出单视图右键菜单
    else if(schemaTree->currentItem()->parent()->text(0).compare("Views")==0)
    {
        mainMenu->clear();
        QAction *viewAction1 = new QAction("alter view",this);
        mainMenu->addAction(viewAction1);
        connect(viewAction1,SIGNAL(triggered()),this,SLOT(initOneView()));
        mainMenu->exec(QCursor::pos());
    }
}

void MainWindow::selectAllofOneTable()
{
    QStringList real_head,real_data,schemas,tables,req_allDB;
    int row,col,mainPos;
    QString mainKey;
    string resflag;
    bool flag1,flag2;
    
    resflag = executeWithQuery(1,"use "+schemaTree->currentItem()->parent()->parent()->text(0)+";");
    if(resflag.compare("success")==0)
    {
        flag1 = getMainKey(mainKey,schemaTree->currentItem()->text(0));
        flag2 = selectTest(schemaTree->currentItem()->text(0),real_head,real_data,row,col,mainKey,mainPos);
        
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

void MainWindow::createOneTable()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    tableCreate = new TableCrt();
    rightLayout->addWidget(tableCreate);
    hasCrtTable = true;
    tableCreate->setDBName(schemaTree->currentItem()->parent()->text(0));
    
}

void MainWindow::alterOneTable()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    tableAlter = new TableAlt();
    rightLayout->addWidget(tableAlter);
    hasCrtTable = true;
    tableAlter->setDTName(schemaTree->currentItem()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
    tableAlter->loadTableInfo();
}

void MainWindow::createOneFunc()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    qFunctionTest *funcTable = new qFunctionTest();
    rightLayout->addWidget(funcTable);
    funcTable->setAllName(schemaTree->currentItem()->parent()->parent()->text(0),"xx");
}

void MainWindow::initOneFunction()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    qFunctionTest *funcTable = new qFunctionTest();
    rightLayout->addWidget(funcTable);
    QString res;
    res = get_function(schemaTree->currentItem()->parent()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
    funcTable->setText(res);
    funcTable->setAllName(schemaTree->currentItem()->parent()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
}

void MainWindow::createOnePro()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    qProcedureTest *proTable = new qProcedureTest();
    rightLayout->addWidget(proTable);
    proTable->setAllName(schemaTree->currentItem()->parent()->parent()->text(0),"");
}

void MainWindow::initOneProcedure()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    qProcedureTest *proTable = new qProcedureTest();
    rightLayout->addWidget(proTable);
    QString res;
    res = get_procedure(schemaTree->currentItem()->parent()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
    proTable->setText(res);
    proTable->setAllName(schemaTree->currentItem()->parent()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
}

void MainWindow::createOneView()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    qViewTest *viewTable = new qViewTest();
    rightLayout->addWidget(viewTable);
    viewTable->setAllName(schemaTree->currentItem()->parent()->text(0),"");
}

void MainWindow::initOneView()
{
    QWidget *mytemp = rightLayout->itemAt(0)->widget();
    rightLayout->removeWidget(mytemp);
    delete(mytemp);
    
    qViewTest *viewTable = new qViewTest();
    rightLayout->addWidget(viewTable);
    QString res;
    res = get_view(schemaTree->currentItem()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
    viewTable->setText(res);
    viewTable->setAllName(schemaTree->currentItem()->parent()->parent()->text(0),schemaTree->currentItem()->text(0));
}

void MainWindow::deleteOneSchema()
{
    QString drop = "are you sure to drop database "+schemaTree->currentItem()->text(0)+"  ?";
    QMessageBox::StandardButton rb = QMessageBox::question(NULL,"Drop",drop,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        string res = dropDBTest(schemaTree->currentItem()->text(0));
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
void MainWindow::schemaTreeInit()
{
    QStringList initAllDB;
    string res;
    res = getAllDatabases(initAllDB);
    allDB = initAllDB;
    for(int i=0;i<initAllDB.length();i++)
    {
        QTreeWidgetItem *temp_root = new QTreeWidgetItem(schemaTree,QStringList(allDB[i]));
    }
}

void MainWindow::manageTreeInit()
{
    QTreeWidgetItem *temp_root = new QTreeWidgetItem(manageTree,QStringList("Variables"));
    QTreeWidgetItem *temp_root2 = new QTreeWidgetItem(manageTree,QStringList("Connections"));
}

void MainWindow::initManageInfo(QTreeWidgetItem *temp_root, int temp)
{
    QTableView *dataView;
    dataView=new QTableView(centralwidget);
    dataView->setObjectName(QStringLiteral("dataView"));
    dataView->setMinimumHeight(300);
    //QMessageBox::information(this, "ERROR!", QString(item->text(0)));
    //item->columnCount();
    if(manageTree->currentItem()->text(0).compare("Connections")==0)
    {
        QWidget *temp = rightLayout->itemAt(0)->widget();
        rightLayout->removeWidget(temp);
        delete(temp);
        
        QStandardItemModel *model = getConnections(model);
        
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
            model->setData(index, QVariant(getOneVariable(str[i])));
        }
        
        rightLayout->addWidget(dataView);
        dataView->setModel(model);
        dataView->update();
    }
    //QMessageBox::information(this, "ERROR!", mysql_error(&mysqlObj.mysql));
}


void MainWindow::initOneDBInfo(QTreeWidgetItem *temp_root, int temp)
{
    if(schemaTree->currentItem()->parent()==NULL&&schemaTree->currentItem()->childCount()==0)
    {
        QString temp2 = temp_root->text(0);
        //qDebug("%s",temp2);
        initTables();
        initViews();
        initRoutines();
    }
}

void MainWindow::initTables()
{
    QTreeWidgetItem *root = new QTreeWidgetItem(schemaTree->currentItem(),QStringList(QString("Tables")));
    QString dbName = schemaTree->currentItem()->text(0);
    string res;
    QStringList table_list;
    res = getAllTables(table_list,dbName);
    if(res.compare("success")==0&&table_list.length()!=0)
    {
        QTreeWidgetItem *table_leaf = new QTreeWidgetItem(root,table_list);
        schemaTree->currentItem()->addChild(table_leaf);
    }
}

void MainWindow::initViews()
{
    QTreeWidgetItem *root = new QTreeWidgetItem(schemaTree->currentItem(),QStringList(QString("Views")));
    QString dbName = schemaTree->currentItem()->text(0);
    string res;
    QStringList view_list;
    res = find_views(dbName,view_list);
    if(res.compare("success")==0&&view_list.length()!=0)
    {
        QTreeWidgetItem *view_leaf = new QTreeWidgetItem(root,view_list);
        schemaTree->currentItem()->addChild(view_leaf);
        //qDebug("%s",view_list[0]);
    }
    //qDebug("%s",schemaTree->currentItem()->text(0));
    qDebug("ok");
}

void MainWindow::initRoutines()
{
    QTreeWidgetItem *root = new QTreeWidgetItem(schemaTree->currentItem(),QStringList(QString("Routines")));
    QTreeWidgetItem *leaf1 = new QTreeWidgetItem(root,QStringList(QString("procedure")));
    QTreeWidgetItem *leaf2 = new QTreeWidgetItem(root,QStringList(QString("function")));
    QString dbName = schemaTree->currentItem()->text(0);
    string res;
    QStringList pro_list,func_list;
    res = find_pros(dbName,pro_list);
    if(res.compare("success")==0&&pro_list.length()!=0)
    {
        QTreeWidgetItem *pro_leaf = new QTreeWidgetItem(leaf1,pro_list);
        schemaTree->currentItem()->addChild(pro_leaf);
    }
    res = find_funcs(dbName,func_list);
    if(res.compare("success")==0&&func_list.length()!=0)
    {
        QTreeWidgetItem *func_leaf = new QTreeWidgetItem(leaf2,func_list);
        schemaTree->currentItem()->addChild(func_leaf);
    }
    //qDebug("%s",schemaTree->currentItem()->text(0));
    qDebug("ok");
}
