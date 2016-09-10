//
//  tablewidget.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//
#include "tablewidget.h"
#include "mysqlQuery.h"
#include "insertTable.h"
#include "mylistview.h"
TableWidget::TableWidget()
{
	this->onInsertFlag = 0;//初次进入时必然不是插入状态
	this->safeMode = 0;//默认用户设置了主键 即非安全模式
    //插入数据按钮
    QPushButton *pushButton = new QPushButton(tr("insert"), this);
    pushButton->setGeometry(QRect(70, 40, 75, 23));
	//确认插入数据按钮
	QPushButton *pushButton4 = new QPushButton(tr("apply insert"),this);
	//pushButton4->setGeometry(QRect(70,40,75,23));
    //删除某行按钮
    QPushButton *pushButton2 = new QPushButton(tr("delete"), this);
    pushButton2->setGeometry(QRect(70, 40, 75, 23));
    //刷新表数据按钮
    QPushButton *pushButton3 = new QPushButton(tr("refresh"), this);
    pushButton2->setGeometry(QRect(70, 40, 75, 23));
    //初始化表格控件
    table = new QTableWidget(this);
    table->setColumnCount(1);
    table->setRowCount(1);
    //初始化表头
    QStringList headers;
    headers << "new Column";
    table->setHorizontalHeaderLabels(headers);
	//加入日志栏
	catalogWidget = new Widget_ShowCatalog();
    //添加控件
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(table);
    mainLayout->addWidget(pushButton);
    mainLayout->addWidget(pushButton2);
    mainLayout->addWidget(pushButton3);
	mainLayout->addWidget(pushButton4);
	mainLayout->addWidget(catalogWidget);
    this->setLayout(mainLayout);
    //点击某个单元格时触发更新操作
	//connect(table, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(testSlot(int,int)));
    
	connect(table, SIGNAL(clicked(QModelIndex)), this, SLOT(UpdateData(QModelIndex)));
    //点击新增按钮时向表格插入一行
    connect(pushButton, SIGNAL(clicked()), this, SLOT(InsertLine()));
	//点击确认插入时真正执行sql
	connect(pushButton4, SIGNAL(clicked()),this,SLOT(ConfirmInsert()));
    //点击删除按钮时获取选中行并将其删除
    connect(pushButton2, SIGNAL(clicked()), this, SLOT(DeleteLine()));
    //点击刷新按钮时刷新表格中所有数据
    //待改进 点击后判断数据是否变化并提示用户保存改动
    connect(pushButton3, SIGNAL(clicked()), this, SLOT(RefreshTableData()));
}

void TableWidget::ConfirmInsert()
{
	QString myquery,selectedCols,insertVals;
	std::string res;
	double timestamp,sumTimeStamp;
	long affectedRows,sumAffectedRows;
	sumTimeStamp = 0;
	sumAffectedRows = 0;
	int startLine, endLine;

	startLine = this->qSumRow;
	endLine = this->table->rowCount();
	if (onInsertFlag == 0 || endLine == startLine)
	{
		QMessageBox::information(this, "Warning", "No columns has been changed!");
	}
	else
	{
		//循环增加列信息
		for (int j = startLine; j<endLine; j++)
		{
			myquery = "insert into " + tableName;
			selectedCols = "";
			insertVals = "";
			for (int i = 0; i<qSumCol; i++)
			{
				if (table->item(j, i) != NULL)
				{
					selectedCols = selectedCols + "`" + this->qHead[i] + "`";
					insertVals = insertVals + "'" + table->item(j, i)->text() + "'";
					if (i != (qSumCol - 1) && table->item(j, i + 1) != NULL)//不是最后一列且后面一列不为空
					{
						selectedCols = selectedCols + ",";
						insertVals = insertVals + ",";
					}
				}
			}
			myquery = myquery + "(" + selectedCols + ") " + "values(" + insertVals + ");";
			res = executeWithQuery(0, myquery, timestamp, affectedRows);
			timestamp += timestamp;
			affectedRows += affectedRows;
			if (res.compare("success") != 0)
				goto error;
		}
		//解除旧列锁定
		if (this->qSumRow != 0)
		{
			for (int i = 0; i<this->table->rowCount(); i++)
			{
				for (int j = 0; j<this->table->columnCount(); j++)
				{
					if (this->table->item(i, j) != NULL)
					{
						this->table->item(i, j)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
					}
					/*if (j == 1)
					{
						this->table->cellWidget(i, j)->setDisabled(false);
					}*/
				}
			}
		}
		//全部正确来到此步 否则goto至error
		//取消新增状态
		this->catalogWidget->AddOneLogInfo(myquery, timestamp, affectedRows, true);
		this->onInsertFlag = 0;
		this->qSumRow = this->table->rowCount();
		QMessageBox::information(this, "Infomation", "Add new columns SUCCESS!");
	}	
	return;
error: 
	QMessageBox::information(this, "Add Column Failed!", "Please check all items");
	this->catalogWidget->AddOneLogInfo(myquery, timestamp, affectedRows, true);
	return;
}

