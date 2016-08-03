#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    
    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QStringLiteral("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    
    leftTabWidg=new QTabWidget(centralwidget);
    leftTabWidg->setObjectName(QStringLiteral("leftTabWidg"));
    leftTabWidg->setMinimumSize(400, 450);
    leftTabWidg->setTabEnabled(0, true);
    leftTabWidg->setTabEnabled(1, true);
    leftTabWidg->setTabBarAutoHide(false);
    
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(leftTabWidg->sizePolicy().hasHeightForWidth());
    leftTabWidg->setSizePolicy(sizePolicy1);
    
    manageTab = new QWidget();
    manageTab->setObjectName(QStringLiteral("ManageTab"));
    QVBoxLayout *manageLayout=new QVBoxLayout(manageTab);
    manageTree=new QTreeWidget();
    manageTree->setObjectName(QStringLiteral("leftTreeWidg"));
    manageTree->setMinimumSize(300, 400);
    manageLayout->addWidget(manageTree);
    manageTab->setLayout(manageLayout);
    
    schemaTab = new QWidget(centralwidget);
    schemaTab->setObjectName(QStringLiteral("schemaTab"));
    QVBoxLayout *schemaLayout=new QVBoxLayout(schemaTab);
    schemaTree=new QTreeWidget();
    schemaTree->setObjectName(QStringLiteral("schemaTree"));
    schemaTree->setMinimumSize(300, 400);
    schemaLayout->addWidget(schemaTree);
    schemaTab->setLayout(schemaLayout);

    leftTabWidg->addTab(manageTab, QString("Manage"));
    leftTabWidg->addTab(schemaTab, QString("Schema"));
    
    gridLayout->addWidget(leftTabWidg, 0, 0, 1, 1);
    
    rightWidget=new QWidget();
    QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(rightWidget->sizePolicy().hasHeightForWidth());
    rightWidget->setSizePolicy(sizePolicy2);
    
    dataView=new QTableView(centralwidget);
    dataView->setObjectName(QStringLiteral("dataView"));
    dataView->setMinimumHeight(300);
    
    rightLayout=new QVBoxLayout();
    rightLayout->addWidget(dataView);
    rightWidget->setLayout(rightLayout);
    
    gridLayout->addWidget(rightWidget, 0, 1, 1, 1);
    
    this->setCentralWidget(centralwidget);
    resize(850, 450);
}

MainWindow::~MainWindow()
{

}

