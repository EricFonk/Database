//
//  table_create.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#include "table_create.h"
#include "mysqlQuery.h"

TableCrt::TableCrt()
{
    //初始化数据类型下拉列表
    QComboBox *datatype = new QComboBox();
    QStringList datalist;//数据类型列表
    datalist<<"int"<<"varchar(45)"<<"char(45)"<<"float"<<"double"<<"boolean"<<"tinyint"<<"smallint"<<"bigint"<<"tinyint"<<"datetime";
    datatype->addItems(datalist);
    //设置表格行列
    tableOpt = new QTableWidget(this);
    tableOpt->setColumnCount(9);
    tableOpt->setRowCount(1);
    //设置表头
    QStringList headers;
    headers<<"column name"<<"datatype"<<"PK"<<"NN"<<"UQ"<<"BIN"<<"UN"<<"AI"<<"default";
    tableOpt->setHorizontalHeaderLabels(headers);
    //添加下拉框
    tableOpt->setCellWidget(0,1,datatype);
    for(int i=2;i<8;i++)
    {
        QTableWidgetItem *check = new QTableWidgetItem();
        check->setCheckState(Qt::Unchecked);
        tableOpt->setItem(0,i,check);
    }
    //向widget中添加组件
    newTableName = new QLineEdit(this);//新表名输入框
    newTableName->setPlaceholderText("please input a table name");
    QLabel *tableName = new QLabel(tr("new table name"),this);
    QLabel *dbLabel = new QLabel(tr("Schema"),this);
    dbNameLabel = new QLabel(dbName,this);//所属模式名
    QPushButton *applyBtn = new QPushButton(tr("apply"),this);//提交按钮
    QPushButton *insertBtn = new QPushButton(tr("insert new column"),this);//添加新列？
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    mainLayout->addWidget(tableOpt);
    mainLayout->addWidget(insertBtn);
    mainLayout->addWidget(tableName);
    mainLayout->addWidget(newTableName);
    mainLayout->addWidget(dbLabel);
    mainLayout->addWidget(dbNameLabel);
    mainLayout->addWidget(applyBtn);
    this->setLayout(mainLayout);
    //connect(insertBtn, static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked),
    //	this, static_cast<void(TableCrtAlt::*)(bool)>(&TableCrtAlt::applyCreate));
    connect(applyBtn,SIGNAL(clicked(bool)),this,SLOT(applyCreate(bool)));
    connect(insertBtn,SIGNAL(clicked()),this,SLOT(insertCol()));
}
//设置表所属schema
void TableCrt::setDBName(QString dbname)
{
    dbName = dbname;
    dbNameLabel->setText(dbName);
}
void TableCrt::loadTableInfo(QString tname)
{
	newTableName->setText(tname);
	QStringList col_name,col_default,col_isNull,col_type,col_key,col_extra,col_uni;
	getTableAllCols(tname,col_name,col_default,col_isNull,col_type,col_key,col_extra,col_uni);
	int row = col_name.length();
	tableOpt->setRowCount(row);
	for(int i=0;i<row;i++)
	{
		tableOpt->setItem(i,0,new QTableWidgetItem(col_name[i]));
		tableOpt->setItem(i,8,new QTableWidgetItem(col_default[i]));
		if(col_isNull[i].compare("NO")==0)
		{
			QTableWidgetItem *check = new QTableWidgetItem();
			check->setCheckState(Qt::Checked);	
			tableOpt->setItem(i,3,check);
		}
		tableOpt->setItem(i,1,new QTableWidgetItem(col_type[i]));
		if(col_key[i].compare("PRI")==0)
		{
			QTableWidgetItem *check = new QTableWidgetItem();
			check->setCheckState(Qt::Checked);	
			tableOpt->setItem(i,2,check);
		}
		if(col_extra[i].compare("auto_increment")==0)
		{
			QTableWidgetItem *check = new QTableWidgetItem();
			check->setCheckState(Qt::Checked);	
			tableOpt->setItem(i,7,check);
		}	
	}
	
}
//新增一列
/*
	初始化表格的行数 为最后一行设置初值
 */
