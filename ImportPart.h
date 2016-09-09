//
//  ImportPart.hpp
//
//  Created by Eric on 16/8/22.
//
//
#ifndef IMPORTPART_H
#define IMPORTPART_H
#include <QtGui>
#include <qcheckbox>
#include <qtableview>
#include <qlabel>
#include <qwidget>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qboxlayout.h>

class ImportPart :public QWidget
{
	Q_OBJECT
public:
	ImportPart();
	void ComboBoxImportDesInit();
public:
	//导入整体
	QVBoxLayout *ImportLayout;
	QWidget *WholeImport;

	//导入地址
	QHBoxLayout *FirstLayout;
	QLabel *Label_FilePath;
	QLineEdit *LineEdit_FilePath;
	QPushButton *PushButton_FileChosen;

	//导入数据库
	QHBoxLayout *SecondLayout;
	QLabel *Label_ImportDes;
	QComboBox *ComboBox_ImportDes;
	QLabel *Label_NewImprtDes;
	QLineEdit *LineEdit_NewImportDes;

	//提示部分
	QVBoxLayout *ThirdLayout;//包括tableview
	QLabel *Label_LOG;

	//导入情况显示
	QTableView *TableView_Show;

	//导入按钮
	QHBoxLayout *FourthLayout;
	QLabel *Label_PromotStatement;
	QPushButton *PushButton_StartImport;

	public slots:
	bool ImportConnectSQL();
	void SetImportDes();
	void ImportPath();
	void Import();
};
#endif //IMPORTPART_H