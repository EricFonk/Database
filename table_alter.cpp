//
//  table_alter.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#include "table_alter.h"
#include "mysqlQuery.h"

TableAlt::TableAlt()
{
	onInsertFlag = 0;
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
	newTableName = new QLineEdit(this);//新表名输入框
	newTableName->setPlaceholderText("");
	QLabel *tableName = new QLabel(tr("table name"),this);
	QLabel *dbLabel = new QLabel(tr("Schema"),this);
	dbNameLabel = new QLabel(dbName,this);//所属模式名
	QPushButton *addColumnBtn = new QPushButton(tr("+"),this);
	QPushButton *dropColumnBtn = new QPushButton(tr("-"),this);
	QPushButton *cfmAddColBtn = new QPushButton(tr("confirm add column"),this);
	QPushButton *applyBtn = new QPushButton(tr("apply alter"),this);//提交按钮
	QPushButton *refreshBtn = new QPushButton(tr("refresh the columns setting"));
	QVBoxLayout *mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(tableOpt);
	mainLayout->addWidget(addColumnBtn);
	mainLayout->addWidget(dropColumnBtn);
	mainLayout->addWidget(cfmAddColBtn);
	mainLayout->addWidget(tableName);
	mainLayout->addWidget(newTableName);
	mainLayout->addWidget(dbLabel);
	mainLayout->addWidget(dbNameLabel);
	mainLayout->addWidget(applyBtn);
	mainLayout->addWidget(refreshBtn);
	this->setLayout(mainLayout); 	
	//connect(insertBtn, static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked), 
	//	this, static_cast<void(TableCrtAlt::*)(bool)>(&TableCrtAlt::ApplyCreate));
	connect(applyBtn,SIGNAL(clicked(bool)),this,SLOT(ApplyAlter(bool)));
	connect(dropColumnBtn,SIGNAL(clicked()),this,SLOT(DropCol()));
	connect(addColumnBtn,SIGNAL(clicked()),this,SLOT(InsertCol()));
	connect(cfmAddColBtn,SIGNAL(clicked()),this,SLOT(ConfirmInsertCol()));
	connect(refreshBtn,SIGNAL(clicked()),this,SLOT(RefreshAllInfo()));
	//connect(tableOpt->model(),SIGNAL(modelDataChanged(const QModelIndex &topLeft, QModelIndex &buttomRight)),this,SLOT(OnDataChange(const QModelIndex &topLeft, QModelIndex &buttomRight)));
}

void TableAlt::SetDTName(QString dbname,QString tname)
{
    this->dbName = dbname;
    this->tName = tname;
}

void TableAlt::LoadTableInfo()
{
	newTableName->setText(tName);
	dbNameLabel->setText(dbName);
	if(onInsertFlag!=0)
	{
		//只刷新逻辑模型 不更新视图
		col_name.clear();
		col_default.clear();
		col_isNull.clear();
		col_type.clear();
		col_key.clear();
		col_extra.clear();
		//特殊状态归零
		onInsertFlag = 0;
		GetTableAllCols(tName,col_name,col_default,col_isNull,col_type,col_key,col_extra,col_uni);
		columns_num = col_name.length();
		tableOpt->setRowCount(columns_num);
	}
	else
	{
		//第一次初始化需要加载视图
		GetTableAllCols(tName,col_name,col_default,col_isNull,col_type,col_key,col_extra,col_uni);
		columns_num = col_name.length();
		tableOpt->setRowCount(columns_num);
		//动态数组定义问题？？？
		check1 = new QTableWidgetItem[columns_num];
		check2 = new QTableWidgetItem[columns_num];
		check3 = new QTableWidgetItem[columns_num];
		datatype = new QComboBox[columns_num];
		QStringList datalist;
		datalist<<"int"<<"varchar(45)"<<"char(45)"<<"float"<<"double"<<"boolean"<<"tinyint"<<"smallint"<<"bigint"<<"tinyint"<<"datetime";

		for(int i=0;i<columns_num;i++)
		{
			tableOpt->setItem(i,0,new QTableWidgetItem(col_name[i]));
			tableOpt->setItem(i,8,new QTableWidgetItem(col_default[i]));
			//初始化下拉列表
			QComboBox *tempDataType = new QComboBox;
			tempDataType->addItems(datalist);
			tempDataType->setCurrentText(col_type[i]);
			tableOpt->setCellWidget(i,1,tempDataType);

			QTableWidgetItem *tempcheck3 = new QTableWidgetItem();
			QTableWidgetItem *tempcheck2 = new QTableWidgetItem();
			QTableWidgetItem *tempcheck7 = new QTableWidgetItem();
			
			if(col_isNull[i].compare("NO")==0)
			{	
				tempcheck3->setCheckState(Qt::Checked);
			}
			else
			{
				tempcheck3->setCheckState(Qt::Unchecked);
			}
			if(col_key[i].compare("PRI")==0)
			{
				tempcheck2->setCheckState(Qt::Checked);	
			}
			else
			{
				tempcheck2->setCheckState(Qt::Unchecked);
			}
			if(col_extra[i].compare("auto_increment")==0)
			{
				tempcheck7->setCheckState(Qt::Checked);	
			}	
			else
			{
				tempcheck7->setCheckState(Qt::Unchecked);
			}

			tableOpt->setItem(i,3,tempcheck3);
			tableOpt->setItem(i,2,tempcheck2);
			tableOpt->setItem(i,7,tempcheck7);			
			//this->tableOpt->setItem(i,4,tempcheck);
		}
	}
}

