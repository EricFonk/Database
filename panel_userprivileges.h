#ifndef PANEL_USERPRIVILEGES_H
#define PANEL_USERPRIVILEGES_H

#include"qdialog.h"
#include"qwidget.h"
#include"QVBoxLayout"
#include"QHBoxLayout"
#include"QGridLayout"
#include"qlabel.h"
#include"qtableview.h"
#include"qpushbutton.h"
#include"qcombobox.h"
#include"qcheckbox.h"
#include"qlineedit.h"

class Panel_UserPrivileges : public QWidget
{
    Q_OBJECT
    
public:
    Panel_UserPrivileges(QWidget *parent = 0);
    ~Panel_UserPrivileges();
    
public:
	//左侧部分
	QVBoxLayout *leftVBoxLayout;
	QHBoxLayout *bottomLeftHBoxLayout;
	QLabel *labelUserAccount;
	QTableView *tableView;
	QPushButton *ptnAdd;
	QPushButton *ptnDelete;
	QPushButton *ptnRefresh;
	
	//右侧部分
	QTabWidget *tabWidget;
	//tab1
	QWidget *tab1;
	QWidget *tab1LayoutWidget;
	QGridLayout *tab1GridLayout;
	QVBoxLayout *rightVBoxLayout;
	QLabel *labelLoginName;
	QLabel *labelAuthentication;
	QLabel *labelLimitToHost;
	QLabel *labelPassword;
	QLabel *labelConfirmPassword;
	QLineEdit *ledName;
	QComboBox *comboBoxPasswordType;
	QLineEdit *ledHost;
	QLineEdit *ledPassword;
	QLineEdit *ledConfirm;
	QPushButton *ptnExpire;
	QPushButton *ptnRevert;
	QPushButton *ptnApply;
	//tab2
	QWidget *tab2;
	QWidget *tab2LayoutWidget;
	QGridLayout *tab2GridLayout;
	QHBoxLayout *tab2HBoxLayout;
	QLabel *labelMaxQueries;
	QLabel *labelNumberOfQueries;
	QLabel *labelMaxUpdates;
	QLabel *labelNumberOfUpdates;
	QLabel *labelMaxConnections;
	QLabel *labelNumberOfTimes;
	QLabel *labelCurrentConnections;
	QLabel *labelNumberOfConnections;
	QLineEdit *ledQueries;
	QLineEdit *ledUpdates;
	QLineEdit *ledConnections;
	QLineEdit *ledCurrentcon;
	QPushButton *ptnRevert2;
	QPushButton *ptnApply2;
	//tab3
	QWidget *tab3;
	QWidget *tab3LayoutWidget;
	QGridLayout *tab3GridLayout;
	QHBoxLayout *tab3HBoxLayout1;
	QHBoxLayout *tab3HBoxLayout2;
	QVBoxLayout *tab3VBoxLayout;
	QCheckBox *cboxDelete;
	QCheckBox *cboxDrop;
	QCheckBox *cboxEvent;
	QCheckBox *cboxExecute;
	QCheckBox *cboxFile;
	QCheckBox *cboxOption;
	QCheckBox *cboxIndex;
	QCheckBox *cboxInsert;
	QCheckBox *cboxLockTables;
	QCheckBox *cboxProcess;
	QCheckBox *cboxReferences;
	QCheckBox *cboxReload;
	QCheckBox *cboxClient;
	QCheckBox *cboxSlave;
	QCheckBox *cboxSelect;
	QCheckBox *cboxShowDatabases;
	QCheckBox *cboxShowView;
	QCheckBox *cboxShutDown;
	QCheckBox *cboxSuper;
	QCheckBox *cboxTrigger;
	QCheckBox *cboxUpdate;
	QCheckBox *cboxTemporaryTables;
	QCheckBox *cboxCreateUser;
	QCheckBox *cboxCreateView;
	QCheckBox *cboxAlter;
	QCheckBox *cboxAlterRountine;
	QCheckBox *cboxCreate;
	QCheckBox *cboxCreateRoutine;
	QCheckBox *cboxCreateTableSpace;
	QPushButton *ptnRevoke;
	QPushButton *ptnRevert3;
	QPushButton *ptnApply3;
	//tab4
	QWidget *tab4;
	QWidget *tab4LayoutWidget;
	QGridLayout *tab4GridLayout;
	QHBoxLayout *tab4HBoxLayout1;
	QHBoxLayout *tab4HBoxLayout2;
	QHBoxLayout *tab4HBoxLayout3;
	QHBoxLayout *tab4HBoxLayout0;
	QVBoxLayout *tab4VBoxLayout1;
	QVBoxLayout *tab4VBoxLayout2;
	QTableView *tab4TableView;
	QPushButton *tab4PtnAddEntry;
	QPushButton *tab4PtnDeleteEntry;
	QPushButton *tab4PtnRevokeAllPriv;
	QPushButton *tab4PtnUnselectAll;
	QPushButton *tab4PtnSelctAll;
	QPushButton *ptnApply4;
	QPushButton *ptnRevert4;
	QLabel *labelSelectSchema;
	QComboBox *tab4ComboBoxSchema;
	//Object Rights
	QCheckBox *tab4CboxSelect;
	QCheckBox *tab4CboxInsert;
	QCheckBox *tab4CboxUpdate;
	QCheckBox *tab4CboxDelete;
	QCheckBox *tab4CboxExecute;
	QCheckBox *tab4CboxShowView;
	//DDL Rights
	QCheckBox *tab4CboxCreate;
	QCheckBox *tab4CboxAlter;
	QCheckBox *tab4CboxReferences;
	QCheckBox *tab4CboxIndex;
	QCheckBox *tab4CboxCreateView;
	QCheckBox *tab4CboxCreateRoutine;
	QCheckBox *tab4CboxAlterRoutine;
	QCheckBox *tab4CboxEvent;
	QCheckBox *tab4CboxDrop;
	QCheckBox *tab4CboxTrigger;
	//Other Rights
	QCheckBox *tab4CboxGrantOption;
	QCheckBox *tab4CboxCreateTempTables;
	QCheckBox *tab4CboxLockTables;

	public slots:
	void Refresh_Slot();
	void Show_User_Slot();
	void Create_User_Slot();
	void Add_Acount_Slot();
	void Revert_Slot();
	void Expire_Password_Slot();
	void Delete_Slot();
	void Show_Privileges_Slot();
	void Revoke_AllPrivileges_Slot();
	void Authority_Management_Slot();
	void Revert_CheckBox_Slot();
	void User_Resource_Slot();
	void Revert_User_Resource_Slot();
	void Set_User_Resource_Slot();
	//tab4
	void Query_Schema_Slot();
	void Add_Entry_Slot();
	void Set_Schema_Privileges_Slot();
	void Select_All_Slot();
	void Unselect_All_Slot();
	void Delete_Entry_Slot();
	void Revoke_Schema_Privileges_Slot();
	void Show_Schema_Privileges_Slot();
	void Revert_Schema_Slot();
};

#endif // PANEL_USERPRIVILEGES_H
