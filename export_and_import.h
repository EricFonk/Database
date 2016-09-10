#ifndef EXPORT_AND_IMPORT_H
#define EXPORT_AND_IMPORT_H

#include <QtWidgets/QMainWindow>
#include "ExportPart.h"
#include "ImportPart.h"
class Export_and_Import : public QWidget
{
	Q_OBJECT

public:
	Export_and_Import(QWidget *parent = 0);
	~Export_and_Import();

private:
	QLabel *Label_Title;
	QTabWidget *TabWidget_Main;
	QVBoxLayout *WindowLayout;
	ImportPart *IP;
	ExportPart *EP;
	public slots:
};

#endif // EXPORT_AND_IMPORT_H