void TableAlt::OnDataChange(const QModelIndex &topLeft, QModelIndex &buttomRight)
{
    qDebug("alter hhhhhhhhhhhhhhhhhhhhhhhhhhhhhh!");
}

void TableAlt::ApplyAlter(bool param)
{
	QString new_col_name;
	QString new_col_default;
	QString new_col_isNull;
	QString new_col_type;
	QString new_col_key;
	QString new_col_extra;
	QString alterQuery;
	QStringList oneQuery;
	QString pkQuery;
	QStringList tarColNum;
	
	int new_col_num;
	int flag_hasAlterItem;
	new_col_num = this->tableOpt->rowCount();

	//初始化onequery
	alterQuery = "alter table "+tName+" ";

	for(int i=0;i<columns_num;i++)
	{
		flag_hasAlterItem=false;
		//oneQuery += " ";
		for(int j=0;j<8;j++)
		{	
			pkQuery = "";
			if(tableOpt->item(i,0)->text()==NULL||tableOpt->item(i,0)->text().compare("")==0)
			{
				//给出不能为空的错误提示
				QMessageBox::information(this,"ERROR","Column must has a Name !");
			}
			else
			{
				switch(j)
				{
				case 0:
					if(this->tableOpt->item(i,j)->text()!=col_name[i])
					{
						flag_hasAlterItem = true;
					}
					//修改列名sql参数
					new_col_name = this->tableOpt->item(i,j)->text();
					break;
				case 1:
					{
						QWidget *temp = tableOpt->cellWidget(i,1);
						QComboBox *tempCB = (QComboBox *)temp;
						if(tempCB->currentText()!=col_type[i])
						{
							flag_hasAlterItem = true;
						}
						//修改列类型sql参数
						new_col_type = tempCB->currentText();
						break;
					}
				case 2:
					if((this->tableOpt->item(i,j)->checkState()==Qt::Checked)&&(col_key[i].compare("PRI")!=0))
					{
						flag_hasAlterItem = true;
						pkQuery = ",add primary key ('"+col_name[i]+"')";
					}
					if((this->tableOpt->item(i,j)->checkState()!=Qt::Checked)&&(col_key[i].compare("PRI")==0))
					{
						flag_hasAlterItem = true;
						pkQuery = ",drop primary key";
					}
					break;
				case 3:
					//去掉非空约束
					if((this->tableOpt->item(i,j)->checkState()!=Qt::Checked)&&(col_isNull[i].compare("NO")==0))
					{
						flag_hasAlterItem = true;
						new_col_isNull = "NULL";
					}
					//添加非空约束
					if((this->tableOpt->item(i,j)->checkState()==Qt::Checked)&&(col_isNull[i].compare("NO")!=0))
					{
						int temp = this->tableOpt->item(i,j)->checkState();
						qDebug("%d",temp);
						flag_hasAlterItem = true;
						new_col_isNull = "NOT NULL";
					}
					break;
				case 7:
					//删除自增约束
					if((this->tableOpt->item(i,j)->checkState()!=Qt::Checked)&&(col_extra[i].compare("auto_increment")==0))
					{
						flag_hasAlterItem = true;
						new_col_extra = " ";
					}
					//添加自增约束
					if((this->tableOpt->item(i,j)->checkState()==Qt::Checked)&&(col_extra[i].compare("auto_increment")!=0))
					{
						flag_hasAlterItem = true;
						new_col_extra = "AUTO_INCREMENT";
					}
					break;
				}
			}
		}
		if(flag_hasAlterItem)//列信息有所改动
		{
			//onInsertFlag = 2;
			//将行号加入有改动的行list
			tarColNum<<""+i;
			//把关于一行的修改sql语句组装起来	
			oneQuery.push_back("CHANGE `"+col_name[i]+"` `"+new_col_name+"` "+new_col_type+" "+new_col_isNull+" "+new_col_extra);								
		}
		//this->tableOpt->indexAt();
	}
	if(oneQuery.length()!=0&&flag_hasAlterItem)
	{	
		alterQuery = "ALTER TABLE "+dbName+"."+tName;
		for(int i=0;i<oneQuery.length();i++)
		{
			if(i==0)
			{
				alterQuery += " ";
			}
			else
			{
				if(oneQuery[i].compare("")!=0&&oneQuery[i]!=NULL)
				{
					alterQuery += ",";
				}
			}
			alterQuery += oneQuery[i];	
		}
		//直接组装好语句还是设置好参数调用函数
		string flag = ExecuteWithQuery(0,alterQuery+";");

		if(flag.compare("success")==0)
		{
			//数据库操作成功后 对逻辑模型数据进行更新
			ConfirmLatestUpdateOperation();
			QMessageBox::information(this,"Infomation","Update columns infomation SUCCESS!");
			//UpdateAllInfoAfter(tarColNum,2);
		}
		else
		{
			//暂时选择不回退视图数据 因为逻辑模型数据并没有发生改动
			QMessageBox::information(this,"Alter Failed!",QString::fromStdString(flag));
		}
	}
	if(!flag_hasAlterItem)
	{
		QMessageBox::information(this,"Warning!","No columns has been changed!");
	}
}

