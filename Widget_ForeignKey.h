#pragma once

#ifndef Widget_ForeignKey_H
#define Widget_ForeignKey_H

#include <QtWidgets>
#include <QtGui>
#include <QString>
#include <QStringList>

class Widget_ForeignKey :public QWidget
{
	Q_OBJECT

public:
	Widget_ForeignKey();
	~Widget_ForeignKey();
protected:
	QTableWidget *_tableManageRefTable;
	QTableWidget *_tableManageRefColumn;

	bool _onInsertFlag;//正在插入状态
	QStringList _tableHead;
	int _fkSum;//外键总数
	QString _dbName;//本表所属库
	QString _tName;//本表名字
	QStringList _allFKName;//所有外键名字
	QStringList _refTableName;//所有外键表名（与外键数一一对应）
	QStringList _refColumnName;//所有外键所参照的列名(与外键数一一对应)
	QStringList _fkColumnName;//每个外键在本表中的列名(与外键数一一对应)
	QStringList _allColumnName;//本表所有列名
	//QString sqlQuery;//执行语句
	//float executeTime;//执行耗时
	//long affectedRow;//影响行数
	//bool executeFlag;//是否正确执行
	//QString errorInfo;//错误信息
public:
	void LoadRefTable(QString iDBName,QString iTableName,QStringList iAllColumnName);
	void LoadOneDBAllTables();//加载左侧下拉框的表名
	void LoadOneTableAllCols(int x, int y);//加载右侧下拉框的列名


	private slots:
	void LoadRefColumn(int x,int y);
	void AddNewFK();
	void AlterOldFK();
	void DropOldFK();
};

#endif
