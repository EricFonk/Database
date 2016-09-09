#include "ImportPart.h"
#include "qapplication.h"
#include "qstring.h"
#include "qmessagebox.h"
//#include "qsqldatabase.h"
//#include "qsqlquerymodel.h"
//#include "qstandarditemmodel"
//#include "qsqlquery.h"
//#include "qsqlrecord.h"
//#include "qsqlrelationaltablemodel"
//#include "winsock.h"
#include "iostream"
#include "stdio.h"
#include "string.h"
#include "mysql.h"
#include "qfile.h"
#include "qvector.h"
#include "qfiledialog.h"
#include "stdlib.h"
#include "qtextstream.h"
#include "export_and_import.h"
#include "qobject.h"

#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")

QString ImportPassword = "63026302";
MYSQL Import_Mysql;
ImportPart::ImportPart()
{
	//导入地址
	FirstLayout = new QHBoxLayout();
	Label_FilePath = new QLabel(tr("File Path:"));
	Label_FilePath->setStyleSheet("font:bold;font-size:12px");//字体加粗
	LineEdit_FilePath = new QLineEdit();
	LineEdit_FilePath->setFixedWidth(700);
	PushButton_FileChosen = new QPushButton(tr("..."));
	PushButton_FileChosen->setStyleSheet("font:bold");
	FirstLayout->addWidget(Label_FilePath);
	FirstLayout->addWidget(LineEdit_FilePath);
	FirstLayout->addWidget(PushButton_FileChosen);
	FirstLayout->addStretch(1);
	FirstLayout->setMargin(10);

	//导入数据库地址
	
	SecondLayout = new QHBoxLayout();
	Label_ImportDes = new QLabel(tr("Import Destination:"));
	Label_ImportDes->setStyleSheet("font:bold;font-size:12px");
	ComboBox_ImportDes = new QComboBox();
	ComboBox_ImportDes->addItem(" ");
	ComboBox_ImportDes->setFixedWidth(500);
	Label_NewImprtDes = new QLabel(tr("New Import Destination:"));
	Label_NewImprtDes->setStyleSheet("font:bold;font-size:12px");
	LineEdit_NewImportDes = new QLineEdit();
	LineEdit_NewImportDes->setFixedWidth(250);
	SecondLayout->addWidget(Label_ImportDes);
	SecondLayout->addWidget(ComboBox_ImportDes);
	SecondLayout->addStretch(2);
	SecondLayout->addWidget(Label_NewImprtDes);
	SecondLayout->addWidget(LineEdit_NewImportDes);
	SecondLayout->setMargin(10);

	//提示以及显示情况部分
	ThirdLayout = new QVBoxLayout();
	Label_LOG = new QLabel("LOG:");
	Label_LOG->setStyleSheet("font-size:12px");
	TableView_Show = new QTableView();
	ThirdLayout->addWidget(Label_LOG);
	ThirdLayout->addWidget(TableView_Show);
	ThirdLayout->setMargin(10);

	//导入按钮
	FourthLayout = new QHBoxLayout();
	Label_PromotStatement = new QLabel(tr("Press [Start Import] to Start..."));
	PushButton_StartImport = new QPushButton(tr("Start Import"));
	FourthLayout->addWidget(Label_PromotStatement);
	FourthLayout->addStretch(5);
	FourthLayout->addWidget(PushButton_StartImport);
	FourthLayout->setMargin(10);

	//导入整体
	ImportLayout = new QVBoxLayout();
	WholeImport = new QWidget();
	ImportLayout->addLayout(FirstLayout);
	ImportLayout->addLayout(SecondLayout);
	ImportLayout->addLayout(ThirdLayout);
	ImportLayout->addLayout(FourthLayout);
	WholeImport->setLayout(ImportLayout);

	ComboBoxImportDesInit();

	connect(ComboBox_ImportDes, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(SetImportDes()));
	connect(PushButton_FileChosen, SIGNAL(clicked()), this, SLOT(ImportPath()));
	connect(PushButton_StartImport, SIGNAL(clicked()), this, SLOT(Import()));
}
bool ImportPart::ImportConnectSQL()
{
	mysql_init(&Import_Mysql);
	if (!mysql_real_connect(&Import_Mysql, "localhost", "root", ImportPassword.toStdString().c_str(), "", 3306, NULL, 0))
	{
		QMessageBox::warning(this, tr("Warning!"), mysql_error(&Import_Mysql), QMessageBox::Ok);
		return false;
	}
	return true;
}
void ImportPart::ComboBoxImportDesInit()
{
	MYSQL_RES *result;
	int res;
	if (ImportConnectSQL() == false)
	{
	}
	else
	{
		res = mysql_query(&Import_Mysql, "show databases; ");
		if (!res)
		{
			result = mysql_store_result(&Import_Mysql);
			if (result)
			{
				MYSQL_ROW row;
				int databasenum;
				databasenum = mysql_affected_rows(&Import_Mysql);
				for (int i = 0; i < databasenum; i++)
				{
					row = mysql_fetch_row(result);
					if (row != NULL)
					{
						ComboBox_ImportDes->addItem(row[0]);
					}
				}
			}
		}
	}
	mysql_free_result(result);
	mysql_close(&Import_Mysql);
}
void ImportPart::ImportPath()
{
	QString file_full, file_name, file_path, file_suffix;
	QFileInfo fi;
	file_full = QFileDialog::getOpenFileName(this);
	fi = QFileInfo(file_full);
	file_name = fi.fileName();
	file_path = fi.absolutePath();
	file_suffix = fi.suffix();
	if (file_suffix == ("sql"))
	{
		LineEdit_FilePath->setText(file_path + '/' + file_name);
	}
	else
	{
		QMessageBox::warning(this, tr("Fault"), "Please select  .sql file", QMessageBox::Ok);
		LineEdit_FilePath->setText(" ");
	}
}
void ImportPart::SetImportDes()
{
	if (ComboBox_ImportDes->currentText() == " ")
	{
		LineEdit_NewImportDes->setEnabled(true);
	}
	else
	{
		LineEdit_NewImportDes->setEnabled(false);
	}
}