//修改表名
void TableAlt::RenameTable()
{
	//获取旧表名和新表名 调用修改表名函数
	QString new_TName;
	QString flag;
	new_TName = this->newTableName->text();
	flag = AlterTableRenameTable(tName,new_TName);
	if(flag.compare("OK")!=0)
	{
		//数据库操作成功后 对逻辑模型数据更新
		this->tName = new_TName;
		QMessageBox::information(this,"Rename Table",flag);
	}
	else
	{
		QMessageBox::information(this,"Rename Table","Rename Table Successed!");
	}
}

//修改

void TableAlt::InsertCol()
{
    //获取列信息 调用增列函数 返回提示信息
    QString new_colName;
    QString new_colType;
    QString new_colExtra;
    QString new_colIsNull;
    QString new_colKey;
    QString new_colDefault;
    
    //获取输入的新一列的 列名 列类型 各种约束
    //如何能在一个界面中 点击新建列即在原table中进行编辑
    //锁定旧列
    if(this->onInsertFlag==0)
    {
        for(int i=0;i<this->tableOpt->rowCount();i++)
        {
            for(int j=0;j<this->tableOpt->columnCount();j++)
            {
                if(this->tableOpt->item(i,j)!=NULL)
                    this->tableOpt->item(i,j)->setFlags(Qt::NoItemFlags);
                if(j==1)
                {
                    this->tableOpt->cellWidget(i,j)->setDisabled(true);
                }
            }
        }
        //进入新增状态
        this->onInsertFlag=1;
    }
    //初始化下拉列表 必须是局部的变量
    QComboBox *datatype = new QComboBox();
    QStringList datalist;
    datalist<<"int"<<"varchar(45)"<<"char(45)"<<"float"<<"double"<<"boolean"<<"tinyint"<<"smallint"<<"bigint"<<"tinyint"<<"datetime";
    datatype->addItems(datalist);
    //重设行数
    tableOpt->setRowCount((this->tableOpt->rowCount())+1);
    qDebug(this->tableOpt->rowCount()+"");
    //加入数据类型下拉框
    tableOpt->setCellWidget((this->tableOpt->rowCount()-1),1,datatype);
    //循环加入单选框 必须是局部的变量？
    for(int i=2;i<8;i++)
    {
        QTableWidgetItem *check = new QTableWidgetItem();
        check->setCheckState(Qt::Unchecked);
        tableOpt->setItem((this->tableOpt->rowCount()-1),i,check);
    }
}

