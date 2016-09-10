//
//  schemas_test.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#include "schemas_test.h"
#include "mysqlQuery.h"

SchemasTest::SchemasTest()
{
    model = new QStringListModel(this);
    
    QStringList data;
    data<<"";
    model->setStringList(data);
    
    listView = new QListView(this);
    listView->setModel(model);
    
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *insertBtn = new QPushButton(tr("CreateDatabase"),this);
    QPushButton *dropBtn = new QPushButton(tr("DropDatabase"),this);
    btnLayout->addWidget(insertBtn);
    btnLayout->addWidget(dropBtn);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(listView);
    mainLayout->addLayout(btnLayout);
    this->setLayout(mainLayout);
    
    connect(insertBtn,SIGNAL(clicked()),this,SLOT(createDatabase()));
    connect(dropBtn,SIGNAL(clicked()),this,SLOT(dropDatabase()));
}

void SchemasTest::initData(QStringList list)
{
    allDB = list;
    model->setStringList(list);
}

void SchemasTest::createDatabase()
{
    bool isOK;
    QString text = QInputDialog::getText(NULL, "Update","update with new data",
                                         QLineEdit::Normal, "", &isOK);
    if(isOK) {
        string res=CreateDBTest(text);
        if(res.compare("success")==0)
        {
            allDB<<text;
            int row = model->rowCount();
            model->insertRows(row, 1);
            QModelIndex index = model->index(row);
            model->setData(index, text);
            listView->setCurrentIndex(index);
            //listView->edit(index);
        }
        else
        {
            QMessageBox::information(this, "ERROR!", QString::fromStdString(res));
        }
    }
}

void SchemasTest::dropDatabase()
{
    int row = listView->currentIndex().row();
    QString drop = "are you sure to drop database "+allDB[row]+"  ?";
    QMessageBox::StandardButton rb = QMessageBox::question(NULL,"Drop",drop,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QString oldDB = allDB[row];
        string res = dropDBTest(oldDB);
        if(res.compare("success")==0)
        {
            model->removeRows(row, 1); 
        }
        else
        {
            QMessageBox::information(this, "ERROR!", QString::fromStdString(res)); 	
        }
    }
}
