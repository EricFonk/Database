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
	//QString sqlQuery;//ִ�����
	//float executeTime;//ִ�к�ʱ
	//long affectedRow;//Ӱ������
	//bool executeFlag;//�Ƿ���ȷִ��
	//QString errorInfo;//������Ϣ
public:
	void AddOneLogInfo(QString pISqlQuery,float pIExecuteTime,long pIAffectedRow,bool pIExecuteFlag);
	void RefreshLog();
};

#endif
