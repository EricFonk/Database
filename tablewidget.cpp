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
    //�������ݰ�ť
    QPushButton *pushButton = new QPushButton(tr("insert"), this);
    pushButton->setGeometry(QRect(70, 40, 75, 23));
    //ɾ��ĳ�а�ť
    QPushButton *pushButton2 = new QPushButton(tr("delete"), this);
    pushButton2->setGeometry(QRect(70, 40, 75, 23));
    //ˢ�±����ݰ�ť
    QPushButton *pushButton3 = new QPushButton(tr("refresh"), this);
    pushButton2->setGeometry(QRect(70, 40, 75, 23));
    //��ʼ�����ؼ�
    table = new QTableWidget(this);
    table->setColumnCount(1);
    table->setRowCount(1);
    //��ʼ����ͷ
    QStringList headers;
    headers << "new Column";
    table->setHorizontalHeaderLabels(headers);
    //��ӿؼ�
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(table);
    mainLayout->addWidget(pushButton);
    mainLayout->addWidget(pushButton2);
    mainLayout->addWidget(pushButton3);
    this->setLayout(mainLayout);
    //���ĳ����Ԫ��ʱ�������²���
    connect(table, SIGNAL(clicked(QModelIndex)), this, SLOT(UpdateData(QModelIndex)));
    //���������ťʱ�������һ��
    connect(pushButton, SIGNAL(clicked()), this, SLOT(InsertLine()));
    //���ɾ����ťʱ��ȡѡ���в�����ɾ��
    connect(pushButton2, SIGNAL(clicked()), this, SLOT(DeleteLine()));
    //���ˢ�°�ťʱˢ�±������������
    //���Ľ� ������ж������Ƿ�仯����ʾ�û�����Ķ�
    connect(pushButton3, SIGNAL(clicked()), this, SLOT(RefreshTableData()));
}
//���ñ�ͷ
/*
	�����и��� ������
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
//����һ��
/*
	����InsertTable�ؼ�����������й���
	������
 */
void TableWidget::InsertLine()
{
    InsertTable *t = new InsertTable(tableName);
    t->show();
    t->SetHeaders(qSumCol,qHead);
}
//���ر������
/*
	�������data��//���зֽ�
	���ñ������
	ͨ�������������������������ֵ
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
//ˢ�±������
/*
	��ȡ������
	���ò�ѯ����
	���ü������ݺ���
 */
void TableWidget::RefreshTableData()
{
    QStringList real_head,real_data;
    QString mainKey;
    int row,col,mainPos;
    bool mytest;
    mytest=GetMainKey(mainKey,tableName);
    mytest=SelectTest(tableName,real_head,real_data,row,col,mainKey,mainPos);
    InitTableData(real_data,row,col);
}

//��������
/*
	��ȡ��ǰѡ�е�����������
	����update�����������ݿ�
	���سɹ��������Ϣ
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
        string res=UpdateTest(tableName,qHead[curCol]+"='"+text+"'",mainKey+"='"+table->item(curRow,mainKeyPos)->text()+"'");
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
//ɾ��ĳ��
/*
	��ȡ�û�ѡ�еĸ��ӵ�����
	��ʾ�û��Ƿ�ȷ��ɾ������
	��װɾ�����
	����ִ��sql��亯����������ɾ��
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
        QString myQuery;
        myQuery ="delete from "+tableName+" where "+mainKey+"='"+table->item(currentRow,mainKeyPos)->text()+"';";
        string res=ExecuteWithQuery(1,myQuery);
        
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
