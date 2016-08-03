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
    QPushButton *cfmDropColBtn = new QPushButton(tr("confirm drop column"),this);
    QPushButton *applyBtn = new QPushButton(tr("apply alter"),this);//提交按钮
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    mainLayout->addWidget(tableOpt);
    mainLayout->addWidget(addColumnBtn);
    mainLayout->addWidget(dropColumnBtn);
    mainLayout->addWidget(cfmAddColBtn);
    mainLayout->addWidget(cfmDropColBtn);
    mainLayout->addWidget(tableName);
    mainLayout->addWidget(newTableName);
    mainLayout->addWidget(dbLabel);
    mainLayout->addWidget(dbNameLabel);
    mainLayout->addWidget(applyBtn);
    this->setLayout(mainLayout);
    //connect(insertBtn, static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked),
    //	this, static_cast<void(TableCrtAlt::*)(bool)>(&TableCrtAlt::applyCreate));
    connect(applyBtn,SIGNAL(clicked(bool)),this,SLOT(applyAlter(bool)));
    connect(addColumnBtn,SIGNAL(clicked()),this,SLOT(insertCol()));
    connect(cfmAddColBtn,SIGNAL(clicked()),this,SLOT(confirmInsertCol()));
    //connect(tableOpt->model(),SIGNAL(modelDataChanged(const QModelIndex &topLeft, QModelIndex &buttomRight)),this,SLOT(onDataChange(const QModelIndex &topLeft, QModelIndex &buttomRight)));
}

void TableAlt::setDTName(QString dbname,QString tname)
{
    this->dbName = dbname;
    this->tName = tname;
}

void TableAlt::loadTableInfo()
{
    newTableName->setText(tName);
    dbNameLabel->setText(dbName);
    getTableAllCols(tName,col_name,col_default,col_isNull,col_type,col_key,col_extra);
    this->columns_num = col_name.length();
    this->onInsertFlag = 0;
    tableOpt->setRowCount(columns_num);
    QTableWidgetItem *check1 = new QTableWidgetItem[columns_num];
    QTableWidgetItem *check2 = new QTableWidgetItem[columns_num];
    QTableWidgetItem *check3 = new QTableWidgetItem[columns_num];
    QComboBox *datatype = new QComboBox[columns_num];
    QStringList datalist;
    datalist<<"int"<<"varchar(45)"<<"char(45)"<<"float"<<"double"<<"boolean"<<"tinyint"<<"smallint"<<"bigint"<<"tinyint"<<"datetime";
    
    for(int i=0;i<columns_num;i++)
    {
        tableOpt->setItem(i,0,new QTableWidgetItem(col_name[i]));
        tableOpt->setItem(i,8,new QTableWidgetItem(col_default[i]));
        //初始化下拉列表
        datatype[i].addItems(datalist);
        tableOpt->setCellWidget(i,1,&datatype[i]);
        
        if(col_isNull[i].compare("NO")==0)
        {
            check1[i].setCheckState(Qt::Checked);
        }
        else
        {
            check1[i].setCheckState(Qt::Unchecked);
        }
        if(col_key[i].compare("PRI")==0)
        {
            check2[i].setCheckState(Qt::Checked);
        }
        else
        {
            check2[i].setCheckState(Qt::Unchecked);
        }
        if(col_extra[i].compare("auto_increment")==0)
        {
            check3[i].setCheckState(Qt::Checked);
        }
        else
        {
            check3[i].setCheckState(Qt::Unchecked);
        }
        
        tableOpt->setItem(i,3,&check1[i]);
        tableOpt->setItem(i,2,&check2[i]);
        tableOpt->setItem(i,7,&check3[i]);
    }
}

void TableAlt::onDataChange(const QModelIndex &topLeft, QModelIndex &buttomRight)
{
    qDebug("alter hhhhhhhhhhhhhhhhhhhhhhhhhhhhhh!");
}