//void TableWidget::testSlot(int row, int col)
//{
//	this->table->setItem(row,col, new QTableWidgetItem("hjhhhhhhhhhhhhh"));
//
//}
//设置表头
/*
	设置列个数 列名称 同时设置是否安全模式！重要
 */
void TableWidget::SetHeaders(QString table_name,int col,QStringList headers,QStringList main,QStringList pos)
{
    tableName = table_name;
    mainKey = main;
    mainKeyPos = pos;
    qHead = headers;
    table->setColumnCount(col);
    table->setHorizontalHeaderLabels(headers);
	if (mainKey.length() == 0)
		this->safeMode = 1;//主键不存在 设为安全模式
}
//新增一行
/*
	加入InsertTable控件以完成新增行功能
	待完善
 */
void TableWidget::InsertLine()
{
    /*InsertTable *t = new InsertTable(tableName);
    t->show();
    t->SetHeaders(qSumCol,qHead);*/
	if (this->safeMode)//如果为安全模式则提示不可操作
	{
		QMessageBox::information(this, "Operation denied", "You are under SafeMode ，please set PK for the table first");
	}
	else
	{
		//锁定旧列
		if (this->onInsertFlag == 0)
		{
			for (int i = 0; i<this->table->rowCount(); i++)
			{
				for (int j = 0; j<this->table->columnCount(); j++)
				{
					if (this->table->item(i, j) != NULL)
						this->table->item(i, j)->setFlags(Qt::NoItemFlags);
					//if (j == 1)
					//{
					//	this->table->cellWidget(i, j)->setDisabled(true);
					//}
				}
			}
			//进入新增状态
			this->onInsertFlag = 1;
		}
		//重设行数
		table->setRowCount((this->table->rowCount()) + 1);
	}
}
//加载表格数据
/*
	将传入的data以//进行分解
	重置表格行数
	通过行数与列数遍历表格设置数值
 */
void TableWidget::InitTableData(QStringList data,int sumRow,int sumCol)
{
    QStringList oneRow;
    qSumRow = sumRow;
    qSumCol = sumCol;
    table->setRowCount(sumRow);
    for(int i=0;i<sumRow;i++)
    {
        oneRow = data[i].split("//");
        for(int j=0;j<sumCol;j++)
        {
            qDebug("initing!!!! %d %d\n",i,j);
            
            table->setItem(i,j,new QTableWidgetItem(QString(oneRow[j])));
        }
    }
}
//刷新表格数据
/*
	获取主键名
	调用查询函数
	调用加载数据函数
 */
void TableWidget::RefreshTableData()
{
	if (this->onInsertFlag == 1)
	{
		QMessageBox::information(this, "Operation denied", "Please finish your insert operation first!");
	}
	else
	{
		QStringList real_head, real_data;
		QStringList mainKey, mainPos;
		int row, col;
		bool mytest;
		mytest = getMainKey(mainKey, tableName);
		mytest = selectTest(tableName, real_head, real_data, row, col, mainKey, mainPos);
		if(mytest)
			InitTableData(real_data, row, col);
		else
			QMessageBox::information(this, "ERROR!", GetErrorInfo());
	}
}

