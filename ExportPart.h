//
//  ExportPart.hpp
//
//  Created by Eric on 16/8/21.
//
//

#ifndef EXPORTPART_H
#define EXPORTPART_H

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>
#include <QtGui>
#include <qboxlayout.h>
#pragma pack(push,1)  
typedef struct database
{
	QString databasename;
	QStringList table;
	bool SelectStatus = false;
}database;
#pragma pack(pop) 
class ExportPart :public QWidget
{
	Q_OBJECT
public:
	ExportPart();
	void TableWidgetShowDatabasesInit();
	void FileCopy(QString TempFile, QString FinalFile);
	QString GetType();
public:
	QVBoxLayout *WholeWindow;
	QWidget *WholeExport;
	//显示数据库部分
	QGroupBox *GroupBox_TablesToExport;
	QHBoxLayout *TopLayout_1;
	QHBoxLayout *TopLayout_2;
	QVBoxLayout *TopLayout;
	QTableWidget *TableWidget_ShowDatabases;
	QTableWidget *TableWidget_ShowTables;
	QPushButton *PushBtn_Refresh;
	QComboBox *ComboBox_ExportType;
	QPushButton *PushBtn_SelectTable;
	QPushButton *PushBtn_UnselectAll;

	//导出对象部分
	QGroupBox *GroupBox_ObjectToExport;
	QHBoxLayout *MidLayout;
	QCheckBox *CheckBox_ExportSet1;//Dump Stored procedures and Functions
	QCheckBox *CheckBox_ExportSet2;// Dump Events
	QCheckBox *CheckBox_ExportSet3;//Dump Trigger

								   //导出设置部分
	QGroupBox *GroupBox_ExportOption;
	QHBoxLayout *BottomLayout;
	QLabel *Label_ExportFolder;
	QLineEdit *LineEdit_ExportPath;
	QPushButton *PushBtn_FileOpen;

	//导出按钮部分
	QHBoxLayout *OtherLayout;
	QLabel *Label_PromptStatement;
	QPushButton *PushBtn_StartExport;

	private slots:
	bool ExportConnectSQL();
	void Refresh();
	void SavePath();
	void GetTable(int row, int col);
	void GetSelectTable(int row, int col);
	void Export();
	void Select_AllTable();
	void UnSelect_AllTable();
};
#endif