void TableAlt::applyAlter(bool param)
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
    
    for(int i=0;i<new_col_num;i++)
    {
        flag_hasAlterItem=false;
        oneQuery += " ";
        for(int j=0;j<8;j++)
        {
            pkQuery = "";
            if(tableOpt->item(i,j)->text()==NULL)
            {
                //给出不能为空的错误提示
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
                            pkQuery = ",add primary key ('"+col_name[i]+"')";
                        }
                        if((!this->tableOpt->item(i,j)->checkState())&&(col_key[i].compare("PRI")!=0))
                        {
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
                            flag_hasAlterItem = true;
                            new_col_isNull = "NOT NULL";
                        }
                        break;
                    case 8:
                        //添加自增约束
                        if((this->tableOpt->item(i,j)->checkState()!=Qt::Checked)&&(col_extra[i].compare("auto_increment")==0))
                        {
                            flag_hasAlterItem = true;
                            new_col_extra = "AUTO_INCREMENT";
                        }
                        //删除自增约束
                        if((this->tableOpt->item(i,j)->checkState()==Qt::Checked)&&(col_extra[i].compare("auto_increment")!=0))
                        {
                            flag_hasAlterItem = true;
                            new_col_extra = " ";
                        }
                        break;
                }
            }
        }
        if(flag_hasAlterItem)//列信息有所改动
        {
            //将行号加入有改动的行list
            tarColNum<<""+i;
            //把关于一行的修改sql语句组装起来
            oneQuery.push_back("CHANGE `"+col_name[i]+"` `"+new_col_name+"` "+new_col_type+" "+new_col_isNull+" "+new_col_extra);
        }
        //this->tableOpt->indexAt();
    }
    if(oneQuery.length()!=0)
    {
        //是直接调用刷新函数
        loadTableInfo();
        //还是手动刷新视图
        //updateAllInfoAfter(tarColNum,2);
        
        alterQuery = "ALTER TABLE "+tName;
        for(int i=0;i<oneQuery.length();i++)
        {
            if(i==0)
            {
                alterQuery += " ";
            }
            else
            {
                alterQuery += ",";
            }
            alterQuery += oneQuery[i];
        }
        //直接组装好语句还是设置好参数调用函数
        executeWithQuery(0,alterQuery);
    }
}


//修改表名
void TableAlt::renameTable()
{
    //获取旧表名和新表名 调用修改表名函数
    QString new_TName;
    QString flag;
    new_TName = this->newTableName->text();
    flag = AlterTable_RenameTable(tName,new_TName);
    if(flag.compare("OK")!=0)
    {
        QMessageBox::information(this,"Rename Table",flag);
    }
    else
    {
        QMessageBox::information(this,"Rename Table","Rename Table Successed!");
    }
}

//修改

void TableAlt::insertCol()
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

void TableAlt::confirmInsertCol()
{
    QStringList allColNames,allColTypes,allColOpts,allPkSets;
    QStringList allPkOpts,allIndexOpts;//主键约束（只有列名）索引约束（只有列名）
    QString oneOpt;
    int startLine,endLine;
    
    startLine=columns_num;
    endLine=this->tableOpt->rowCount();
    
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
    QString resFlag = AlterTable_AddColumns(dbName,tName,allColNames,allColTypes,allColOpts,allPkOpts,allIndexOpts);
    if(resFlag.compare("success")==0)
    {
        //解除旧列锁定
        for(int i=0;i<this->tableOpt->rowCount();i++)
        {
            for(int j=0;j<this->tableOpt->columnCount();j++)
            {
                if(this->tableOpt->item(i,j)!=NULL)
                    this->tableOpt->item(i,j)->setFlags(Qt::ItemIsEnabled);
                if(j==1)
                {
                    this->tableOpt->cellWidget(i,j)->setDisabled(false);
                }
            }
        }
        //取消新增状态
        this->onInsertFlag=0;
        //是直接调用刷新函数
        //this->loadTableInfo();
        //还是手动刷新视图
        //updateAllInfoAfter(tarColNum,0);
    }
    else
    {
        //打印错误信息
    }
}

