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
	this->setAttribute(Qt::WA_DeleteOnClose, true);
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
    QString myquery = "insert into "+tableName;
	QString selectedCols = "";
	QString insertVals = "";
    for(int i=0;i<qSumCol;i++)
    {
		if (table->item(0, i) != NULL)
		{
			selectedCols = selectedCols + "`" + this->qHead[i] + "`";
			insertVals = insertVals + "'" + table->item(0, i)->text() + "'";
			if (i != (qSumCol - 1)&&table->item(0,i+1)!=NULL)//不是最后一个且后面一个不为空
			{
				selectedCols = selectedCols + ",";
				insertVals = insertVals + ",";
			}
		}
    }
    myquery = myquery + "("+ selectedCols + ") " + "values(" + insertVals + ");";
    std::string res;
	double timestamp;
	long affectedRows;
    res = executeWithQuery(0,myquery,timestamp, affectedRows);
    if(res.compare("success")==0)
    {
        qDebug("insert success");
        //do sth
        //insert with query
        QMessageBox::information(this,"Insert Success!","Please refresh table");
		this->close();
    }
    else
    {
        QMessageBox::information(this,"ERROR!",QString::fromStdString(res));
    }
}