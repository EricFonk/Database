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
#include "Widget_ShowCatalog.h"


class TableWidget : public QWidget
{
    Q_OBJECT
public:
    TableWidget();
	void SetHeaders(QString table_name, int col, QStringList headers, QStringList main, QStringList pos);//设置表头
    void InitTableData(QStringList,int,int);//加载表格数据
    void QSetErrorInfo(QString);//设置错误信息
public:
    QTableWidget *table;
	Widget_ShowCatalog *catalogWidget;
	QVBoxLayout *mainLayout;
    QPushButton *pushButton;
    QString tableName,errorInfo;//表名 错误信息
	QStringList mainKey, mainKeyPos;//主键名 主键位置
    int qSumRow,qSumCol,onInsertFlag,safeMode;//总共行数 总共列数 插入状态 安全模式（由于没有设置主键禁止用户在表格中增删改）
    QStringList qHead,updateQuery;//所有列信息 更新query
    private slots:
	//void testSlot(int row, int col);
    void InsertLine();
	void ConfirmInsert();
    void UpdateData(QModelIndex);
    void DeleteLine();
    void RefreshTableData();
};

#endif /* tablewidget_h */
