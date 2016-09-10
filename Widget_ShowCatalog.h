#pragma once

#ifndef Widget_ShowCatalog_H
#define Widget_ShowCatalog_H

#include <QtWidgets>
#include <QtGui>
#include <QString>
#include <QStringList>

class Widget_ShowCatalog :public QWidget
{
	Q_OBJECT

public:
	Widget_ShowCatalog();
	~Widget_ShowCatalog();
public:
	QTableWidget *tableShowLog;

	QStringList tableHead;
	//QString sqlQuery;//执行语句
	//float executeTime;//执行耗时
	//long affectedRow;//影响行数
	//bool executeFlag;//是否正确执行
	//QString errorInfo;//错误信息
public:
	void AddOneLogInfo(QString pISqlQuery,float pIExecuteTime,long pIAffectedRow,bool pIExecuteFlag);
	void RefreshLog();
};

#endif
