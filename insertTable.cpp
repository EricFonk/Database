//
//  insertTable.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#include "insertTable.h"
#include "mysql.h"
#include "mysqlQuery.h"
InsertTable::InsertTable(QString table_name)
{
    tableName = table_name;
    QPushButton *pushButton = new QPushButton(tr("insert"), this);
    pushButton->setGeometry(QRect(70,40,75,23));
    
    table = new QTableWidget(this);
    table->setColumnCount(1);
    table->setRowCount(1);
    QStringList headers;
    headers << "new Column";
    table->setHorizontalHeaderLabels(headers);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(table);
    mainLayout->addWidget(pushButton);
    this->setLayout(mainLayout);
    
    connect(pushButton, SIGNAL(clicked()),this,SLOT(ApplyInsert()));
}

void InsertTable::SetHeaders(int col,QStringList headers)
{
    qSumCol = col;
    qHead = headers;
    table->setColumnCount(col);
    table->setHorizontalHeaderLabels(headers);
}

void InsertTable::ApplyInsert()
{
    QString myquery = "insert into "+tableName+" values(";
    for(int i=0;i<qSumCol;i++)
    {
        myquery = myquery + "'" + table->item(0,i)->text() + "'";
        if(i!=(qSumCol-1))
        {
            myquery = myquery + ",";
        }
    }
    myquery = myquery + ");";
    std::string res;
    res = executeWithQuery(0,myquery);
    if(res.compare("success")==0)
    {
        qDebug("insert success");
        //do sth
        //insert with query
        QMessageBox::information(this,"Insert Success!","Please refresh table");
    }
    else
    {
        QMessageBox::information(this,"ERROR!",QString::fromStdString(res));
    }
}