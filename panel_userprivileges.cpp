#include"Panel_UserPrivileges.h"
#include"qapplication.h"
#include"qmessagebox.h"

//#include"winsock.h"
#include"mysql.h"
#include"iostream"
#include"mysqlQuery.h"
#include"qheaderview.h"
//#pragma comment(lib,"libmysql.lib")
Panel_UserPrivileges::Panel_UserPrivileges(QWidget *parent)
: QWidget(parent)
{
	setWindowTitle(tr("User_Privileges"));
	/*****************************左侧****************************/
	labelUserAccount = new QLabel(tr("User Accounts:"));
	tableView = new QTableView();
	ptnAdd = new QPushButton(tr("Add Account"));
	ptnDelete = new QPushButton(tr("Delete"));
	ptnRefresh = new QPushButton(tr("Refresh"));
	//完成三个按钮的布局
	bottomLeftHBoxLayout = new QHBoxLayout();
	bottomLeftHBoxLayout->setSpacing(2); //设定各个控件之间的间距为2
	bottomLeftHBoxLayout->addWidget(ptnAdd);
	bottomLeftHBoxLayout->addWidget(ptnDelete);
	bottomLeftHBoxLayout->addWidget(ptnRefresh);
	//左边垂直布局
	leftVBoxLayout = new QVBoxLayout();
	leftVBoxLayout->setMargin(5);
	leftVBoxLayout->addWidget(labelUserAccount);
	leftVBoxLayout->addWidget(tableView);
	leftVBoxLayout->addLayout(bottomLeftHBoxLayout);
	//右边垂直布局
	tabWidget = new QTabWidget();
	rightVBoxLayout = new QVBoxLayout();
	rightVBoxLayout->addWidget(tabWidget);

	/**************************************tab1**************************/
	tab1 = new QWidget();
	tabWidget->addTab(tab1, QString());
	tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("User_Privileges", "Login", 0));
	tab1LayoutWidget = new  QWidget(tab1);
	tab1GridLayout = new QGridLayout(tab1LayoutWidget);

	labelLoginName = new QLabel(tr("Login Name:"));
	ledName = new QLineEdit();
	labelAuthentication = new QLabel(tr("Authenticaiton Type:"));
	comboBoxPasswordType = new QComboBox();
	comboBoxPasswordType->addItem((QWidget::tr("SHA256 password")));
	comboBoxPasswordType->addItem((QWidget::tr("Standard")));
	labelLimitToHost = new QLabel(tr("Limit to Hosts Maching:"));
	ledHost = new QLineEdit();
	labelPassword = new QLabel(tr("password:"));
	ledPassword = new QLineEdit();
	labelConfirmPassword = new QLabel(tr("Confirm password:"));
	ledConfirm = new QLineEdit();
	ptnExpire = new QPushButton(tr("Expire password"));
	ptnRevert = new QPushButton(tr("Revert"));
	ptnApply = new QPushButton(tr("Apply"));

	//向网格布局中加入需要布局的控件
	tab1GridLayout->addWidget(labelLoginName, 0, 0);
	tab1GridLayout->addWidget(ledName, 0, 1);
	tab1GridLayout->addWidget(labelAuthentication, 1, 0);
	tab1GridLayout->addWidget(comboBoxPasswordType, 1, 1);
	tab1GridLayout->addWidget(labelLimitToHost, 2, 0);
	tab1GridLayout->addWidget(ledHost, 2, 1);
	tab1GridLayout->addWidget(labelPassword, 3, 0);
	tab1GridLayout->addWidget(ledPassword, 3, 1);
	tab1GridLayout->addWidget(labelConfirmPassword, 4, 0);
	tab1GridLayout->addWidget(ledConfirm, 4, 1);
	tab1GridLayout->addWidget(ptnExpire, 5, 1);
	tab1GridLayout->addWidget(ptnRevert, 6, 0);
	tab1GridLayout->addWidget(ptnApply, 6, 1);

	/***************************tab2***********************************/
	tab2 = new QWidget();
	tabWidget->addTab(tab2, QString());
	tabWidget->setTabText(tabWidget->indexOf(tab2), tr("Account Limits"));
	tab2LayoutWidget = new QWidget(tab2);
	tab2GridLayout = new QGridLayout(tab2LayoutWidget);

	labelMaxQueries = new QLabel(tr("Max Queries:"));
	ledQueries = new QLineEdit();
	labelNumberOfQueries = new QLabel(tr("Number of queries the account can execute within one hour."));
	labelMaxUpdates = new QLabel(tr("Max Updates:"));
	ledUpdates = new QLineEdit;
	labelNumberOfUpdates = new QLabel(tr("Number of updates the account can execute within one hour."));
	labelMaxConnections = new QLabel(tr("Max Connections:"));
	ledConnections = new QLineEdit;
	labelNumberOfTimes = new QLabel(tr("The number of times the account can execute within one hour."));
	labelCurrentConnections = new QLabel(tr("Concurrent  Connections:"));
	ledCurrentcon = new QLineEdit;
	labelNumberOfConnections = new QLabel(tr("The number of simultaneous connections the account can execute within one hour."));

	ptnRevert2 = new QPushButton(tr("Revert"));
	ptnApply2 = new QPushButton(tr("Apply"));
	tab2HBoxLayout = new QHBoxLayout();
	tab2HBoxLayout->addWidget(ptnRevert2);
	tab2HBoxLayout->addWidget(ptnApply2);

	tab2GridLayout->addWidget(labelMaxQueries, 0, 0);
	tab2GridLayout->addWidget(ledQueries, 0, 1);
	tab2GridLayout->addWidget(labelNumberOfQueries, 2, 0);
	tab2GridLayout->addWidget(labelMaxUpdates, 3, 0);
	tab2GridLayout->addWidget(ledUpdates, 3, 1);
	tab2GridLayout->addWidget(labelNumberOfUpdates, 4, 0);
	tab2GridLayout->addWidget(labelMaxConnections, 5, 0);
	tab2GridLayout->addWidget(ledConnections, 5, 1);
	tab2GridLayout->addWidget(labelNumberOfTimes, 6, 0);
	tab2GridLayout->addWidget(labelCurrentConnections, 7, 0);
	tab2GridLayout->addWidget(ledCurrentcon, 7, 1);
	tab2GridLayout->addWidget(labelNumberOfConnections, 8, 0);
	tab2GridLayout->addLayout(tab2HBoxLayout, 9, 1);

	/***********************Tb3*****************************/
	tab3 = new QWidget();
	tabWidget->addTab(tab3, QString());
	tabWidget->setTabText(tabWidget->indexOf(tab3), tr("Administrative Roles"));
	tab3LayoutWidget = new QWidget(tab3);
	tab3GridLayout = new QGridLayout(tab3LayoutWidget);

	cboxDelete = new QCheckBox(tr("DELETE"));
	cboxClient = new QCheckBox(tr("REPLICATION CLIENT"));
	cboxAlter = new QCheckBox(tr("ALTER"));
	cboxDrop = new QCheckBox(tr("DROP"));
	cboxSlave = new QCheckBox(tr("REPLICATION SLAVE"));
	cboxAlterRountine = new QCheckBox(tr("ALTER ROUTINE"));
	cboxEvent = new QCheckBox(tr("EVENT"));
	cboxSelect = new QCheckBox(tr("SELECT"));
	cboxCreate = new QCheckBox(tr("CREATE"));
	cboxExecute = new QCheckBox(tr("EXECUTE"));
	cboxShowDatabases = new QCheckBox(tr("SHOW DATABASES"));
	cboxCreateRoutine = new QCheckBox(tr("CREATE ROUTINE"));
	cboxFile = new QCheckBox(tr("FILE"));
	cboxShowView = new QCheckBox(tr("SHOW VIEW"));
	cboxCreateTableSpace = new QCheckBox(tr("CREATE TABLESPACE"));
	cboxOption = new QCheckBox(tr("GRANT OPTION"));
	cboxShutDown = new QCheckBox(tr("SHUTDOWN"));
	cboxIndex = new QCheckBox(tr("INDEX"));
	cboxSuper = new QCheckBox(tr("SUPER"));
	cboxInsert = new QCheckBox(tr("INDEX"));
	cboxTrigger = new QCheckBox(tr("TRIGGER"));
	cboxLockTables = new QCheckBox(tr("LOCK TABLES"));
	cboxUpdate = new QCheckBox(tr("UPDATE"));
	cboxProcess = new QCheckBox(tr("PROCESS"));
	cboxTemporaryTables = new QCheckBox(tr("CREATE TEMPORARY TABLES"));
	cboxReferences = new QCheckBox(tr("REFERENCES"));
	cboxCreateUser = new QCheckBox(tr("CREATE USER"));
	cboxReload = new QCheckBox(tr("RELOAD"));
	cboxCreateView = new QCheckBox(tr("CREATE VIEW"));

	ptnRevoke = new QPushButton(tr("Revoke All Privileges"));
	tab3HBoxLayout1 = new QHBoxLayout();
	tab3HBoxLayout1->addStretch(); //在按钮之前插入一个占位符
	tab3HBoxLayout1->addWidget(ptnRevoke);

	ptnRevert3 = new QPushButton(tr("Revert"));
	ptnApply3 = new QPushButton(tr("Apply"));
	tab3HBoxLayout2 = new QHBoxLayout();
	tab3HBoxLayout2->addStretch(); //在按钮之前插入一个占位符，右对齐
	tab3HBoxLayout2->addWidget(ptnRevert3);
	tab3HBoxLayout2->addWidget(ptnApply3);

	tab3GridLayout->addWidget(cboxDelete, 0, 0);
	tab3GridLayout->addWidget(cboxClient, 0, 1);
	tab3GridLayout->addWidget(cboxAlter, 0, 2);
	tab3GridLayout->addWidget(cboxDrop, 1, 0);
	tab3GridLayout->addWidget(cboxSlave, 1, 1);
	tab3GridLayout->addWidget(cboxAlterRountine, 1, 2);
	tab3GridLayout->addWidget(cboxEvent, 2, 0);
	tab3GridLayout->addWidget(cboxSelect, 2, 1);
	tab3GridLayout->addWidget(cboxCreate, 2, 2);
	tab3GridLayout->addWidget(cboxExecute, 3, 0);
	tab3GridLayout->addWidget(cboxShowDatabases, 3, 1);
	tab3GridLayout->addWidget(cboxCreateRoutine, 3, 2);
	tab3GridLayout->addWidget(cboxFile, 4, 0);
	tab3GridLayout->addWidget(cboxShowView, 4, 1);
	tab3GridLayout->addWidget(cboxCreateTableSpace, 4, 2);
	tab3GridLayout->addWidget(cboxOption, 5, 0);
	tab3GridLayout->addWidget(cboxShutDown, 5, 1);
	tab3GridLayout->addWidget(cboxIndex, 6, 0);
	tab3GridLayout->addWidget(cboxSuper, 6, 1);
	tab3GridLayout->addWidget(cboxInsert, 7, 0);
	tab3GridLayout->addWidget(cboxTrigger, 7, 1);
	tab3GridLayout->addWidget(cboxLockTables, 8, 0);
	tab3GridLayout->addWidget(cboxUpdate, 8, 1);
	tab3GridLayout->addWidget(cboxProcess, 9, 0);
	tab3GridLayout->addWidget(cboxTemporaryTables, 9, 1);
	tab3GridLayout->addWidget(cboxReferences, 10, 0);
	tab3GridLayout->addWidget(cboxCreateUser, 10, 1);
	tab3GridLayout->addWidget(cboxReload, 11, 0);
	tab3GridLayout->addWidget(cboxCreateView, 11, 1);
	tab3GridLayout->addLayout(tab3HBoxLayout1, 13, 0);
	tab3GridLayout->addLayout(tab3HBoxLayout2, 14, 2);
	/**************************tab4*****************************/
	tab4 = new QWidget();
	tabWidget->addTab(tab4, QString());
	tabWidget->setTabText(tabWidget->indexOf(tab4), tr("Schema Privileges"));
	tab4LayoutWidget = new QWidget(tab4);
	tab4VBoxLayout2 = new QVBoxLayout(tab4LayoutWidget);

	tab4TableView = new QTableView();
	tab4PtnAddEntry = new QPushButton(tr("Add Entry"));
	tab4PtnDeleteEntry = new QPushButton(tr("Delete Entry"));
	tab4PtnRevokeAllPriv = new QPushButton(tr("Revoke All Privileges"));
	labelSelectSchema = new QLabel(tr("SelectSchema:"));
	tab4ComboBoxSchema = new QComboBox();

	//上半部分
	tab4VBoxLayout1 = new QVBoxLayout();
	tab4HBoxLayout1 = new QHBoxLayout();
	tab4HBoxLayout0 = new QHBoxLayout();
	tab4HBoxLayout0->addWidget(labelSelectSchema);
	tab4HBoxLayout0->addWidget(tab4ComboBoxSchema);
	tab4VBoxLayout1->addWidget(tab4PtnAddEntry);
	tab4VBoxLayout1->addWidget(tab4PtnDeleteEntry);
	tab4VBoxLayout1->addLayout(tab4HBoxLayout0);
	tab4HBoxLayout1->addWidget(tab4TableView);
	tab4HBoxLayout1->addLayout(tab4VBoxLayout1);
	//下半部分
	tab4GridLayout = new QGridLayout();

	tab4CboxSelect = new QCheckBox(tr("SELECT"));
	tab4CboxInsert = new QCheckBox(tr("INSERT"));
	tab4CboxUpdate = new QCheckBox(tr("UPDATE"));
	tab4CboxDelete = new QCheckBox(tr("DELETE"));
	tab4CboxExecute = new QCheckBox(tr("EXECUTE"));
	tab4CboxShowView = new QCheckBox(tr("SHOW VIEW"));

	tab4CboxCreate = new QCheckBox(tr("CREATE"));
	tab4CboxAlter = new QCheckBox(tr("ALTER"));
	tab4CboxReferences = new QCheckBox(tr("REFERENCES"));
	tab4CboxIndex = new QCheckBox(tr("INDEX"));
	tab4CboxCreateView = new QCheckBox(tr("CREATE VIEW"));
	tab4CboxCreateRoutine = new QCheckBox(tr("CREATE ROUTINE"));
	tab4CboxAlterRoutine = new QCheckBox(tr("ALTER ROUTINE"));
	tab4CboxEvent = new QCheckBox(tr("EVENT"));
	tab4CboxDrop = new QCheckBox(tr("DROP"));
	tab4CboxTrigger = new QCheckBox(tr("TRIGGER"));

	tab4CboxGrantOption = new QCheckBox(tr("GRANT OPTION"));
	tab4CboxCreateTempTables = new QCheckBox(tr("CREATE TRMPORARY TABLES"));
	tab4CboxLockTables = new QCheckBox(tr("LOCK TABLES"));

	tab4GridLayout->addWidget(tab4CboxSelect, 0, 0);
	tab4GridLayout->addWidget(tab4CboxInsert, 1, 0);
	tab4GridLayout->addWidget(tab4CboxUpdate, 2, 0);
	tab4GridLayout->addWidget(tab4CboxDelete, 3, 0);
	tab4GridLayout->addWidget(tab4CboxExecute, 4, 0);
	tab4GridLayout->addWidget(tab4CboxShowView, 5, 0);

	tab4GridLayout->addWidget(tab4CboxCreate, 0, 1);
	tab4GridLayout->addWidget(tab4CboxAlter, 1, 1);
	tab4GridLayout->addWidget(tab4CboxReferences, 2, 1);
	tab4GridLayout->addWidget(tab4CboxIndex, 3, 1);
	tab4GridLayout->addWidget(tab4CboxCreateView, 4, 1);
	tab4GridLayout->addWidget(tab4CboxCreateRoutine, 5, 1);

	tab4GridLayout->addWidget(tab4CboxAlterRoutine, 0, 2);
	tab4GridLayout->addWidget(tab4CboxEvent, 1, 2);
	tab4GridLayout->addWidget(tab4CboxDrop, 2, 2);
	tab4GridLayout->addWidget(tab4CboxTrigger, 3, 2);

	tab4GridLayout->addWidget(tab4CboxGrantOption, 0, 3);
	tab4GridLayout->addWidget(tab4CboxCreateTempTables, 1, 3);
	tab4GridLayout->addWidget(tab4CboxLockTables, 2, 3);

	tab4HBoxLayout2 = new QHBoxLayout();
	tab4PtnUnselectAll = new QPushButton(tr("Unselect All"));
	tab4PtnSelctAll = new QPushButton(tr("Select All"));
	tab4HBoxLayout2->addWidget(tab4PtnUnselectAll);
	tab4HBoxLayout2->addWidget(tab4PtnSelctAll);

	tab4GridLayout->addLayout(tab4HBoxLayout2, 4, 2);
	tab4GridLayout->addWidget(tab4PtnRevokeAllPriv, 4, 3);

	tab4HBoxLayout3 = new QHBoxLayout();
	ptnApply4 = new QPushButton(tr("Apply"));
	ptnRevert4 = new QPushButton(tr("Revert"));
	tab4HBoxLayout3->addStretch();
	tab4HBoxLayout3->addWidget(ptnApply4);
	tab4HBoxLayout3->addWidget(ptnRevert4);

	tab4VBoxLayout2->addLayout(tab4HBoxLayout1);
	tab4VBoxLayout2->addLayout(tab4GridLayout);
	tab4VBoxLayout2->addLayout(tab4HBoxLayout3);

	/******************************************************/
	//主布局
	QWidget *Window = new QWidget(this);
	Window->resize(950, 450);
	QHBoxLayout *MainLayout = new QHBoxLayout(Window);
	MainLayout->setMargin(15);//设定对话框的边距为15
	MainLayout->setSpacing(10);
	MainLayout->addLayout(leftVBoxLayout, 1);
	MainLayout->addLayout(rightVBoxLayout, 6);
	/**********************************************************/
	//界面初始化
	comboBoxPasswordType->setEnabled(false);
	ledName->setEnabled(false);
	ledHost->setEnabled(false);
	ledPassword->setEnabled(false);
	ledConfirm->setEnabled(false);
	ledQueries->setText("0");
	ledUpdates->setText("0");
	ledConnections->setText("0");
	ledCurrentcon->setText("0");
	ledPassword->setEchoMode(QLineEdit::Password);
	ledConfirm->setEchoMode(QLineEdit::Password);
	ptnExpire->setEnabled(false);
	ptnRevert->setEnabled(false);
	ptnApply->setEnabled(false);
	ptnDelete->setEnabled(false);
	tabWidget->setEnabled(false);
	tab1->setEnabled(false);
	tab2->setEnabled(false);
	tab3->setEnabled(false);
	tab4->setEnabled(false);
	tab4ComboBoxSchema->setEnabled(false);
	tab4CboxSelect->setEnabled(false);
	tab4CboxInsert->setEnabled(false);
	tab4CboxUpdate->setEnabled(false);
	tab4CboxDelete->setEnabled(false);
	tab4CboxExecute->setEnabled(false);
	tab4CboxShowView->setEnabled(false);
	tab4CboxCreate->setEnabled(false);
	tab4CboxAlter->setEnabled(false);
	tab4CboxReferences->setEnabled(false);
	tab4CboxIndex->setEnabled(false);
	tab4CboxCreateView->setEnabled(false);
	tab4CboxCreateRoutine->setEnabled(false);
	tab4CboxAlterRoutine->setEnabled(false);
	tab4CboxEvent->setEnabled(false);
	tab4CboxDrop->setEnabled(false);
	tab4CboxTrigger->setEnabled(false);
	tab4CboxGrantOption->setEnabled(false);
	tab4CboxCreateTempTables->setEnabled(false);
	tab4CboxLockTables->setEnabled(false);
	tab4PtnUnselectAll->setEnabled(false);
	tab4PtnSelctAll->setEnabled(false);
	tab4PtnDeleteEntry->setEnabled(false);
	/*******************************************************************/
	QString judgement;
	QStringList userHost;
	int i = 0;
	int numberOfColumns = 0; //获取列的数目
	int numberOfRows = 0; //获取行的数目

	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		QStandardItemModel *userModel = new QStandardItemModel();
		tableView->setModel(userModel);
		userModel->setHorizontalHeaderItem(0, new QStandardItem("User")); //添加列名
		userModel->setHorizontalHeaderItem(1, new QStandardItem("host")); //添加列名
		for (int m = 0; m < numberOfRows; m++)
		{
			for (int n = 0; n < numberOfColumns; n++)
			{
				userModel->setItem(m, n, new QStandardItem(userHost.at(i)));
				i++;
				if (i > numberOfColumns*numberOfRows)
				{
					break;
				}
			}
		}
	}
	else
	{
		qDebug("ERROR");
	}
	/**********************************************************/
	//槽函数连接
	connect(ptnRefresh, SIGNAL(clicked()), this, SLOT(Refresh_Slot()));
	connect(tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(Show_User_Slot()));
	connect(ptnApply, SIGNAL(clicked()), this, SLOT(Create_User_Slot()));
	connect(ptnAdd, SIGNAL(clicked()), this, SLOT(Add_Acount_Slot()));
	connect(ptnExpire, SIGNAL(clicked()), this, SLOT(Expire_Password_Slot()));
	connect(ptnRevert, SIGNAL(clicked()), this, SLOT(Revert_Slot()));
	connect(ptnDelete, SIGNAL(clicked()), this, SLOT(Delete_Slot()));
	connect(tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(Show_Privileges_Slot()));
	connect(ptnApply3, SIGNAL(clicked()), this, SLOT(Authority_Management_Slot()));
	connect(ptnRevoke, SIGNAL(clicked()), this, SLOT(Revoke_AllPrivileges_Slot()));
	connect(ptnRevert3, SIGNAL(clicked()), this, SLOT(Revert_CheckBox_Slot()));
	connect(tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(User_Resource_Slot()));
	connect(ptnApply2, SIGNAL(clicked()), this, SLOT(Set_User_Resource_Slot()));
	connect(ptnRevert2, SIGNAL(clicked()), this, SLOT(Revert_User_Resource_Slot()));
	connect(tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(Query_Schema_Slot()));
	connect(tab4PtnAddEntry, SIGNAL(clicked()), this, SLOT(Add_Entry_Slot()));
	connect(tab4PtnSelctAll, SIGNAL(clicked()), this, SLOT(Select_All_Slot()));
	connect(tab4PtnUnselectAll, SIGNAL(clicked()), this, SLOT(Unselect_All_Slot()));
	connect(ptnApply4, SIGNAL(clicked()), this, SLOT(Set_Schema_Privileges_Slot()));
	connect(tab4TableView, SIGNAL(clicked(QModelIndex)), this, SLOT(Show_Schema_Privileges_Slot()));
	connect(tab4PtnRevokeAllPriv, SIGNAL(clicked()), this, SLOT(Show_Schema_Privileges_Slot()));
	connect(tab4PtnDeleteEntry, SIGNAL(clicked()), this, SLOT(Delete_Entry_Slot()));
	connect(ptnRevert4, SIGNAL(clicked()), this, SLOT(Revert_Schema_Slot()));
	//connect(tab4PtnRevokeAllPriv, SIGNAL(clicked()), this, SLOT(Revoke_Schema_Privileges_Slot()));
	/**********************************************************/

}