void TableAlt::ConfirmInsertCol()
{
	QStringList allColNames,allColTypes,allColOpts,allPkSets;
	QStringList allPkOpts,allIndexOpts;//主键约束（只有列名）索引约束（只有列名）
	QString oneOpt;
	int startLine,endLine;

	startLine=columns_num;
	endLine=this->tableOpt->rowCount();

	if(onInsertFlag==0||endLine==startLine)
	{
		QMessageBox::information(this,"Warning","No columns has been changed!");
	}
	else
	{
		//循环增加列信息
		for(int i=startLine;i<endLine;i++)
		{
			//获取并加入新列名
			allColNames<<this->tableOpt->item(i,0)->text();
			//获取并加入新列类型
			QWidget *temp = tableOpt->cellWidget(i,1);
			QComboBox *tempCB = (QComboBox *)temp;
			allColTypes << tempCB->currentText();
			//将一行约束设置置空
			oneOpt = "";
			for(int j=2;j<8;j++)
			{
				if(tableOpt->item(i,j)->checkState()==Qt::Checked)
				{
					if(j==2)
					{
						//主键约束另做
						allPkOpts<<this->tableOpt->item(i,0)->text();
					}
					else if(j==4)
					{
						//索引处理
						allIndexOpts<<this->tableOpt->item(i,0)->text();
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
						oneOpt += " "+keyword;
					}
				}
			}
			if(tableOpt->item(i,9)!=NULL)
			{
				oneOpt+=" DEFAULT "+tableOpt->item(i,9)->text();
			}
			//加入约束
			allColOpts<<oneOpt;		
		}
		//进行数据库改表操作
		QString resFlag = AlterTableAddColumns(dbName,tName,allColNames,allColTypes,allColOpts,allPkOpts,allIndexOpts);
		//解除旧列锁定
		for(int i=0;i<this->tableOpt->rowCount();i++)
		{
			for(int j=0;j<this->tableOpt->columnCount();j++)
			{
				if(this->tableOpt->item(i,j)!=NULL)
				{
					this->tableOpt->item(i,j)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
				}
				if(j==1)
				{
					this->tableOpt->cellWidget(i,j)->setDisabled(false);
				}
			}
		}
		//取消新增状态
		this->onInsertFlag=0;
		if(resFlag.compare("success")==0)
		{
			//数据库操作成功后 对逻辑模型数据进行更新
			ConfirmLatestAddOperation(endLine);
			QMessageBox::information(this,"Infomation","Add new columns SUCCESS!");
			//UpdateAllInfoAfter(tarColNum,0);
		}
		else
		{
			//需要回滚增列操作吗？
			//打印错误信息
			//RollBackLatestAddOperation(startLine,endLine);
			QMessageBox::information(this,"Add Column Failed!","Please check all items");
		}
	}
}

void TableAlt::DropCol()
{
	//获取列名 调用删列函数 返回提示信息
	QString drop_colName;
	QString flag;
	QStringList tarColNum;

	if(this->onInsertFlag==1)
	{
		QMessageBox::information(this,"Operation denied","Please finish your insert operation first!");
	}
	/*else if(!this->tableOpt->hasFocus())
	{
		QMessageBox::information(this,"Warning","Please select one column of current table to drop!");
	}*/
	else
	{
		tarColNum<<""+this->tableOpt->currentRow();
		drop_colName = this->tableOpt->item(this->tableOpt->currentRow(),0)->text();
		flag = AlterTableDropColumn(dbName,tName,drop_colName);
		if(flag.compare("OK")!=0)
		{
			//删除不需要回退
		//	RollBackLatestDropOperation(this->tableOpt->currentRow());
			QMessageBox::information(this,"Drop Column Failed!",flag);
		}
		else
		{
			QMessageBox::information(this,"Drop Column Success!","Please refresh table");
			//onInsertFlag=3;
			//数据库操作成功后 对逻辑模型数据进行更新
			ConfirmLatestDropOperation(this->tableOpt->currentRow());
			//UpdateAllInfoAfter(tarColNum,1);
		}
	}
}

