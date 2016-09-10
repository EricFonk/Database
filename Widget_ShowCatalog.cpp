#include "Widget_ShowCatalog.h"
#include "mysqlQuery.h"

Widget_ShowCatalog::Widget_ShowCatalog()
{
	this->tableShowLog = new QTableWidget(this);
	this->tableHead << "ExecuteStatus" << "ExecuteInfo" << "ExecuteTime" << "AffectedRows" << "SqlQuery";
	this->tableShowLog->setColumnCount(5);
	this->tableShowLog->setRowCount(0);
	this->tableShowLog->setHorizontalHeaderLabels(this->tableHead);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(this->tableShowLog);
	this->setLayout(mainLayout);
}

void Widget_ShowCatalog::AddOneLogInfo(QString pISqlQuery,float pIExecuteTime,long pIAffectedRow,bool pIExecuteFlag)
{
	int currentRows;
	QString exeTime,affectedRows;
	exeTime = QString::number(pIExecuteTime) + "ms";
	affectedRows = QString::number(pIAffectedRow) + " rows";
	currentRows = this->tableShowLog->rowCount();
	this->tableShowLog->setRowCount(currentRows+1);
	this->tableShowLog->setItem(currentRows, 2, new QTableWidgetItem(exeTime));
	this->tableShowLog->setItem(currentRows, 3, new QTableWidgetItem(affectedRows));
	this->tableShowLog->setItem(currentRows, 4, new QTableWidgetItem(pISqlQuery));
	if (pIExecuteFlag)
	{
		this->tableShowLog->setItem(currentRows, 0, new QTableWidgetItem("Execute Success!"));
		this->tableShowLog->setItem(currentRows, 1, new QTableWidgetItem("All right"));
	}
	else
	{
		this->tableShowLog->setItem(currentRows, 0, new QTableWidgetItem("Execute Failed!"));
		this->tableShowLog->setItem(currentRows, 1, new QTableWidgetItem(GetErrorInfo()));
	}
}

void Widget_ShowCatalog::RefreshLog()
{
	this->tableShowLog->clear();
}

Widget_ShowCatalog::~Widget_ShowCatalog()
{

}