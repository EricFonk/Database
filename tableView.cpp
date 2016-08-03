//
//  tableView.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#include "tableView.h"
#include "mysqlQuery.h"
#include "insertTable.h"
#include "mylistview.h"
TableWidget::TableWidget()
{
    //插入数据按钮
    QPushButton *pushButton = new QPushButton(tr("insert"), this);
    pushButton->setGeometry(QRect(70, 40, 75, 23));
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
    //添加控件
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(table);
    mainLayout->addWidget(pushButton);
    mainLayout->addWidget(pushButton2);
    mainLayout->addWidget(pushButton3);
    this->setLayout(mainLayout);
    //点击某个单元格时触发更新操作
    connect(table, SIGNAL(clicked(QModelIndex)), this, SLOT(UpdateData(QModelIndex)));
    //点击新增按钮时向表格插入一行
    connect(pushButton, SIGNAL(clicked()), this, SLOT(InsertLine()));
    //点击删除按钮时获取选中行并将其删除
    connect(pushButton2, SIGNAL(clicked()), this, SLOT(DeleteLine()));
    //点击刷新按钮时刷新表格中所有数据
    //待改进 点击后判断数据是否变化并提示用户保存改动
    connect(pushButton3, SIGNAL(clicked()), this, SLOT(RefreshTableData()));
}
//设置表头
/*
	设置列个数 列名称
 */
void TableWidget::SetHeaders(QString table_name,int col,QStringList headers,QString main,int pos)
{
    tableName = table_name;
    mainKey = main;
    mainKeyPos = pos;
    qHead = headers;
    table->setColumnCount(col);
    table->setHorizontalHeaderLabels(headers);
}
//新增一行
/*
	加入InsertTable控件以完成新增行功能
	待完善
 */
void TableWidget::InsertLine()
{
    InsertTable *t = new InsertTable(tableName);
    t->show();
    t->SetHeaders(qSumCol,qHead);
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
    QStringList real_head,real_data;
    QString mainKey;
    int row,col,mainPos;
    bool mytest;
    mytest=getMainKey(mainKey,tableName);
    mytest=selectTest(tableName,real_head,real_data,row,col,mainKey,mainPos);
    InitTableData(real_data,row,col);
}

//更新数据
/*
	获取当前选中的行数和列数
	调用update函数更新数据库
	返回成功或错误信息
 */
void TableWidget::UpdateData(QModelIndex index)
{
    bool isOK;
    QString text = QInputDialog::getText(NULL, "Update","update with new data",
                                         QLineEdit::Normal, index.data().toString(), &isOK);
    if(isOK) {
        int curRow,curCol;
        curCol = table->currentItem()->column();
        curRow = table->currentItem()->row();
        string res=updateTest(tableName,qHead[curCol]+"='"+text+"'",mainKey+"='"+table->item(curRow,mainKeyPos)->text()+"'");
        if(res.compare("success")==0)
        {
            table->setItem(curRow,curCol,new QTableWidgetItem(QString(text)));
        }
        else
        {
            QMessageBox::information(this, "ERROR!", QString::fromStdString(res));
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
    if(table->currentItem())
    {
        currentRow = table->currentItem()->row();
    }
    qDebug("%d cRow",currentRow);
    QString del;
    del = "are you sure to delete the Num."+ QString::number(currentRow+1,10)+" line ?";
    QMessageBox::StandardButton rb = QMessageBox::question(NULL,"Delete",del,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        qDebug("ok");
        QString myquery;
        myquery ="delete from "+tableName+" where "+mainKey+"='"+table->item(currentRow,mainKeyPos)->text()+"';";
        string res=executeWithQuery(1,myquery);
        
        if(res.compare("success")==0)
        {
            qDebug("delete success");
            table->removeRow(currentRow);
        }
        else
        {
            QMessageBox::information(this, "ERROR!", QString::fromStdString(res)); 
        }
    }
}