void TableAlt::dropCol()
{
    //获取列名 调用删列函数 返回提示信息
    QString drop_colName;
    QString flag;
    QStringList tarColNum;
    
    tarColNum<<""+this->tableOpt->currentRow();
    drop_colName = this->tableOpt->item(this->tableOpt->currentRow(),0)->text();
    flag = AlterTable_DropColumn(tName,drop_colName);
    if(flag.compare("OK")!=0)
    {
        QMessageBox::information(this,"Drop Column Failed!",flag);
    }
    else
    {
        QMessageBox::information(this,"Drop Column Success!","Please refresh table");
        //是直接调用刷新函数
        loadTableInfo();
        //还是手动刷新视图
        //updateAllInfoAfter(tarColNum,1);
    }
}

//更新tablealt中的原数据（上一次操作后的数据） 以便在更改时判断那些列被更改了
void TableAlt::updateAllInfoAfter(QStringList colNumList,int optType)
{
    int oldColSum,newColSum;
    
    oldColSum = this->columns_num;
    newColSum = this->tableOpt->rowCount();
    
    switch(optType)
    {
            //添加列之后更新原数据逻辑模型
        case 0:
            for(int i=oldColSum;i<newColSum;i++)
            {
                //需不需要进行非空判断？
                col_name<<this->tableOpt->item(i,0)->text();
                col_default<<this->tableOpt->item(i,9)->text();
                col_extra<<this->tableOpt->item(i,8)->text();
                if(this->tableOpt->item(i,3)->checkState()==Qt::Checked)
                {
                    col_isNull<<"NOT NULL";
                }
                else
                {
                    col_isNull<<"NULL";
                }
                QWidget *temp = tableOpt->cellWidget(i,1);
                QComboBox *tempCB = (QComboBox *)temp;
                col_type<<tempCB->currentText();
                if(this->tableOpt->item(i,2)->checkState()==Qt::Checked)
                {
                    col_key<<"PRI";
                }
                else
                {
                    col_key<<"";
                }
            }
            this->columns_num = this->tableOpt->rowCount();
            //将新列加入视图
            break;
            //删除列之后更新原数据逻辑模型
        case 1:
            this->columns_num = this->tableOpt->rowCount();
            //根据行号删除所有原数据对象中对应的数据
            //sizeof能否获取数组长度？？
            for(int i=0;i<colNumList.length();i++)
            {
                col_name.removeAt(colNumList[i].toInt());
                col_default.removeAt(colNumList[i].toInt());
                col_isNull.removeAt(colNumList[i].toInt());
                col_type.removeAt(colNumList[i].toInt());
                col_key.removeAt(colNumList[i].toInt());
                col_extra.removeAt(colNumList[i].toInt());
            }		
            //将旧列删除视图
            break;
            //更新列信息之后更新原数据逻辑模型
        case 2:
            //根据行号更新所有原数据对象中对应的数据
            //sizeof能否获取数组长度？？
            for(int i=0;i<sizeof(colNumList);i++)
            {
                col_name.replace(i,this->tableOpt->item(colNumList[i].toInt(),0)->text());
                col_extra.replace(i,this->tableOpt->item(colNumList[i].toInt(),8)->text());
                col_default.replace(i,this->tableOpt->item(colNumList[i].toInt(),9)->text());
                if(this->tableOpt->item(i,3)->checkState()==Qt::Checked)
                {
                    col_isNull.replace(i,"NOT NULL");
                }
                else
                {
                    col_isNull.replace(i,"NULL");
                }
                QWidget *temp = tableOpt->cellWidget(i,1);
                QComboBox *tempCB = (QComboBox *)temp;
                col_type.replace(i,tempCB->currentText());
                if(this->tableOpt->item(i,2)->checkState()==Qt::Checked)
                {
                    col_key.replace(i,"PRI");
                }
                else
                {
                    col_key.replace(i,"");
                }
            }
            //对修改过的列更新视图
            break;
    }
}

