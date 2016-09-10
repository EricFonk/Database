#include "export_and_import.h"
#include "ExportPart.h"
#include "ImportPart.h"
#include "qmessagebox.h"
#include "mysqlQuery.h"

Export_and_Import::Export_and_Import(QWidget *parent)
	: QWidget(parent)
{
	IP = new ImportPart();
	EP = new ExportPart();
	TabWidget_Main = new QTabWidget();
	WindowLayout = new QVBoxLayout(this);
	Label_Title = new QLabel(tr("Data Export And Import"));
	Label_Title->setStyleSheet("font:bold;font-size:21px");
	WindowLayout->addWidget(Label_Title);
	TabWidget_Main->addTab(EP->WholeExport, "Export Database");
	TabWidget_Main->addTab(IP->WholeImport, "Import Database");
	WindowLayout->addWidget(TabWidget_Main);

	this->setLayout(WindowLayout);
	this->setWindowTitle("Export And Import Databases");
}

Export_and_Import::~Export_and_Import()
{

}