////更新tablealt中的原数据（上一次操作后的数据） 以便在更改时判断那些列被更改了
//void TableAlt::UpdateAllInfoAfter(QStringList colNumList,int optType)
//{
//	int oldColSum,newColSum;
//
//	oldColSum = this->columns_num;
//	newColSum = this->tableOpt->rowCount();
//
//	if(this->onInsertFlag==1)
//	{
//		QMessageBox::information(this,"Operation denied","Please finish your insert operation first!");
//	}
//	else
//	{
//		switch(optType)
//		{
//			//添加列之后更新原数据逻辑模型
//		case 0:
//			for(int i=oldColSum;i<newColSum;i++)
//			{
//				//需不需要进行非空判断？
//				col_name<<this->tableOpt->item(i,0)->text();
//				col_default<<this->tableOpt->item(i,9)->text();
//				col_extra<<this->tableOpt->item(i,8)->text();
//				if(this->tableOpt->item(i,3)->checkState()==Qt::Checked)
//				{
//					col_isNull<<"NOT NULL";
//				}
//				else
//				{
//					col_isNull<<"NULL";
//				}
//				QWidget *temp = tableOpt->cellWidget(i,1);
//				QComboBox *tempCB = (QComboBox *)temp;
//				col_type<<tempCB->currentText();
//				if(this->tableOpt->item(i,2)->checkState()==Qt::Checked)
//				{
//					col_key<<"PRI";
//				}
//				else
//				{
//					col_key<<"";
//				}
//			}
//			this->columns_num = this->tableOpt->rowCount();
//			//将新列加入视图
//			break;
//			//删除列之后更新原数据逻辑模型
//		case 1:
//			this->columns_num = this->tableOpt->rowCount();
//			//根据行号删除所有原数据对象中对应的数据
//			//sizeof能否获取数组长度？？
//			for(int i=0;i<colNumList.length();i++)
//			{
//				col_name.removeAt(colNumList[i].toInt());
//				col_default.removeAt(colNumList[i].toInt());
//				col_isNull.removeAt(colNumList[i].toInt());
//				col_type.removeAt(colNumList[i].toInt());
//				col_key.removeAt(colNumList[i].toInt());
//				col_extra.removeAt(colNumList[i].toInt());
//			}		
//			//将旧列删除视图
//			break;
//			//更新列信息之后更新原数据逻辑模型
//		case 2:
//			//根据行号更新所有原数据对象中对应的数据
//			//sizeof能否获取数组长度？？
//			for(int i=0;i<sizeof(colNumList);i++)
//			{
//				col_name.replace(i,this->tableOpt->item(colNumList[i].toInt(),0)->text());
//				col_extra.replace(i,this->tableOpt->item(colNumList[i].toInt(),8)->text());
//				col_default.replace(i,this->tableOpt->item(colNumList[i].toInt(),9)->text());
//				if(this->tableOpt->item(i,3)->checkState()==Qt::Checked)
//				{
//					col_isNull.replace(i,"NOT NULL");
//				}
//				else
//				{
//					col_isNull.replace(i,"NULL");
//				}
//				QWidget *temp = tableOpt->cellWidget(i,1);
//				QComboBox *tempCB = (QComboBox *)temp;
//				col_type.replace(i,tempCB->currentText());
//				if(this->tableOpt->item(i,2)->checkState()==Qt::Checked)
//				{
//					col_key.replace(i,"PRI");
//				}
//				else
//				{
//					col_key.replace(i,"");
//				}
//			}
//			//对修改过的列更新视图
//			break;
//		default:
//			break;
//		}
//	}	
//}

TableAlt::~TableAlt()
{
	delete[] check1;
	delete[] check2;
	delete[] check3;
	delete[] datatype;
}

//因为用户对视图进行了修改而数据库修改失败 所以需要将视图上的修改进行回退
//回退增列操作
void TableAlt::RollBackLatestAddOperation(int oldColSum, int newColSum)
{
	//将从修改前总列数至修改后总列数之间的列去掉
	for(int i=oldColSum;i<newColSum;i++)
	{
		delete &check1[i];
		delete &check2[i];
		delete &check3[i];
		delete &datatype[i];
		this->tableOpt->removeRow(i);
	}
}