//更新数据
/*
	获取当前选中的行数和列数
	调用update函数更新数据库
	返回成功或错误信息
 */
void TableWidget::UpdateData(QModelIndex index)
{
	if (this->safeMode==1)
	{
		QMessageBox::information(this, "Operation denied", "You are under SafeMode , please set PK for the table first");
	}
	else
	{
		if (this->onInsertFlag != 1)
		{
			bool isOK;

			QString text = QInputDialog::getText(NULL, "Update", "update with new data",
				QLineEdit::Normal, index.data().toString(), &isOK);
			if (isOK)
			{
				int curRow, curCol;
				curCol = table->currentItem()->column();
				curRow = table->currentItem()->row();
				QString termQuery = "";
				for (int i = 0; i < mainKey.length(); i++)
				{
					termQuery += mainKey[i] + "='" + table->item(curRow, mainKeyPos[i].toInt())->text() + "'";
					if (i != mainKey.length() - 1)
						termQuery += " and ";
				}
				double timestamp;
				long affectedRows;
				//为了日志栏又组装了sql语句 是否还需要updateTest函数 直接使用execute函数？
				QString myquery = "update " + tableName + " set ";
				myquery = myquery + qHead[curCol] + "='" + text + "'" + " where " + termQuery + ";";
				string res = updateTest(tableName, qHead[curCol] + "='" + text + "'", termQuery, timestamp, affectedRows);
				if (res.compare("success") == 0)
				{
					this->catalogWidget->AddOneLogInfo(myquery, timestamp, affectedRows, true);
					table->setItem(curRow, curCol, new QTableWidgetItem(QString(text)));
				}
				else
				{
					this->catalogWidget->AddOneLogInfo(myquery, timestamp, 0, false);
					//QMessageBox::information(this, "ERROR!", QString::fromStdString(res));
				}
			}
		}
	}
}

void TableWidget::QSetErrorInfo(QString error)
{
    errorInfo = error;
}
//删除某行
/*
	获取用户选中的格子的行数
	提示用户是否确定删除本行
	组装删除语句
	调用执行sql语句函数进行数据删除
 */
void TableWidget::DeleteLine()
{
    int currentRow=0;
    bool isOK;
	if (this->safeMode == 1)
	{
		QMessageBox::information(this, "Operation denied", "You are under SafeMode , please set PK for the table first");
	}
	else
	{
		if (this->onInsertFlag == 1)
		{
			QMessageBox::information(this, "Operation denied", "Please finish your insert operation first!");
		}
		else
		{
			if (qSumRow != 0)
			{
				if (table->currentItem())
				{
					currentRow = table->currentItem()->row();
				}
				qDebug("%d cRow", currentRow);
				QString del;
				del = "are you sure to delete the Num." + QString::number(currentRow + 1, 10) + " line ?";
				QMessageBox::StandardButton rb = QMessageBox::question(NULL, "Delete", del, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				if (rb == QMessageBox::Yes)
				{
					qDebug("ok");
					QString myquery;
					QString termQuery = "";
					double timestamp;
					long affectedRows;
					for (int i = 0; i < mainKey.length(); i++)
					{
						termQuery += mainKey[i] + "='" + table->item(currentRow, mainKeyPos[i].toInt())->text() + "'";
						if (i != mainKey.length() - 1)
							termQuery += " and ";
					}
					myquery = "delete from " + tableName + " where " + termQuery + ";";
					string res = executeWithQuery(1, myquery, timestamp, affectedRows);

					if (res.compare("success") == 0)
					{
						qDebug("delete success");
						this->catalogWidget->AddOneLogInfo(myquery, timestamp, affectedRows, true);
						table->removeRow(currentRow);
						qSumRow--; 
					}
					else
					{
						this->catalogWidget->AddOneLogInfo(myquery, timestamp, affectedRows, false);
						QMessageBox::information(this, "ERROR!", QString::fromStdString(res));
					}
				}
			}
		}
	}
}
