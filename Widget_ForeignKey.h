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

	bool _onInsertFlag;//���ڲ���״̬
	QStringList _tableHead;
	int _fkSum;//�������
	QString _dbName;//����������
	QString _tName;//��������
	QStringList _allFKName;//�����������
	QStringList _refTableName;//��������������������һһ��Ӧ��
	QStringList _refColumnName;//������������յ�����(�������һһ��Ӧ)
	QStringList _fkColumnName;//ÿ������ڱ����е�����(�������һһ��Ӧ)
	QStringList _allColumnName;//������������
	//QString sqlQuery;//ִ�����
	//float executeTime;//ִ�к�ʱ
	//long affectedRow;//Ӱ������
	//bool executeFlag;//�Ƿ���ȷִ��
	//QString errorInfo;//������Ϣ
public:
	void LoadRefTable(QString iDBName,QString iTableName,QStringList iAllColumnName);
	void LoadOneDBAllTables();//�������������ı���
	void LoadOneTableAllCols(int x, int y);//�����Ҳ������������


	private slots:
	void LoadRefColumn(int x,int y);
	void AddNewFK();
	void AlterOldFK();
	void DropOldFK();
};

#endif
