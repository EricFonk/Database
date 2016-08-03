//
//  tablewidget.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#ifndef tablewidget_h
#define tablewidget_h

#include <QtWidgets>
#include <QtGui>
#include <QString>



class TableWidget : public QWidget
{
    Q_OBJECT
public:
    TableWidget();
    void SetHeaders(QString,int,QStringList,QString,int);//设置表头
    void InitTableData(QStringList,int,int);//加载表格数据
    void QSetErrorInfo(QString);//设置错误信息
public:
    QTableWidget *table;
    QPushButton *pushButton;
    QString tableName,mainKey,errorInfo;//表名 主键名 错误信息
    int mainKeyPos,qSumRow,qSumCol;//主键位置 总共行数 总共列数
    QStringList qHead,updateQuery;//所有列信息 更新query
    private slots:
    void InsertLine();
    void UpdateData(QModelIndex);
    void DeleteLine();
    void RefreshTableData();
};

#endif /* tablewidget_h */