void TableCrt::insertCol()
{
    int row;
    row = tableOpt->rowCount();
    //初始化下拉列表
    QComboBox *datatype = new QComboBox();
    QStringList datalist;
    datalist<<"int"<<"varchar(45)"<<"char(45)"<<"float"<<"double"<<"boolean"<<"tinyint"<<"smallint"<<"bigint"<<"tinyint"<<"datetime";
    datatype->addItems(datalist);
    
    tableOpt->setRowCount(row+1);
    tableOpt->setCellWidget(row,1,datatype);
    for(int i=2;i<8;i++)
    {
        QTableWidgetItem *check = new QTableWidgetItem();
        check->setCheckState(Qt::Unchecked);
        tableOpt->setItem(row,i,check);
    }
}
//提交建表动作
/*
 首先获取当前tablewidget中行数（实际是该表的列个数）
 然后对表格进行遍历
 获取每一个单元格值（即每一列的每一个属性）
 组装为建表语句
 */
void TableCrt::applyCreate(bool)
{
	int colnum=0;
	QStringList createOpt,pkOpts,uniqOpts;
	QString pkOption="",tableName="";
	colnum = tableOpt->rowCount();//获取用户新建列数

	for(int i=0;i<colnum;i++)
	{
		QString oneOpt="";
		QWidget *temp = tableOpt->cellWidget(i,1);
		QComboBox *tempCB = (QComboBox *)temp;
		
		QStringList checkOpts;
		QString pkOpt="",uniqOpt="";
		checkOpts<<tempCB->currentText();
		for(int j=2;j<8;j++)
		{
			if(tableOpt->item(i,j)->checkState()==Qt::Checked)
			{
				if(j==2)
				{
					pkOpt="`"+tableOpt->item(i,0)->text()+"`";	
					pkOpts<<pkOpt;
				}
				else if(j==4)
				{
					uniqOpt= "UNIQUE INDEX `"+tableOpt->item(i,0)->text()+"_UNIQUE` (`"+tableOpt->item(i,0)->text()+"` ASC)";
					uniqOpts<<uniqOpt;
				}
				else
				{
					QString keyword="";
					switch(j)
					{
						case 3:keyword="NOT NULL";
							break;
						case 5:keyword="";
							break;
						case 6:keyword="UNSIGNED";
							break;
						case 7:keyword="ZEROFILL";
							break;
						case 8:keyword="AUTO_INCREMENT";
							break;
					}
					checkOpts<<keyword;
				}
			}
		}
		if(tableOpt->item(i,9)!=NULL)
		{
			checkOpts<<"DEFAULT "+tableOpt->item(i,9)->text();
		}
		//加入列名
		oneOpt = "`"+tableOpt->item(i,0)->text()+"`";
		//加入约束
		for(int i=0;i<checkOpts.length();i++)
		{
			oneOpt = oneOpt+" "+checkOpts[i];
		}
		createOpt<<oneOpt;
		//主键约束
		if(pkOpts.length()!=0){
			pkOption="PRIMARY KEY (";
			for(int i=0;i<pkOpts.length();i++)
			{
				pkOption = pkOption+pkOpts[i];
				if(i!=(pkOpts.length()-1))
				{
					pkOption=pkOption+",";
				}
				else
				{
					pkOption=pkOption+")";
				}
			}
		}		
	}
	tableName = newTableName->text();
	string res = createNewTable(dbName,tableName,createOpt,pkOption,uniqOpts);
	if(res.compare("success")==0)
	{			
		emit createTableSuccess(tableName);
            //listView->edit(index); 
	}
	else
	{
		QMessageBox::information(this, "ERROR!", QString::fromStdString(res)); 
	}
	qDebug("test");
}