Panel_UserPrivileges::~Panel_UserPrivileges()
{
    
}
void Panel_UserPrivileges::Refresh_Slot()
{
	QStringList userHost;
	QString judgement;
	int i = 0;
	int numberOfColumns = 0; //获取列的数目
	int numberOfRows = 0; //获取行的数目

	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		QStandardItemModel *userModel = new QStandardItemModel();
		tableView->setModel(userModel);
		userModel->setHorizontalHeaderItem(0, new QStandardItem("User")); //添加列名
		userModel->setHorizontalHeaderItem(1, new QStandardItem("host")); //添加列名
		for (int m = 0; m < numberOfRows; m++)
		{
			for (int n = 0; n < numberOfColumns; n++)
			{
				userModel->setItem(m, n, new QStandardItem(userHost.at(i)));
				i++;
				if (i > numberOfColumns*numberOfRows)
				{
					break;
				}
			}
		}
	}
	else
	{
		qDebug("error!");
	}
	tableView->setColumnWidth(0, 120);
	tableView->setColumnWidth(1, 120);
	tabWidget->setEnabled(false);
	ledName->setText("");
	ledHost->setText("");
	ledPassword->setText("");
	ledConfirm->setText("");
	ptnExpire->setEnabled(false);
	ptnRevert->setEnabled(false);
	ptnApply->setEnabled(false);
	ptnDelete->setEnabled(false);
}
void Panel_UserPrivileges::Show_User_Slot()
{
	tabWidget->setEnabled(true);
	tab1->setEnabled(true);
	tab2->setEnabled(true);
	tab3->setEnabled(true);
	ptnDelete->setEnabled(true);
	ledName->setEnabled(true);
	comboBoxPasswordType->setEnabled(true);
	ledHost->setEnabled(true);
	ledPassword->setEnabled(true);
	ledConfirm->setEnabled(true);
	ptnExpire->setEnabled(true);
	comboBoxPasswordType->setEnabled(false);


	int currentRow = tableView->currentIndex().row(); //获取当前行

	int numberOfColumns = 0; //列的数目
	int numberOfRows = 0; //行的数目

	QStringList userHost;
	QStringList password;
	QString judgement;
	QString userName;

	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		userName = userHost.at(2 * currentRow);
		ledName->setText(userHost.at(2 * currentRow));
		ledHost->setText(userHost.at(2 * currentRow + 1));

		judgement = Query_Authentication(password, userName);
		if (judgement == "OK")
		{
			ledPassword->setText(password.at(0));
			ledConfirm->setText(password.at(0));
		}
		else
		{
			qDebug("ERROR!");
		}

	}
	else
	{
		qDebug("ERROR!");
	}
}
void Panel_UserPrivileges::Create_User_Slot()
{
	QString strName = ledName->text();
	QString strHost = ledHost->text();
	QString strPassword = ledPassword->text();
	QString strConfirm = ledConfirm->text();

	if (strName == "" || strHost == "" || strPassword == "" || strConfirm == "")
	{
		QMessageBox::warning(this, tr("Warning!"), tr("User information is not complete!"), QMessageBox::Ok);
		return;

	}
	else if (QString::compare(strPassword, strConfirm))
	{
		QMessageBox::warning(this, tr("Warning!"), tr("Before and after the password is not consistent, please re-enter the password!"), QMessageBox::Ok);
		return;
	}

	QString judgement;
	judgement = Create_User(strName, strHost, strPassword);
	Refresh_Slot();

	tabWidget->setEnabled(false);
	ledName->setText("");
	ledHost->setText("");
	ledPassword->setText("");
	ledConfirm->setText("");
	ledName->setEnabled(false);
	comboBoxPasswordType->setEnabled(false);
	ledHost->setEnabled(false);
	ledPassword->setEnabled(false);
	ledConfirm->setEnabled(false);
	ptnExpire->setEnabled(false);
	ptnApply->setEnabled(false);
	ptnRevert->setEnabled(false);
}
void Panel_UserPrivileges::Add_Acount_Slot()
{
    tabWidget->setEnabled(true);
    tab1->setEnabled(true);
    tab2->setEnabled(false);
    tab3->setEnabled(false);
    ledName->setEnabled(true);
    comboBoxPasswordType->setEnabled(true);
    ledHost->setEnabled(true);
    ledPassword->setEnabled(true);
    ledConfirm->setEnabled(true);
    ptnExpire->setEnabled(true);
    ptnApply->setEnabled(true);
    ptnRevert->setEnabled(true);
    ledName->setText("newuser");
    ledHost->setText("%");
    ledPassword->setText("");
    ledConfirm->setText("");
}
void Panel_UserPrivileges::Revert_Slot()
{
    ledName->setText("");
    ledHost->setText("");
    ledPassword->setText("");
    ledConfirm->setText("");
}
void Panel_UserPrivileges::Revert_CheckBox_Slot()
{
    Show_Privileges_Slot();
}
void Panel_UserPrivileges::Expire_Password_Slot()
{
    ledPassword->setText("");
    ledConfirm->setText("");
    ptnApply->setEnabled(true);
    ptnRevert->setEnabled(true);
}
void Panel_UserPrivileges::Delete_Slot()
{
	int currentRow = tableView->currentIndex().row(); //获取当前行
	QString  judgement;
	QString userName;
	QStringList userHost;
	int numberOfColumns = 0;
	int numberOfRows = 0;
	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		userName = userHost.at(2 * currentRow);
		judgement = Delete_User(userName);
	}
	else
	{
		QMessageBox::information(this, "ERROR!", judgement);
	}
	Refresh_Slot();
}
void Panel_UserPrivileges::Show_Privileges_Slot()
{
	int currentRow = tableView->currentIndex().row(); //获取当前行
	tab3->setEnabled(true);

	QString  judgement;
	QString userName;
	QString host;
	QStringList userHost;
	QStringList userPriv;
	int numberOfColumns = 0;
	int numberOfRows = 0;

	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		userName = userHost.at(2 * currentRow);
		host = userHost.at(2 * currentRow + 1);

		judgement = Query_Privileges_Variables(userName, host);
		if (judgement == "OK")
		{
			judgement = Query_Variables(userPriv);
			if (judgement == "OK")
			{
				QString selectPriv = userPriv.at(0);
				QString insertPriv = userPriv.at(1);
				QString updatePriv = userPriv.at(2);
				QString deletePriv = userPriv.at(3);
				QString createPriv = userPriv.at(4);
				QString dropPriv = userPriv.at(5);
				QString reloadPriv = userPriv.at(6);
				QString shutDownPriv = userPriv.at(7);
				QString processPriv = userPriv.at(8);
				QString filePriv = userPriv.at(9);
				QString grantPriv = userPriv.at(10);
				QString referencesPriv = userPriv.at(11);
				QString indexPriv = userPriv.at(12);
				QString alterPriv = userPriv.at(13);
				QString showDbPriv = userPriv.at(14);
				QString superPriv = userPriv.at(15);
				QString createTmpTablePriv = userPriv.at(16);
				QString lockTablesPriv = userPriv.at(17);
				QString executePriv = userPriv.at(18);
				QString replSlavePriv = userPriv.at(19);
				QString replClientPriv = userPriv.at(20);
				QString createViewPriv = userPriv.at(21);
				QString showViewPriv = userPriv.at(22);
				QString createRoutinePriv = userPriv.at(23);
				QString alterRoutinePriv = userPriv.at(24);
				QString createUserPriv = userPriv.at(25);
				QString eventPriv = userPriv.at(26);
				QString triggerPriv = userPriv.at(27);
				QString createTableSpacePriv = userPriv.at(28);

				if (selectPriv == "Y")
				{
					cboxSelect->setCheckState(Qt::Checked);
				}
				else
				{
					cboxSelect->setCheckState(Qt::Unchecked);
				}
				if (insertPriv == "Y")
				{
					cboxInsert->setCheckState(Qt::Checked);
				}
				else
				{
					cboxInsert->setCheckState(Qt::Unchecked);
				}
				if (updatePriv == "Y")
				{
					cboxUpdate->setCheckState(Qt::Checked);
				}
				else
				{
					cboxUpdate->setCheckState(Qt::Unchecked);
				}
				if (deletePriv == "Y")
				{
					cboxDelete->setCheckState(Qt::Checked);
				}
				else
				{
					cboxDelete->setCheckState(Qt::Unchecked);
				}
				if (createPriv == "Y")
				{
					cboxCreate->setCheckState(Qt::Checked);
				}
				else
				{
					cboxCreate->setCheckState(Qt::Unchecked);
				}
				if (dropPriv == "Y")
				{
					cboxDrop->setCheckState(Qt::Checked);
				}
				else
				{
					cboxDrop->setCheckState(Qt::Unchecked);
				}
				if (reloadPriv == "Y")
				{
					cboxReload->setCheckState(Qt::Checked);
				}
				else
				{
					cboxReload->setCheckState(Qt::Unchecked);
				}
				if (shutDownPriv == "Y")
				{
					cboxShutDown->setCheckState(Qt::Checked);
				}
				else
				{
					cboxShutDown->setCheckState(Qt::Unchecked);
				}
				if (processPriv == "Y")
				{
					cboxProcess->setCheckState(Qt::Checked);
				}
				else
				{
					cboxProcess->setCheckState(Qt::Unchecked);
				}
				if (filePriv == "Y")
				{
					cboxFile->setCheckState(Qt::Checked);
				}
				else
				{
					cboxFile->setCheckState(Qt::Unchecked);
				}
				if (grantPriv == "Y")
				{
					cboxOption->setCheckState(Qt::Checked);
				}
				else
				{
					cboxOption->setCheckState(Qt::Unchecked);
				}
				if (referencesPriv == "Y")
				{
					cboxReferences->setCheckState(Qt::Checked);
				}
				else
				{
					cboxReferences->setCheckState(Qt::Unchecked);
				}
				if (indexPriv == "Y")
				{
					cboxIndex->setCheckState(Qt::Checked);
				}
				else
				{
					cboxIndex->setCheckState(Qt::Unchecked);
				}
				if (alterPriv == "Y")
				{
					cboxAlter->setCheckState(Qt::Checked);
				}
				else
				{
					cboxAlter->setCheckState(Qt::Unchecked);
				}
				if (showDbPriv == "Y")
				{
					cboxShowDatabases->setCheckState(Qt::Checked);
				}
				else
				{
					cboxShowDatabases->setCheckState(Qt::Unchecked);
				}
				if (superPriv == "Y")
				{
					cboxSuper->setCheckState(Qt::Checked);
				}
				else
				{
					cboxSuper->setCheckState(Qt::Unchecked);
				}
				if (createTmpTablePriv == "Y")
				{
					cboxTemporaryTables->setCheckState(Qt::Checked);
				}
				else
				{
					cboxTemporaryTables->setCheckState(Qt::Unchecked);
				}
				if (lockTablesPriv == "Y")
				{
					cboxLockTables->setCheckState(Qt::Checked);
				}
				else
				{
					cboxLockTables->setCheckState(Qt::Unchecked);
				}
				if (executePriv == "Y")
				{
					cboxExecute->setCheckState(Qt::Checked);
				}
				else
				{
					cboxExecute->setCheckState(Qt::Unchecked);
				}
				if (replSlavePriv == "Y")
				{
					cboxSlave->setCheckState(Qt::Checked);
				}
				else
				{
					cboxSlave->setCheckState(Qt::Unchecked);
				}
				if (replClientPriv == "Y")
				{
					cboxClient->setCheckState(Qt::Checked);
				}
				else
				{
					cboxClient->setCheckState(Qt::Unchecked);
				}
				if (createViewPriv == "Y")
				{
					cboxCreateView->setCheckState(Qt::Checked);
				}
				else
				{
					cboxCreateView->setCheckState(Qt::Unchecked);
				}
				if (showViewPriv == "Y")
				{
					cboxShowView->setCheckState(Qt::Checked);
				}
				else
				{
					cboxShowView->setCheckState(Qt::Unchecked);
				}
				if (createRoutinePriv == "Y")
				{
					cboxCreateRoutine->setCheckState(Qt::Checked);
				}
				else
				{
					cboxCreateRoutine->setCheckState(Qt::Unchecked);
				}
				if (alterRoutinePriv == "Y")
				{
					cboxAlterRountine->setCheckState(Qt::Checked);
				}
				else
				{
					cboxAlterRountine->setCheckState(Qt::Unchecked);
				}
				if (createUserPriv == "Y")
				{
					cboxCreateUser->setCheckState(Qt::Checked);
				}
				else
				{
					cboxCreateUser->setCheckState(Qt::Unchecked);
				}
				if (eventPriv == "Y")
				{
					cboxEvent->setCheckState(Qt::Checked);
				}
				else
				{
					cboxEvent->setCheckState(Qt::Unchecked);
				}
				if (triggerPriv == "Y")
				{
					cboxTrigger->setCheckState(Qt::Checked);
				}
				else
				{
					cboxTrigger->setCheckState(Qt::Unchecked);
				}
				if (createTableSpacePriv == "Y")
				{
					cboxCreateTableSpace->setCheckState(Qt::Checked);
				}
				else
				{
					cboxCreateTableSpace->setCheckState(Qt::Unchecked);
				}
			}
		}
	}
	else
	{
		qDebug("ERROR!");
	}
}
void Panel_UserPrivileges::Revoke_AllPrivileges_Slot()
{
	int currentRow = tableView->currentIndex().row(); //获取当前行
	tab3->setEnabled(true);

	QString  judgement;
	QStringList userHost;
	int numberOfColumns = 0;
	int numberOfRows = 0;
	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		QString userName = userHost.at(2 * currentRow);
		QString host = userHost.at(2 * currentRow + 1);
		switch (QMessageBox::warning(this, tr("Revoke All Privileges"), tr("Please confirm revokation of all privileges!"), QMessageBox::Ok | QMessageBox::Cancel))
		{
		case QMessageBox::Ok:
			judgement = Revoke_Privileges(userName, host);
			if (judgement == "OK")
			{
				QMessageBox::information(this, tr("Revoke All Privileges"), tr("Revoke all privileges successfully!"), QMessageBox::Ok);
				cboxSelect->setCheckState(Qt::Unchecked);
				cboxInsert->setCheckState(Qt::Unchecked);
				cboxUpdate->setCheckState(Qt::Unchecked);
				cboxDelete->setCheckState(Qt::Unchecked);
				cboxCreate->setCheckState(Qt::Unchecked);
				cboxDrop->setCheckState(Qt::Unchecked);
				cboxReload->setCheckState(Qt::Unchecked);
				cboxShutDown->setCheckState(Qt::Unchecked);
				cboxProcess->setCheckState(Qt::Unchecked);
				cboxFile->setCheckState(Qt::Unchecked);
				cboxOption->setCheckState(Qt::Unchecked);
				cboxReferences->setCheckState(Qt::Unchecked);
				cboxIndex->setCheckState(Qt::Unchecked);
				cboxAlter->setCheckState(Qt::Unchecked);
				cboxShowDatabases->setCheckState(Qt::Unchecked);
				cboxSuper->setCheckState(Qt::Unchecked);
				cboxTemporaryTables->setCheckState(Qt::Unchecked);
				cboxLockTables->setCheckState(Qt::Unchecked);
				cboxExecute->setCheckState(Qt::Unchecked);
				cboxSlave->setCheckState(Qt::Unchecked);
				cboxClient->setCheckState(Qt::Unchecked);
				cboxCreateView->setCheckState(Qt::Unchecked);
				cboxShowView->setCheckState(Qt::Unchecked);
				cboxCreateRoutine->setCheckState(Qt::Unchecked);
				cboxAlterRountine->setCheckState(Qt::Unchecked);
				cboxCreateUser->setCheckState(Qt::Unchecked);
				cboxEvent->setCheckState(Qt::Unchecked);
				cboxTrigger->setCheckState(Qt::Unchecked);
				cboxCreateTableSpace->setCheckState(Qt::Unchecked);
			}
			break;
		case QMessageBox::Cancel:
			QMessageBox::information(this, tr("Revoke All Privileges"), tr("You have already cancelled cancel the user permissions."), QMessageBox::Ok);
			break;
		default:
			break;
		}
	}
	else
	{
		qDebug("ERROR!");
	}
}
void Panel_UserPrivileges::Authority_Management_Slot()
{
	int currentRow = tableView->currentIndex().row(); //获取当前行
	tab3->setEnabled(true);

	QString  judgement;
	int identifier;
	QStringList userHost;
	int numberOfColumns = 0;
	int numberOfRows = 0;
	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		QString userName = userHost.at(2 * currentRow);
		QString host = userHost.at(2 * currentRow + 1);
		if (cboxSelect->isChecked())
		{
			identifier = 0;
			judgement = Authority_Management(identifier, userName, host);
		}
		if (cboxInsert->isChecked())
		{
			identifier = 1;
			judgement = Authority_Management(identifier, userName, host);
		}
		if (cboxUpdate->isChecked())
		{
			identifier = 2;
			judgement = Authority_Management(identifier, userName, host);
		}
		if (cboxDelete->isChecked())
		{
			identifier = 3;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxCreate->isChecked())
		{
			identifier = 4;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxDrop->isChecked())
		{
			identifier = 5;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxReload->isChecked())
		{
			identifier = 6;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxShutDown->isChecked())
		{
			identifier = 7;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxProcess->isChecked())
		{
			identifier = 8;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxFile->isChecked())
		{
			identifier = 9;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxOption->isChecked())
		{
			identifier = 10;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxReferences->isChecked())
		{
			identifier = 11;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxIndex->isChecked())
		{
			identifier = 12;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxAlter->isChecked())
		{
			identifier = 13;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxShowDatabases->isChecked())
		{
			identifier = 14;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxSuper->isChecked())
		{
			identifier = 15;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxTemporaryTables->isChecked())
		{
			identifier = 16;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxLockTables->isChecked())
		{
			identifier = 17;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxExecute->isChecked())
		{
			identifier = 18;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxSlave->isChecked())
		{
			identifier = 19;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxClient->isChecked())
		{
			identifier = 20;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxCreateView->isChecked())
		{
			identifier = 21;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxShowView->isChecked())
		{
			identifier = 22;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxCreateRoutine->isChecked())
		{
			identifier = 23;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxAlterRountine->isChecked())
		{
			identifier = 24;
			judgement = Authority_Management(identifier, userName, host);

		}
		if (cboxCreateUser->isChecked())
		{
			identifier = 25;
			judgement = Authority_Management(identifier, userName, host);
		}
		if (cboxEvent->isChecked())
		{
			identifier = 26;
			judgement = Authority_Management(identifier, userName, host);
		}
		if (cboxTrigger->isChecked())
		{
			identifier = 27;
			judgement = Authority_Management(identifier, userName, host);
		}
		if (cboxCreateTableSpace->isChecked())
		{
			identifier = 28;
			judgement = Authority_Management(identifier, userName, host);
		}
		QMessageBox::information(this, tr("Set User's Privileges"), tr("Set the user's privileges successfully!"), QMessageBox::Ok);
	}
	else
	{
		qDebug("ERROR!");
	}
}
void Panel_UserPrivileges::User_Resource_Slot()
{
	int currentRow = tableView->currentIndex().row(); //获取当前行
	tab2->setEnabled(true);

	QString  judgement;
	QStringList userResource;
	QStringList userHost;
	int numberOfColumns = 0;
	int numberOfRows = 0;

	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		QString userName = userHost.at(2 * currentRow);
		QString host = userHost.at(2 * currentRow + 1);
		judgement = Query_UserResource(userResource, userName, host);
		if (judgement == "OK")
		{
			QString MaxQuestions = userResource.at(0);
			QString MaxUpdates = userResource.at(1);
			QString MaxConnections = userResource.at(2);
			QString MaxUserConnections = userResource.at(3);
			ledQueries->setText(MaxQuestions);
			ledUpdates->setText(MaxUpdates);
			ledConnections->setText(MaxConnections);
			ledCurrentcon->setText(MaxUserConnections);
		}
	}
	else
	{
		qDebug("ERROR!");
	}
}
void Panel_UserPrivileges::Revert_User_Resource_Slot()
{
    User_Resource_Slot();
}
void Panel_UserPrivileges::Set_User_Resource_Slot()
{
	int currentRow = tableView->currentIndex().row(); //获取当前行
	tab2->setEnabled(true);

	QString  judgement;
	QStringList userResource;
	QStringList userHost;
	int numberOfColumns = 0;
	int numberOfRows = 0;

	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		QString userName = userHost.at(2 * currentRow);
		QString host = userHost.at(2 * currentRow + 1);
		QString Questions = ledQueries->text();
		QString Updates = ledUpdates->text();
		QString Connections = ledConnections->text();
		QString UserConnections = ledCurrentcon->text();
		judgement = Set_UserResource(userName, host, Questions, Updates, Connections, UserConnections);
		if (judgement == "OK")
		{
			QMessageBox::information(this, tr("Set User's Resource!"), tr("Set the user's resource successfully!"), QMessageBox::Ok);
		}
		else
		{
			qDebug("ERROR!");
		}
	}
	else
	{
		qDebug("ERROR!");
	}
}