void ImportPart::Import()
{
	QString CreateDatabaseSql;
	QString UseDatabaseSql;
	QString Sql;
	QString ImportDatabase;
	int res;
	bool flag = true;
	int pos = 0;
	QStringList log_head;
	int  i = 0;
	ImportDatabase = ComboBox_ImportDes->currentText();
	log_head << "SQL Statement" << "Result" << "Remark";
	QStandardItemModel *log = new QStandardItemModel();
	TableView_Show->setModel(log);
	log->setHorizontalHeaderLabels(log_head);
	if (ImportConnectSQL() == true)
	{
		if (ComboBox_ImportDes->currentText() == "")
		{
			ImportDatabase = LineEdit_NewImportDes->text();
			UseDatabaseSql = "use " + ImportDatabase + " ; ";
			CreateDatabaseSql = "create database " + ImportDatabase + " ; ";
			mysql_query(&Import_Mysql, CreateDatabaseSql.toStdString().c_str());
			mysql_query(&Import_Mysql, UseDatabaseSql.toStdString().c_str());
			LineEdit_NewImportDes->setText("");
		}
		else
		{
			UseDatabaseSql = "use " + ImportDatabase + " ; ";
			mysql_query(&Import_Mysql, UseDatabaseSql.toStdString().c_str());
		}
		QFile file(LineEdit_FilePath->text());
		if (!file.open(QIODevice::ReadOnly)) {
			return;
		}
		int i = 0;
		while (!file.atEnd()) {
			QString lineString = QString(file.readLine()).trimmed();
			if (lineString.startsWith(" "))
			{
			}
			else if (lineString.startsWith("--"))
			{
			}
			else
			{
				pos = lineString.indexOf(";");
				Sql.append(lineString);
				if (pos != 0 && pos != -1)
				{
					mysql_query(&Import_Mysql, "set names gbk");
					if (mysql_query(&Import_Mysql, Sql.toStdString().c_str())) {
						log->setItem(i, 0, new QStandardItem(Sql));
						log->setItem(i, 1, new QStandardItem("Failed!"));
						log->setItem(i, 2, new QStandardItem(mysql_error(&Import_Mysql)));
						flag = false;
						Sql = "";
						i++;
					}
					else
					{
						log->setItem(i, 0, new QStandardItem(Sql));
						log->setItem(i, 1, new QStandardItem("Successful!"));
						log->setItem(i, 2, new QStandardItem(" "));
						Sql = "";
						i++;
					}
					pos = 0;
				}
			}
		}
		file.close();
		TableView_Show->setColumnWidth(0, 500);
		TableView_Show->setColumnWidth(1, 100);
		TableView_Show->setColumnWidth(2, 431);
		mysql_close(&Import_Mysql);//断开连接
		if (flag == true)
		{
			QMessageBox::information(this, tr("OK!!"), "Import Successful!", QMessageBox::Ok);
		}
		else
		{
			QMessageBox::information(this, tr("Fault"), "Import Fail!", QMessageBox::Ok);
			PushButton_StartImport->setText("Start Again");
		}
	}
}