//回退删列操作 不需要
//void TableAlt::RollBackLatestDropOperation(int tarColNum)
//{
//	//将目标行添加回原位置(位置参数由零开始)
//	this->tableOpt->insertRow(tarColNum);
//	this->tableOpt->item(tarColNum,0)->text()=col_name[tarColNum];
//	if(col_isNull[tarColNum].compare("NO")==0)
//	{
//		check1[tarColNum].setCheckState(Qt::Checked);	
//	}
//	else
//	{
//		check1[tarColNum].setCheckState(Qt::Unchecked);
//	}
//	if(col_key[tarColNum].compare("PRI")==0)
//	{
//		check2[tarColNum].setCheckState(Qt::Checked);	
//	}
//	else
//	{
//		check2[tarColNum].setCheckState(Qt::Unchecked);
//	}
//	if(col_extra[tarColNum].compare("auto_increment")==0)
//	{
//		check3[tarColNum].setCheckState(Qt::Checked);	
//	}	
//	else
//	{
//		check3[tarColNum].setCheckState(Qt::Unchecked);
//	}
//
//	tableOpt->setItem(tarColNum,3,&check1[tarColNum]);
//	tableOpt->setItem(tarColNum,2,&check2[tarColNum]);
//	tableOpt->setItem(tarColNum,7,&check3[tarColNum]);
//
//}
//回退列信息修改操作
void TableAlt::RollBackLatestUpdateOperation()
{
	int i,j;
	for(i=0;i<columns_num;i++)
	{
		for(j=0;i<9;j++)
		{
			switch(i)
			{
			case 0:
				tableOpt->item(i,j)->text() = col_name[i];
				break;
			case 1:
			{
				QWidget *temp = tableOpt->cellWidget(i,1);
				QComboBox *tempCB = (QComboBox *)temp;
				tempCB->setCurrentText(col_type[i]);
				break;
			}
			case 3:
				tableOpt->setItem(i,j,&check1[i]);
				break;
			case 2:
				tableOpt->setItem(i,j,&check2[i]);
				break;
			case 7:
				tableOpt->setItem(i,j,&check3[i]);
				break;
			case 8:
				tableOpt->item(i,j)->text()=col_default[i];
				break;
			}
		}
	}
}
//因为用户对数据库修改成功之后 逻辑模型中的数据还是修改之前的 所以需要进行更新 以便进行继续的修改
//插入列操作之后向逻辑模型中添加
void TableAlt::ConfirmLatestAddOperation(int newColSum)
{
	//先保存旧行数 再对行数属性更新！！！
	int oldColSum = this->columns_num;
	this->columns_num = newColSum;
	int i=0;
	int j=0;
	for(i=oldColSum;i<newColSum;i++)
	{
		if(this->tableOpt->item(i,0)->text()!=NULL)
		{
			this->col_name<<this->tableOpt->item(i,0)->text();
		}
		else
		{
			this->col_name<<"";
		}

		QWidget *temp = this->tableOpt->cellWidget(i,1);
		QComboBox *tempCB = (QComboBox *)temp;
		this->col_type<<tempCB->currentText();
		if(this->tableOpt->item(i,2)->checkState()==Qt::Checked)
		{
			this->col_key<<"PRI";
		}
		else
		{
			this->col_key<<"";
		}
		if(this->tableOpt->item(i,3)->checkState()==Qt::Checked)
		{
			this->col_isNull<<"NO";	
		}
		else
		{
			this->col_isNull<<"YES";
		}

		if(this->tableOpt->item(i,7)->checkState()==Qt::Checked)
		{
			this->col_extra<<"auto_increment";
		}
		else
		{
			//数组下标很重要 所以即时没有约束也要加一个空串占位
			this->col_extra<<"";
		}

		if(this->tableOpt->item(i,8)!=NULL)
		{
			this->col_default<<this->tableOpt->item(i,8)->text();
		}
		else
		{
			this->col_default<<"";
		}

	}
}
//在数据库完成删列操作成功后 同时对视图进行删除 以及对逻辑模型数据进行删除 与增列和修改列信息有所区别
void TableAlt::ConfirmLatestDropOperation(int tarColNum)
{
	this->columns_num --;
	//视图删除
	this->tableOpt->removeCellWidget(tarColNum,1);
	this->tableOpt->removeCellWidget(tarColNum,2);
	this->tableOpt->removeCellWidget(tarColNum,3);
	this->tableOpt->removeCellWidget(tarColNum,7);

	this->tableOpt->removeRow(tarColNum);
	//逻辑模型删除
	col_name.removeAt(tarColNum);
	col_type.removeAt(tarColNum);
	col_extra.removeAt(tarColNum);
	col_default.removeAt(tarColNum);
	col_isNull.removeAt(tarColNum);
	col_key.removeAt(tarColNum);
}
//在数据库更新成功之后 修改列信息操作已在视图上完成 需要对逻辑模型数据进行更新
void TableAlt::ConfirmLatestUpdateOperation()
{
	int i=0;
	int j=0;
	if(this->onInsertFlag==1)
	{
		QMessageBox::information(this,"Operation denied","Please finish your insert operation first!");
	}
	else
	{
		for(i=0;i<this->tableOpt->rowCount();i++)
		{
			if(this->tableOpt->item(i,0)->text()!=NULL)
			{
				this->col_name[i]=this->tableOpt->item(i,0)->text();
			}
			else
			{
				//此处应该报错 但是之前应经过数据库操作成功验证 故理论上此分支不会被调用
				this->col_name<<"";
			}
			QWidget *temp = this->tableOpt->cellWidget(i,1);
			QComboBox *tempCB = (QComboBox *)temp;
			this->col_type[i]=tempCB->currentText();

			if(this->tableOpt->item(i,3)->checkState()==Qt::Checked)
			{
				this->col_isNull[i]="NO";	
			}
			else
			{
				this->col_isNull[i]="YES";
			}
			break;

			if(this->tableOpt->item(i,7)->checkState()==Qt::Checked)
			{
				this->col_extra[i]="auto_increment";
			}
			else
			{
				//数组下标很重要 所以即时没有约束也要加一个空串占位
				this->col_extra[i]="";
			}

			if(this->tableOpt->item(i,8)->text()!=NULL)
			{
				this->col_default[i]=this->tableOpt->item(i,8)->text();
			}
			else
			{
				this->col_default[i]="";
			}
		}
	}
}

