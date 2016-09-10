#include "Widget_ForeignKey.h"
#include "mysqlQuery.h"

Widget_ForeignKey::Widget_ForeignKey()
{
	_fkSum = 0;
	_onInsertFlag = 0;

	QPushButton *addFk = new QPushButton("Add one Fk");
	QPushButton *dropFk = new QPushButton("Drop one Fk");
	QPushButton *applyInsert = new QPushButton("Finish Adding Fk");
	QPushButton *applyAlter = new QPushButton("Apply FK Alter");

	QStringList refTable, refColumn;
	refTable << "Foreign Key Name" << "Referenced Table";
	refColumn << "Col" << "Referenced Column";
	_tableManageRefTable = new QTableWidget(this);//选择外键表
	_tableManageRefColumn = new QTableWidget(this);//选择外键列
	
	_tableManageRefTable->setColumnCount(2);
	_tableManageRefTable->setRowCount(0);
	_tableManageRefTable->setHorizontalHeaderLabels(refTable);

	_tableManageRefColumn->setColumnCount(2);
	_tableManageRefColumn->setRowCount(0);
	_tableManageRefColumn->setHorizontalHeaderLabels(refColumn);
	QHBoxLayout *mainLayout = new QHBoxLayout(this);

	mainLayout->addWidget(_tableManageRefTable);
	mainLayout->addWidget(_tableManageRefColumn);
	mainLayout->addWidget(addFk);
	mainLayout->addWidget(dropFk);
	mainLayout->addWidget(applyAlter);
	mainLayout->addWidget(applyInsert);
	this->setLayout(mainLayout);

	connect(addFk, SIGNAL(clicked()),this,SLOT(AddNewFK()));
	connect(dropFk, SIGNAL(clicked()),this,SLOT(DropOldFK()));
	connect(applyAlter, SIGNAL(clicked()),this,SLOT(ApplyAlterFK()));
	connect(applyInsert, SIGNAL(clicked()),this,SLOT(ApplyInsertFK()));
	connect(_tableManageRefTable, SIGNAL(cellClicked(int,int)), this, SLOT(LoadRefColumn(int,int)));
	connect(_tableManageRefColumn,SIGNAL(cellClicked(int,int)),this,SLOT(LoadOneTableAllCols(int,int)));
}

void Widget_ForeignKey::LoadRefTable(QString iDBName,QString iTableName,QStringList iAllColumnName)
{
	_dbName = iDBName;
	_tName = iTableName;
	_allColumnName = iAllColumnName;
	bool flag;
	//进入数据库查询所有外键信息 
	flag = AlterTable_SelectAllFKFromOneTable(iDBName,iTableName,_fkSum,_allFKName,_fkColumnName,_refTableName,_refColumnName);
	_tableManageRefTable->setRowCount(_fkSum);
	for (int i = 0; i < _fkSum; i++)
	{
		_tableManageRefTable->setItem(i,0,new QTableWidgetItem(_allFKName[i]));
		_tableManageRefTable->setItem(i,1,new QTableWidgetItem(_refTableName[i]));
	}
}

//点击左侧某个外键 加载右侧的该外键信息（不需操作数据库）
void Widget_ForeignKey::LoadRefColumn(int x,int y)
{
	QString tempFKName;

	tempFKName = _tableManageRefTable->item(x,1)->text();
	_tableManageRefColumn->setRowCount(_allColumnName.length());
	//根据外键顺序显示某个外键信息
	for (int i=0; i < _allColumnName.length(); i++)
	{
		_tableManageRefColumn->setItem(i,0,new QTableWidgetItem(_allColumnName[i]));
		for (int j=0; j < _fkSum; j++)
		{
			if (_fkColumnName[j] == _allColumnName[i])
			{
				_tableManageRefColumn->item(i, 0)->setCheckState(Qt::Checked);
				_tableManageRefColumn->setItem(i,1,new QTableWidgetItem(_refColumnName[j]));
			}
		}
	}

}

void Widget_ForeignKey::LoadOneDBAllTables()
{

}

void Widget_ForeignKey::LoadOneTableAllCols(int x,int y)
{
	QString tempTableName;
	int tempRow;

	if (_tableManageRefColumn->currentColumn() == 1)
	{
		tempRow = _tableManageRefColumn->currentRow();
		tempTableName = _refTableName[tempRow];
		//进入数据库根据表名搜索该参考表的所有列名

	}
}
 
void Widget_ForeignKey::AddNewFK()
{

}

void Widget_ForeignKey::AlterOldFK()
{

}

void Widget_ForeignKey::DropOldFK() 
{
	QString curFKName;
	int curRow;
	if (_onInsertFlag == 0)
	{
		if (_tableManageRefTable->currentItem() == NULL)
		{
			QMessageBox::information(this, "ERROR!", "Please choose one FK first！");
		}
		else
		{
			curRow = _tableManageRefTable->currentRow();
			curFKName = _tableManageRefTable->item(curRow,0)->text();
			QString myquery = "alter table `" + _dbName + "`.`" + _tName + "` drop foreign key `"+curFKName+"`;";
			double timestamp;
			long affectedRows;
			string res = executeWithQuery(1, myquery, timestamp, affectedRows);

			if (res.compare("success") == 0)
			{
				qDebug("delete success");
				//this->parent()->findChild<Widget_ShowCatalog *>("catalogWidget")->AddOneLogInfo(myquery, timestamp, affectedRows, true);
				_tableManageRefTable->removeRow(curRow);
				int sum = _tableManageRefColumn->rowCount();
				for(int i=0;i<sum;i++)
					_tableManageRefColumn->removeRow(0);
				_fkSum--;
				QMessageBox::information(this, "Success!", "Foreign key has been dropped");
			}
			else
			{
				//this->parentWidget()->findChild<Widget_ShowCatalog *>("catalogWidget")->AddOneLogInfo(myquery, timestamp, affectedRows, false);
				QMessageBox::information(this, "ERROR!", QString::fromStdString(res));
			}
		}
		
	}
}

Widget_ForeignKey::~Widget_ForeignKey()
{

}