#include "ExportPart.h"
#include "QtWidgets\qapplication.h"
#include "qstring.h"
#include "qmessagebox.h"
#include "qstandarditemmodel"
#include "winsock.h"
#include "iostream"
#include "stdio.h"
#include "string.h"
#include "mysql.h"
#include "qfile.h"
#include "qvector.h"
#include "qfiledialog.h"
#include "stdlib.h"
#include "qtextstream.h"
#include "mysqlQuery.h"

#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")

QString SelectDatabaseName;
QString ExportContent;
database AllData[100];
database ExportData[100];
QString ExportPassword = "123456";
MYSQL Export_Mysql;
ExportPart::ExportPart()
{
	WholeExport = new QWidget();
	WholeWindow = new QVBoxLayout();
	//显示数据库部分
	QStringList exporttype;
	exporttype << "Dump Structure and Data" << "Dump Data Only" << "Dump Structure Only";
	TopLayout_1 = new QHBoxLayout();
	TopLayout_2 = new QHBoxLayout();
	TopLayout = new QVBoxLayout();
	TableWidget_ShowDatabases = new QTableWidget();
	TableWidget_ShowDatabases->setShowGrid(false);
	TableWidget_ShowDatabases->verticalHeader()->setVisible(false);
	TableWidget_ShowDatabases->setEditTriggers(QTableWidget::NoEditTriggers);//不能编辑
	TableWidget_ShowDatabases->setSelectionMode(QAbstractItemView::SingleSelection);
	TableWidget_ShowDatabases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	TableWidget_ShowTables = new QTableWidget();
	TableWidget_ShowTables->setShowGrid(false);
	TableWidget_ShowTables->verticalHeader()->setVisible(false);
	TableWidget_ShowTables->setEditTriggers(QTableWidget::NoEditTriggers);//不能编辑
	TableWidget_ShowTables->setSelectionMode(QAbstractItemView::SingleSelection);
	TableWidget_ShowTables->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	GroupBox_TablesToExport = new QGroupBox(tr("Tables to Export"));
	PushBtn_Refresh = new QPushButton(tr("Refresh"));
	PushBtn_SelectTable = new QPushButton(tr("Select Table"));
	PushBtn_SelectTable->setEnabled(false);
	PushBtn_UnselectAll = new QPushButton(tr("Unselect All"));
	PushBtn_UnselectAll->setEnabled(false);
	ComboBox_ExportType = new QComboBox();
	ComboBox_ExportType->addItems(exporttype);
	TopLayout_1->addWidget(TableWidget_ShowDatabases);
	TopLayout_1->setMargin(10);
	TopLayout_1->addWidget(TableWidget_ShowTables);
	TopLayout_2->addWidget(PushBtn_Refresh);
	TopLayout_2->addStretch(8);
	TopLayout_2->addWidget(ComboBox_ExportType);
	TopLayout_2->addStretch(2);
	TopLayout_2->addWidget(PushBtn_SelectTable);
	TopLayout_2->addWidget(PushBtn_UnselectAll);
	TopLayout->addLayout(TopLayout_1);
	TopLayout->addLayout(TopLayout_2);
	GroupBox_TablesToExport->setLayout(TopLayout);

	//导出对象部分
	GroupBox_ObjectToExport = new QGroupBox(tr("Objects to Export"));
	MidLayout = new QHBoxLayout();
	CheckBox_ExportSet1 = new QCheckBox(tr("Dump Stored procedures and Functions"));
	CheckBox_ExportSet2 = new QCheckBox(tr("Dump Events"));
	CheckBox_ExportSet3 = new QCheckBox(tr("Dump Trigger"));
	MidLayout->addWidget(CheckBox_ExportSet1);
	MidLayout->addWidget(CheckBox_ExportSet2);
	MidLayout->addWidget(CheckBox_ExportSet3);
	GroupBox_ObjectToExport->setLayout(MidLayout);

	//导出设置部分
	GroupBox_ExportOption = new QGroupBox(tr("Export Option"));
	BottomLayout = new QHBoxLayout();
	Label_ExportFolder = new QLabel(tr("Export to Dump Project Folder"));
	LineEdit_ExportPath = new QLineEdit();
	//LineEdit_ExportPath->setFixedWidth(700);
	PushBtn_FileOpen = new QPushButton("...");
	BottomLayout->addWidget(Label_ExportFolder);
	BottomLayout->addWidget(LineEdit_ExportPath);
	BottomLayout->addStretch(1);
	BottomLayout->addWidget(PushBtn_FileOpen);
	BottomLayout->addStretch(3);
	GroupBox_ExportOption->setLayout(BottomLayout);

	//其他部分
	Label_PromptStatement = new QLabel(tr("Press [Start Export] to Start..."));
	PushBtn_StartExport = new QPushButton(tr("Start Export"));
	OtherLayout = new QHBoxLayout();
	OtherLayout->addWidget(Label_PromptStatement);
	OtherLayout->addStretch(5);
	OtherLayout->addWidget(PushBtn_StartExport);


	WholeWindow->addWidget(GroupBox_TablesToExport, 7);
	WholeWindow->addSpacing(1);
	WholeWindow->addWidget(GroupBox_ObjectToExport, 1);
	WholeWindow->addSpacing(1);
	WholeWindow->addWidget(GroupBox_ExportOption, 1);
	WholeWindow->addSpacing(1);
	WholeWindow->addLayout(OtherLayout, 1);
	WholeExport->setLayout(WholeWindow);

	ConnectDatabase();
	TableWidgetShowDatabasesInit();

	connect(PushBtn_Refresh, SIGNAL(clicked()), this, SLOT(Refresh()));
	connect(PushBtn_FileOpen, SIGNAL(clicked()), this, SLOT(SavePath()));
	connect(TableWidget_ShowDatabases, SIGNAL(cellClicked(int,int)), this, SLOT(GetTable(int,int)));
	connect(TableWidget_ShowTables, SIGNAL(cellChanged(int, int)), this, SLOT(GetSelectTable(int, int)));
	connect(PushBtn_StartExport, SIGNAL(clicked()), this, SLOT(Export()));
	connect(PushBtn_SelectTable, SIGNAL(clicked()), this, SLOT(Select_AllTable()));
	connect(PushBtn_UnselectAll, SIGNAL(clicked()), this, SLOT(UnSelect_AllTable()));
}