void TableAlt::RefreshAllInfo()
{
	int i;
	if(this->onInsertFlag==1)
	{
		QMessageBox::information(this,"Warning!","Please finish your insert operation first!");
	}
	else
	{
		QMessageBox::StandardButton rb = QMessageBox::question(NULL,"Warning!","Refresh operation will abolish your rencent changes which have not been saved, are you sure to give up the changes?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
		//用户确定要刷新视图
		if(rb==QMessageBox::Yes)
		{
			col_name.clear();
			col_default.clear();
			col_default.clear();
			col_isNull.clear();
			col_type.clear();
			col_key.clear();
			col_extra.clear();
			col_uni.clear();
			//不止视图需要刷新 数据模型一样需要更新
			GetTableAllCols(tName,col_name,col_default,col_isNull,col_type,col_key,col_extra,col_uni);
			columns_num = col_name.length();
			tableOpt->setRowCount(columns_num);
			for(int i=0;i<columns_num;i++)
			{
				tableOpt->setItem(i,0,new QTableWidgetItem(col_name[i]));
				tableOpt->setItem(i,8,new QTableWidgetItem(col_default[i]));
				QTableWidgetItem *notNullItem = new QTableWidgetItem();
				QTableWidgetItem *pkItem = new QTableWidgetItem();
				QTableWidgetItem *aiItem = new QTableWidgetItem();
				if(col_isNull[i].compare("NO")==0)
				{
					notNullItem->setCheckState(Qt::Checked);
					this->tableOpt->setItem(i,3,notNullItem);
				}
				else
				{
					notNullItem->setCheckState(Qt::Unchecked);
				}
				if(col_key[i].compare("PRI")==0)
				{
					pkItem->setCheckState(Qt::Checked);
				}
				else
				{
					pkItem->setCheckState(Qt::Unchecked);
				}
				if(col_extra[i].compare("auto_increment")==0)
				{
					aiItem->setCheckState(Qt::Checked);
				}
				else
				{
					aiItem->setCheckState(Qt::Unchecked);
				}
			}
		}
	}
}