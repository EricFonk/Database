//
//  table_alter.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/2.
//
//

#ifndef table_alter_h
#define table_alter_h

#include <QtWidgets>
#include <QtGui>
#include <QString>
#include <QStringList>
#include "Widget_ForeignKey.h"


class TableAlt:public QWidget
{
    Q_OBJECT
    
public:
	TableAlt();
	~TableAlt();
public:
	QTabWidget *_allTabOpts;
	QWidget *_alterCols;
	Widget_ForeignKey *_alterFK;
	QTableWidget *tableOpt;
	QLineEdit *newTableName;//表名输入框
	QLabel *dbNameLabel;//所属模式标签
	QTableWidgetItem *check1;
	QTableWidgetItem *check2;
	QTableWidgetItem *check3;
	QComboBox *datatype;

	QString dbName;//数据库名
	QString tName;//数据表名
	int columns_num;//列数量
	int onInsertFlag;//是否正在进行插入操作的标志
	QStringList col_name;//列名list
	QStringList col_default;//默认值list
	QStringList col_isNull;//空约束list
	QStringList col_type;//列数据类型list
	QStringList col_key;//键信息list
	QStringList col_extra;//自增等额外约束（目前只有自增）
	QStringList col_uni;//唯一约束
	QStringList col_unsign;
	QStringList col_zero;
	void setDTName(QString dbname, QString tname);
	void loadTableInfo();

	//回退对视图数据最近一次的加列操作
	void renameTable();
	//回退对视图数据最近一次的删列操作
	void rollBackLatestAddOperation(int oldColSum, int newColSum);
	//回退对视图数据最近一次的所有更新操作
	void rollBackLatestDropOperation(int tarColNum);
	//确认最近的新增列操作（在数据库完成实际操作后对逻辑模型修改）
	void confirmLatestAddOperation(int newColSum);
	//确认最近的删列操作（在数据库完成实际操作后对逻辑模型修改）
	void confirmLatestDropOperation(int tarColNum);
	//确认最近的列信息修改操作（在数据库完成实际操作后对逻辑模型修改）
	void confirmLatestUpdateOperation();

	void rollBackLatestUpdateOperation();
	void comfirmLatestOpt(int optType);
	void updateAllInfoAfter(QStringList colNumList,int optType);
	private slots:
		void onDataChange(const QModelIndex &topLeft, QModelIndex &buttomRight);
		void insertCol();
		void confirmInsertCol();
		void dropCol();
		void applyAlter(bool);
		void refreshAllInfo();
};

#endif /* table_alter_h */
