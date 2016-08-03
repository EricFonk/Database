//
//  mylistview.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#include "mylistview.h"

MyListView::MyListView()
{
    model = new QStringListModel(this);
    
    QStringList data;
    data << "Letter A" << "Letter B" << "Letter C";
    model->setStringList(data);
    
    listView = new QListView(this);
    listView->setModel(model);
    
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *insertBtn = new QPushButton(tr("insert"), this);
    QPushButton *delBtn = new QPushButton(tr("Delete"), this);
    QPushButton *showBtn = new QPushButton(tr("Show"), this);
    btnLayout->addWidget(insertBtn);
    btnLayout->addWidget(delBtn);
    btnLayout->addWidget(showBtn);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(listView);
    mainLayout->addLayout(btnLayout);
    this->setLayout(mainLayout);
    
    connect(insertBtn, SIGNAL(clicked()), this, SLOT(insertData()));
    connect(delBtn, SIGNAL(clicked()), this, SLOT(deleteData()));
    connect(showBtn, SIGNAL(clicked()), this, SLOT(showData()));
    
}

void MyListView::initData(QString test)
{
    qDebug("in\n");
    int row = 0;
    model->insertRows(row, 1);
    QModelIndex index = model->index(row);
    model->setData(index, test);
    listView->setCurrentIndex(index);
    listView->edit(index);
}

void MyListView::insertData()
{
    bool isOK;
    QString text = QInputDialog::getText(NULL, "Insert", "Please input new data:",
                                         QLineEdit::Normal, "You are inserting new data.", &isOK);
    if(isOK) {
        int row = listView->currentIndex().row();
        model->insertRows(row, 1);
        QModelIndex index = model->index(row);
        model->setData(index, text);
        listView->setCurrentIndex(index);
        listView->edit(index);
    }
}

void MyListView::deleteData()
{
    if(model->rowCount() > 1) {
        model->removeRows(listView->currentIndex().row(), 1);
    }
}

void MyListView::showData()
{
    QStringList data = model->stringList();
    QString str;
    foreach(QString s, data) {
        str += s + "\n";
    }
    
    QMessageBox::information(this, "Data", str);
}