bool ExportPart::ExportConnectSQL()
{
	/*mysql_init(&Export_Mysql);
	if (!mysql_real_connect(&Export_Mysql, "localhost", "root", ExportPassword.toStdString().c_str(), "", 3306, NULL, 0))
	{
		QMessageBox::warning(this, tr("Warning!"), mysql_error(&Export_Mysql), QMessageBox::Ok);
		return false;
	}
	return true;*/
	return true;
}
void ExportPart::TableWidgetShowDatabasesInit()
{
	QStringList databasename;
	GetAllDatabases(databasename);
	QStringList header;
	header << "Database";
	TableWidget_ShowDatabases->setRowCount(databasename.size());
	TableWidget_ShowDatabases->setColumnCount(1);
	TableWidget_ShowDatabases->setHorizontalHeaderLabels(header);
	for (int i = 0; i < databasename.size(); i++)
	{
		QString s;//s是每个数据库的名字
		QTableWidgetItem *checkBox = new QTableWidgetItem();
		checkBox->setCheckState(Qt::Unchecked);
		TableWidget_ShowDatabases->setItem(i, 0, checkBox);
		checkBox->setText(databasename.at(i));
		s = databasename.at(i);
		AllData[i].databasename = s;
		ExportData[i].databasename = s;
	}
}
void ExportPart::Refresh()
{
	TableWidget_ShowDatabases->clear();
	TableWidget_ShowTables->clear();
	TableWidgetShowDatabasesInit();
}
void ExportPart::Select_AllTable()
{
	for (int i = 0; i < TableWidget_ShowTables->rowCount(); i++)
	{
		TableWidget_ShowTables->item(i, 0)->setCheckState(Qt::Checked);
	}
}
void ExportPart::UnSelect_AllTable()
{
	for (int i = 0; i < TableWidget_ShowTables->rowCount(); i++)
	{
		TableWidget_ShowTables->item(i, 0)->setCheckState(Qt::Unchecked);
	}
}
void ExportPart::SavePath()
{
	QString path;
	QFileDialog *fileDialog = new QFileDialog(this);//创建一个QFileDialog对象，构造函数中的参数可以有所添加。
	fileDialog->setWindowTitle(tr("Export To:"));//设置文件保存对话框的标题
	fileDialog->setAcceptMode(QFileDialog::AcceptSave);//设置文件对话框为保存模式
	fileDialog->setFileMode(QFileDialog::AnyFile);//设置文件对话框弹出的时候显示任何文件，不论是文件夹还是文件
	fileDialog->setViewMode(QFileDialog::Detail);//文件以详细的形式显示，显示文件名，大小，创建日期等信息；
	fileDialog->setGeometry(10, 30, 300, 200);//设置文件对话框的显示位置
	fileDialog->setDirectory(".");//设置文件对话框打开时初始打开的位置
	if (fileDialog->exec() == QFileDialog::Accepted) {//注意使用的是QFileDialog::Accepted或者QDialog::Accepted,不是QFileDialog::Accept
		path = fileDialog->selectedFiles()[0];//得到用户选择的文件名
	}
	LineEdit_ExportPath->setText(path);
}
void ExportPart::GetTable(int row, int col)
{
	QString DatabaseName = AllData[row].databasename;
	int tablenum;
	QStringList OneDatabaseAllTable;
	GetAllTables(OneDatabaseAllTable, DatabaseName);
	tablenum = OneDatabaseAllTable.size();
	TableWidget_ShowTables->setRowCount(tablenum);
	TableWidget_ShowTables->setColumnCount(1);
	QStringList header;
	header << DatabaseName;
	TableWidget_ShowTables->setHorizontalHeaderLabels(header);
	for (int i = 0; i < tablenum; i++)
	{
		QTableWidgetItem *checkBox = new QTableWidgetItem();
		if (TableWidget_ShowDatabases->item(row, col)->checkState() == Qt::Checked)
		{
			checkBox->setCheckState(Qt::Checked);
			ExportData[row].SelectStatus = true;
		}
		else
		{
			checkBox->setCheckState(Qt::Unchecked);
			ExportData[row].SelectStatus = false;
			ExportData[row].table.clear();
		}
		TableWidget_ShowTables->setItem(i, 0, checkBox);
		checkBox->setText(OneDatabaseAllTable.at(i));
		AllData[row].table.append(OneDatabaseAllTable.at(i));
		QString tablename = TableWidget_ShowTables->item(i, 0)->text().trimmed();
		int tablepos = ExportData[row].table.indexOf(tablename);
		if (tablepos == -1)
		{
			ExportData[row].table.append(OneDatabaseAllTable.at(i));
		}
	}
	if (TableWidget_ShowTables->rowCount() != 0)
	{
		PushBtn_SelectTable->setEnabled(true);
		PushBtn_UnselectAll->setEnabled(true);
	}
	else
	{
		PushBtn_SelectTable->setEnabled(false);
		PushBtn_UnselectAll->setEnabled(false);
	}
	/*
	QString DatabaseName = AllData[row].databasename;
	QString query = "show tables from " + DatabaseName + "; ";
	MYSQL_RES *result;
	int res;
	if (ExportConnectSQL() == false)
	{
	}
	else
	{
		res = mysql_query(&Export_Mysql, query.toStdString().c_str());
		if (!res)
		{
			result = mysql_store_result(&Export_Mysql);
			if (result)
			{
				MYSQL_ROW row1;
				int tablenum;
				tablenum = mysql_affected_rows(&Export_Mysql);
				TableWidget_ShowTables->setRowCount(tablenum);
				TableWidget_ShowTables->setColumnCount(1);
				QStringList header;
				header << DatabaseName;
				TableWidget_ShowTables->setHorizontalHeaderLabels(header);
				for (int i = 0; i < tablenum; i++)
				{
					row1 = mysql_fetch_row(result);
					if (row1 != NULL)
					{
						QTableWidgetItem *checkBox = new QTableWidgetItem();
						if (TableWidget_ShowDatabases->item(row, col)->checkState() == Qt::Checked)
						{
							checkBox->setCheckState(Qt::Checked);
							ExportData[row].SelectStatus = true;
						}
						else
						{
							checkBox->setCheckState(Qt::Unchecked);
							ExportData[row].SelectStatus = false;
							ExportData[row].table.clear();
						}
						TableWidget_ShowTables->setItem(i, 0, checkBox);
						checkBox->setText(row1[0]);
						AllData[row].table.append(row1[0]);
						QString tablename = TableWidget_ShowTables->item(i, 0)->text().trimmed();
						int tablepos = ExportData[row].table.indexOf(tablename);
						if (tablepos == -1)
						{
							ExportData[row].table.append(row1[0]);
						}
					}
				}
			}
		}
	}
	if (TableWidget_ShowTables->rowCount() != 0)
	{
		PushBtn_SelectTable->setEnabled(true);
		PushBtn_UnselectAll->setEnabled(true);
	}
	else
	{
		PushBtn_SelectTable->setEnabled(false);
		PushBtn_UnselectAll->setEnabled(false);
	}
	mysql_free_result(result);
	mysql_close(&Export_Mysql);*/
}
void ExportPart::GetSelectTable(int row, int col)
{
	int databasepos;
	QString databasename = TableWidget_ShowTables->horizontalHeaderItem(0)->text();
	QString tablename;
	for (databasepos = 0; databasename != AllData[databasepos].databasename; databasepos++)
	{
	}
	tablename = TableWidget_ShowTables->item(row, col)->text().trimmed();
	int tablepos = ExportData[databasepos].table.indexOf(tablename);
	if (TableWidget_ShowTables->item(row, col)->checkState() == Qt::Checked)
	{
		TableWidget_ShowDatabases->item(databasepos, col)->setCheckState(Qt::Checked);
		ExportData[databasepos].SelectStatus = true;
		if (tablepos == -1)
		{
			ExportData[databasepos].table.append(tablename);
		}
		else
		{
		}
	}
	else
	{
		
		if (tablepos == -1)
		{
		}
		else
		{
			ExportData[databasepos].table.removeAt(tablepos);
		}
	}
	if (ExportData[databasepos].table.size() == 0)
	{
		ExportData[databasepos].SelectStatus = false;
	}
}
void ExportPart::FileCopy(QString TempFile, QString FinalFile)
{
	QFile Temp(TempFile);
	QFile File(FinalFile);
	Temp.open(QIODevice::ReadOnly);
	File.open(QIODevice::Append);
	QTextStream tempfile(&Temp);
	QTextStream file(&File);
	file << tempfile.readAll();
	Temp.close();
	File.close();
}
QString ExportPart::GetType()
{
	int i;
	QString str;
	i = ComboBox_ExportType->currentIndex();
	if (i == 0)
		str = "";
	else if (i == 1)
		str = "-t ";
	else
		str = "--opt -d ";
	str.append("-uroot --default-character-set=gbk -p" + ExportPassword + " ");
	if (CheckBox_ExportSet1->isChecked())
	{
		str.append("-ntd -R ");
	}
	if (CheckBox_ExportSet2->isChecked())
	{
		str.append("-E ");
	}
	if (CheckBox_ExportSet3->isChecked())
	{
		str.append("-triggers ");
	}
	return str;
}
void ExportPart::Export()
{
	QString MysqlPos,ExportType;
	int flag = 0;
	int i = 0;
	int m = 0;
	ExportType = GetType();
	/*if (ExportConnectSQL() == true)
	{
		char sql[100] = "select @@basedir as basePath from dual";
		mysql_query(&Export_Mysql, sql);
		res = mysql_store_result(&Export_Mysql);
		while (row = mysql_fetch_row(res))
		{
			MysqlPos = row[0];
		}
		mysql_free_result(res);
		mysql_close(&Export_Mysql);
	}*/
	MysqlPos=GetMysqlPath();
	while (ExportData[m].databasename != "")
	{
		if (ExportData[m].SelectStatus == false)
		{
			m++;
		}
		else
		{
			ExportContent.append("cmd.exe /c \"" + MysqlPos + "\\bin\\mysqldump.exe\" " + ExportType + ExportData[m].databasename + " ");
			for (int k = 0; k < ExportData[m].table.count(); k++)
			{
				ExportContent.append(ExportData[m].table.at(k) + " ");
			}
			if (i == 0)
			{
				ExportContent.append("> " + LineEdit_ExportPath->text());
				flag = WinExec(ExportContent.toStdString().c_str(), SW_HIDE);
				if (flag < 32)
				{ 
					break;
				}
				ExportContent.clear();
				i++;
			}
			else
			{
				ExportContent.append("> Temp.sql");
				flag  = WinExec(ExportContent.toStdString().c_str(), SW_HIDE);
				if (flag < 32)
				{
					break;
				}
				FileCopy("Temp.sql", LineEdit_ExportPath->text());
				ExportContent.clear();
			}
			m++;
		}
	}
	if (flag < 32)
	{
		QMessageBox::warning(this, tr("Warning!"),"Export "+ExportData[m].databasename+" Fault!", QMessageBox::Ok);
	}
	else
	{
		QMessageBox::information(this, tr("OK!"), "Successful!", QMessageBox::Ok);
	}
}