//tab4

void Panel_UserPrivileges::Query_Schema_Slot()
{
	int currentRow = tableView->currentIndex().row(); //获取当前行
	tab4->setEnabled(true);

	QString judgement;
	QStringList userHost;
	QStringList schema; //查找所有已分配权限Schema
	int i = 0;
	int numberOfColumns = 0;
	int numberOfRows = 0; //行数
	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		QString userName = userHost.at(2 * currentRow);
		QString host = userHost.at(2 * currentRow + 1);
		judgement = Query_Schema(schema, userName, host, &numberOfRows);
		if (judgement == "OK")
		{
			QStandardItemModel *schemaModel = new QStandardItemModel();
			tab4TableView->setModel(schemaModel);
			tab4TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //列宽度自适应
			schemaModel->setHorizontalHeaderItem(0, new QStandardItem("Schema")); //添加列名
			for (int m = 0; m < numberOfRows; m++)
			{
				schemaModel->setItem(m, 0, new QStandardItem(schema.at(i)));
				i++;
				if (i > numberOfRows)
				{
					break;
				}
			}
		}
		else
		{
			qDebug("ERROR!");
		}
	}
	else
	{
		qDebug("ERROR!");
	}
}
void Panel_UserPrivileges::Add_Entry_Slot()
{
	tab4ComboBoxSchema->setEnabled(true);
	tab4CboxSelect->setEnabled(true);
	tab4CboxInsert->setEnabled(true);
	tab4CboxUpdate->setEnabled(true);
	tab4CboxDelete->setEnabled(true);
	tab4CboxExecute->setEnabled(true);
	tab4CboxShowView->setEnabled(true);
	tab4CboxCreate->setEnabled(true);
	tab4CboxAlter->setEnabled(true);
	tab4CboxReferences->setEnabled(true);
	tab4CboxIndex->setEnabled(true);
	tab4CboxCreateView->setEnabled(true);
	tab4CboxCreateRoutine->setEnabled(true);
	tab4CboxAlterRoutine->setEnabled(true);
	tab4CboxEvent->setEnabled(true);
	tab4CboxDrop->setEnabled(true);
	tab4CboxTrigger->setEnabled(true);
	tab4CboxGrantOption->setEnabled(true);
	tab4CboxCreateTempTables->setEnabled(true);
	tab4CboxLockTables->setEnabled(true);
	tab4PtnUnselectAll->setEnabled(true);
	tab4PtnSelctAll->setEnabled(true);
	tab4ComboBoxSchema->setEnabled(true);
	Unselect_All_Slot();
	QStringList schemaList;
	bool judgement;
	judgement = getAllDatabases(schemaList);

	if (judgement == true)
	{
		for (QStringList::iterator it = schemaList.begin(); it != schemaList.end(); ++it)
		{
			QString current = *it;
			tab4ComboBoxSchema->addItem(current);
		}
	}

}
void Panel_UserPrivileges::Set_Schema_Privileges_Slot()
{
	QString schema = tab4ComboBoxSchema->currentText();
	int currentRow = tableView->currentIndex().row(); //获取当前行

	QString  judgement;
	int identifier;
	QStringList userHost;
	int numberOfColumns = 0;
	int numberOfRows = 0;
	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		QString userName = userHost.at(2 * currentRow);
		QString host = userHost.at(2 * currentRow + 1);
		if (tab4CboxSelect->isChecked())
		{
			identifier = 0;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxInsert->isChecked())
		{
			identifier = 1;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxUpdate->isChecked())
		{
			identifier = 2;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxDelete->isChecked())
		{
			identifier = 3;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxExecute->isChecked())
		{
			identifier = 4;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxShowView->isChecked())
		{
			identifier = 5;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxCreate->isChecked())
		{
			identifier = 6;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxAlter->isChecked())
		{
			identifier = 7;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxReferences->isChecked())
		{
			identifier = 8;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxIndex->isChecked())
		{
			identifier = 9;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxCreateView->isChecked())
		{
			identifier = 10;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxCreateRoutine->isChecked())
		{
			identifier = 11;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxAlterRoutine->isChecked())
		{
			identifier = 12;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxEvent->isChecked())
		{
			identifier = 13;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxDrop->isChecked())
		{
			identifier = 14;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxTrigger->isChecked())
		{
			identifier = 15;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxGrantOption->isChecked())
		{
			identifier = 16;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxCreateTempTables->isChecked())
		{
			identifier = 17;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		if (tab4CboxLockTables->isChecked())
		{
			identifier = 18;
			judgement = Set_Schema_Privileges(identifier, schema, userName, host);
		}
		QMessageBox::information(this, tr("Set Schemas Privileges"), tr("Set the Schema privileges successfully!"), QMessageBox::Ok);
	}
	else
	{
		qDebug("ERROR!");
	}
	Query_Schema_Slot();
	Unselect_All_Slot();
	tab4ComboBoxSchema->setEnabled(false);
	tab4CboxSelect->setEnabled(false);
	tab4CboxInsert->setEnabled(false);
	tab4CboxUpdate->setEnabled(false);
	tab4CboxDelete->setEnabled(false);
	tab4CboxExecute->setEnabled(false);
	tab4CboxShowView->setEnabled(false);
	tab4CboxCreate->setEnabled(false);
	tab4CboxAlter->setEnabled(false);
	tab4CboxReferences->setEnabled(false);
	tab4CboxIndex->setEnabled(false);
	tab4CboxCreateView->setEnabled(false);
	tab4CboxCreateRoutine->setEnabled(false);
	tab4CboxAlterRoutine->setEnabled(false);
	tab4CboxEvent->setEnabled(false);
	tab4CboxDrop->setEnabled(false);
	tab4CboxTrigger->setEnabled(false);
	tab4CboxGrantOption->setEnabled(false);
	tab4CboxCreateTempTables->setEnabled(false);
	tab4CboxLockTables->setEnabled(false);
	tab4PtnUnselectAll->setEnabled(false);
	tab4PtnSelctAll->setEnabled(false);
	tab4PtnDeleteEntry->setEnabled(false);
}
void Panel_UserPrivileges::Select_All_Slot()
{
	tab4CboxSelect->setCheckState(Qt::Checked);
	tab4CboxInsert->setCheckState(Qt::Checked);
	tab4CboxUpdate->setCheckState(Qt::Checked);
	tab4CboxDelete->setCheckState(Qt::Checked);
	tab4CboxExecute->setCheckState(Qt::Checked);
	tab4CboxShowView->setCheckState(Qt::Checked);
	tab4CboxCreate->setCheckState(Qt::Checked);
	tab4CboxAlter->setCheckState(Qt::Checked);
	tab4CboxReferences->setCheckState(Qt::Checked);
	tab4CboxIndex->setCheckState(Qt::Checked);
	tab4CboxCreateView->setCheckState(Qt::Checked);
	tab4CboxCreateRoutine->setCheckState(Qt::Checked);
	tab4CboxAlterRoutine->setCheckState(Qt::Checked);
	tab4CboxEvent->setCheckState(Qt::Checked);
	tab4CboxDrop->setCheckState(Qt::Checked);
	tab4CboxTrigger->setCheckState(Qt::Checked);
	tab4CboxGrantOption->setCheckState(Qt::Checked);
	tab4CboxCreateTempTables->setCheckState(Qt::Checked);
	tab4CboxLockTables->setCheckState(Qt::Checked);
}
void Panel_UserPrivileges::Unselect_All_Slot()
{
	tab4CboxSelect->setCheckState(Qt::Unchecked);
	tab4CboxInsert->setCheckState(Qt::Unchecked);
	tab4CboxUpdate->setCheckState(Qt::Unchecked);
	tab4CboxDelete->setCheckState(Qt::Unchecked);
	tab4CboxExecute->setCheckState(Qt::Unchecked);
	tab4CboxShowView->setCheckState(Qt::Unchecked);
	tab4CboxCreate->setCheckState(Qt::Unchecked);
	tab4CboxAlter->setCheckState(Qt::Unchecked);
	tab4CboxReferences->setCheckState(Qt::Unchecked);
	tab4CboxIndex->setCheckState(Qt::Unchecked);
	tab4CboxCreateView->setCheckState(Qt::Unchecked);
	tab4CboxCreateRoutine->setCheckState(Qt::Unchecked);
	tab4CboxAlterRoutine->setCheckState(Qt::Unchecked);
	tab4CboxEvent->setCheckState(Qt::Unchecked);
	tab4CboxDrop->setCheckState(Qt::Unchecked);
	tab4CboxTrigger->setCheckState(Qt::Unchecked);
	tab4CboxGrantOption->setCheckState(Qt::Unchecked);
	tab4CboxCreateTempTables->setCheckState(Qt::Unchecked);
	tab4CboxLockTables->setCheckState(Qt::Unchecked);
}
void Panel_UserPrivileges::Delete_Entry_Slot()
{
	int currentRow = tableView->currentIndex().row(); //获取当前行
	int currentRow1 = tab4TableView->currentIndex().row();
	QString judgement;
	QStringList userHost;
	QStringList schemaList;
	int i = 0;
	int numberOfColumns = 0;
	int numberOfRows = 0; //行数
	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		QString userName = userHost.at(2 * currentRow);
		QString host = userHost.at(2 * currentRow + 1);
		judgement = Query_Schema(schemaList, userName, host, &numberOfRows);
		QString schema = schemaList.at(currentRow1);
		judgement = Delete_Entry(userName, host, schema);
		if (judgement == "OK")
		{
			QMessageBox::information(this, tr("Delete entry:"), tr("Delete entry successfully!"), QMessageBox::Ok);
		}
	}
	else
	{
		qDebug("ERROR!");
	}
	Query_Schema_Slot();
}
void Panel_UserPrivileges::Revoke_Schema_Privileges_Slot()
{
	int currentRow = tableView->currentIndex().row(); //获取当前行
	QString judgement;
	QStringList userHost;
	int i = 0;
	int numberOfColumns = 0;
	int numberOfRows = 0; //行数
	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);
	if (judgement == "OK")
	{
		QString userName = userHost.at(2 * currentRow);
		QString host = userHost.at(2 * currentRow + 1);
		switch (QMessageBox::warning(this, tr("Revoke All Privileges"), tr("Please confirm revokation of all privileges!"), QMessageBox::Ok | QMessageBox::Cancel))
		{
		case QMessageBox::Ok:
			judgement = Revoke_Schema_Privileges(userName, host);
			if (judgement == "OK")
			{
				QMessageBox::information(this, tr("Revoke All Privileges"), tr("Revoke all privileges successfully!"), QMessageBox::Ok);
			}
			break;
		case QMessageBox::Cancel:
			QMessageBox::information(this, tr("Revoke All Privileges"), tr("You have already cancelled cancel the user permissions."), QMessageBox::Ok);
			break;
		default:
			break;
		}
	}
	else
	{
		qDebug("ERROR!");
	}
	Query_Schema_Slot();
}
void Panel_UserPrivileges::Show_Schema_Privileges_Slot()
{
	tab4PtnDeleteEntry->setEnabled(true);
	tab4CboxSelect->setEnabled(true);
	tab4CboxInsert->setEnabled(true);
	tab4CboxUpdate->setEnabled(true);
	tab4CboxDelete->setEnabled(true);
	tab4CboxExecute->setEnabled(true);
	tab4CboxShowView->setEnabled(true);
	tab4CboxCreate->setEnabled(true);
	tab4CboxAlter->setEnabled(true);
	tab4CboxReferences->setEnabled(true);
	tab4CboxIndex->setEnabled(true);
	tab4CboxCreateView->setEnabled(true);
	tab4CboxCreateRoutine->setEnabled(true);
	tab4CboxAlterRoutine->setEnabled(true);
	tab4CboxEvent->setEnabled(true);
	tab4CboxDrop->setEnabled(true);
	tab4CboxTrigger->setEnabled(true);
	tab4CboxGrantOption->setEnabled(true);
	tab4CboxCreateTempTables->setEnabled(true);
	tab4CboxLockTables->setEnabled(true);

	int currentRow = tableView->currentIndex().row(); //获取当前行
	int currentRow1 = tab4TableView->currentIndex().row();
	QString  judgement;
	QStringList userHost; //获取User和host
	QStringList schemaPriv; //获取所选Schema的所有权限
	QStringList schemaList; //获取当前用户所有Shema
	int numberOfColumns = 0;
	int numberOfRows = 0;
	judgement = Query_User_Host(userHost, &numberOfColumns, &numberOfRows);

	if (judgement == "OK")
	{
		QString userName = userHost.at(2 * currentRow);
		QString host = userHost.at(2 * currentRow + 1);
		judgement = Query_Schema(schemaList, userName, host, &numberOfRows);
		QString schema = schemaList.at(currentRow1);
		judgement = Query_Shema_Variables(userName, host, schema);
		if (judgement == "OK")
		{
			judgement = Show_Schema_Privileges(schemaPriv);
			if (judgement == "OK")
			{
				QString selectPriv = schemaPriv.at(0);
				QString insertPriv = schemaPriv.at(1);
				QString updatePriv = schemaPriv.at(2);
				QString deletePriv = schemaPriv.at(3);
				QString createPriv = schemaPriv.at(4);
				QString dropPriv = schemaPriv.at(5);
				QString grantPriv = schemaPriv.at(6);
				QString referencesPriv = schemaPriv.at(7);
				QString indexPriv = schemaPriv.at(8);
				QString alterPriv = schemaPriv.at(9);
				QString createTmpTablePriv = schemaPriv.at(10);
				QString lockTablesPriv = schemaPriv.at(11);
				QString createViewPriv = schemaPriv.at(12);
				QString showViewPriv = schemaPriv.at(13);
				QString createRoutinePriv = schemaPriv.at(14);
				QString alterRoutinePriv = schemaPriv.at(15);
				QString executePriv = schemaPriv.at(16);
				QString eventPriv = schemaPriv.at(17);
				QString triggerPriv = schemaPriv.at(18);
				Unselect_All_Slot();
				if (selectPriv == "Y")
				{
					tab4CboxSelect->setCheckState(Qt::Checked);
				}
				if (insertPriv == "Y")
				{
					tab4CboxInsert->setCheckState(Qt::Checked);
				}
				if (updatePriv == "Y")
				{
					tab4CboxUpdate->setCheckState(Qt::Checked);
				}
				if (deletePriv == "Y")
				{
					tab4CboxDelete->setCheckState(Qt::Checked);
				}
				if (createPriv == "Y")
				{
					tab4CboxCreate->setCheckState(Qt::Checked);
				}
				if (dropPriv == "Y")
				{
					tab4CboxDrop->setCheckState(Qt::Checked);
				}
				if (grantPriv == "Y")
				{
					tab4CboxGrantOption->setCheckState(Qt::Checked);
				}
				if (referencesPriv == "Y")
				{
					tab4CboxReferences->setCheckState(Qt::Checked);
				}
				if (indexPriv == "Y")
				{
					tab4CboxIndex->setCheckState(Qt::Checked);
				}
				if (alterPriv == "Y")
				{
					tab4CboxAlter->setCheckState(Qt::Checked);
				}
				if (createTmpTablePriv == "Y")
				{
					tab4CboxCreateTempTables->setCheckState(Qt::Checked);
				}
				if (lockTablesPriv == "Y")
				{
					tab4CboxLockTables->setCheckState(Qt::Checked);
				}
				if (createViewPriv == "Y")
				{
					tab4CboxCreateView->setCheckState(Qt::Checked);
				}
				if (showViewPriv == "Y")
				{
					tab4CboxShowView->setCheckState(Qt::Checked);
				}
				if (createRoutinePriv == "Y")
				{
					tab4CboxCreateRoutine->setCheckState(Qt::Checked);
				}
				if (alterRoutinePriv == "Y")
				{
					tab4CboxAlterRoutine->setCheckState(Qt::Checked);
				}
				if (executePriv == "Y")
				{
					tab4CboxExecute->setCheckState(Qt::Checked);
				}
				if (eventPriv == "Y")
				{
					tab4CboxEvent->setCheckState(Qt::Checked);
				}
				if (triggerPriv == "Y")
				{
					tab4CboxTrigger->setCheckState(Qt::Checked);
				}
			}
		}
	}
	else
	{
		qDebug("ERROR!");
	}
}
void Panel_UserPrivileges::Revert_Schema_Slot()
{
	Unselect_All_Slot();
	tab4CboxSelect->setEnabled(false);
	tab4CboxInsert->setEnabled(false);
	tab4CboxUpdate->setEnabled(false);
	tab4CboxDelete->setEnabled(false);
	tab4CboxExecute->setEnabled(false);
	tab4CboxShowView->setEnabled(false);
	tab4CboxCreate->setEnabled(false);
	tab4CboxAlter->setEnabled(false);
	tab4CboxReferences->setEnabled(false);
	tab4CboxIndex->setEnabled(false);
	tab4CboxCreateView->setEnabled(false);
	tab4CboxCreateRoutine->setEnabled(false);
	tab4CboxAlterRoutine->setEnabled(false);
	tab4CboxEvent->setEnabled(false);
	tab4CboxDrop->setEnabled(false);
	tab4CboxTrigger->setEnabled(false);
	tab4CboxGrantOption->setEnabled(false);
	tab4CboxCreateTempTables->setEnabled(false);
	tab4CboxLockTables->setEnabled(false);
	tab4PtnUnselectAll->setEnabled(false);
	tab4PtnSelctAll->setEnabled(false);

}
