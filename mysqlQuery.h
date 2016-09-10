//
//  mysqlQuery.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/3.
//
//

#ifndef mysqlQuery_h
#define mysqlQuery_h

//#include "mysqlQuery.h"
//#include <WinSock.h>
#include <QString>
#include "mysql.h"
#include "tablewidget.h"
#include <string>

using namespace std;

QString GetErrorInfo();
bool ConnectDatabase();

QStandardItemModel* getConnections(QStandardItemModel *model);
char* getOneVariable(std::string variable);

bool getMainKey(QStringList &mainKey,QString table_name);
bool selectTest(QString,QStringList&,QStringList&,int&,int&,QStringList,QStringList&);
string updateTest(QString,QString,QString,double&,long&);
string executeWithQuery(int,QString,double&,long&);
bool GetAllDatabases(QStringList&);
string CreateDBTest(QString);
string dropDBTest(QString);
string GetAllTables(QStringList&,QString);
string createNewTable(QString,QString,QStringList,QString,QStringList);
string dropTableWithQuery(QString query);
void getTableAllCols(QString dbName,QString tableName,QStringList &col_name,QStringList &col_default,QStringList &col_isNull,QStringList &col_type,QStringList &col_key,QStringList &col_extra,QStringList &col_uni,QStringList &col_unsign,QStringList &col_zero);
string create_function(QString dbName,QString func_info);
string drop_function(QString dbName,QString func_name);
QString get_function(QString dbName,QString func_name);
string create_view(QString dbName,QString view_info);
string drop_view(QString dbName,QString view_name);
QString get_view(QString dbName,QString view_name);
string create_procedure(QString dbName,QString pro_info);
string drop_procedure(QString dbName,QString pro_name);
QString get_procedure(QString dbName,QString pro_name);
string find_views(QString dbName,QStringList &view_list);
string find_pros(QString dbName,QStringList &func_list);
string find_funcs(QString dbName,QStringList &func_list);
string test(QString);
QString AlterTable_RenameTable(QString tName,QString newTName);
QString AlterTable_AddColumns(QString dbName,QString tName,QStringList allColNames,QStringList allColTypes,QStringList allColOpts,QStringList allPkOpts,QStringList allIndexOpts);
QString AlterTable_DropColumn(QString dbName,QString tName,QString drop_colName);
bool AlterTable_SelectAllFKFromOneTable(QString iDBName,QString iTableName, int &_fkSum, QStringList &_allFKName, QStringList &_fkColumnName, QStringList &_refTableName, QStringList &_refColumnName);

//这是新增函数声明
QString Query_User_Host(QStringList &pIOUserHost, int *pIONumberOfColumns, int *pIONumberOfRows);
QString Query_Authentication(QStringList &pIOPassword, QString pIUserName);
QString Create_User(QString pIUserName, QString pIHost, QString pIPassword);
QString Delete_User(QString pIUserName);
QString Query_Privileges_Variables(QString pIUserName, QString pIHost);
QString Query_Variables(QStringList &pIOUserPriv);
QString Revoke_Privileges(QString pIUserName, QString pIHost);
QString Authority_Management(int pIJudgement, QString pIUserName, QString pIHost);
QString Query_UserResource(QStringList &pIOUserResource, QString pIUserName, QString pIHost);
QString Set_UserResource(QString pIUserName, QString pIHost, QString pIQuestions, QString pIUpdates, QString pIConnections, QString pIUserConnections);
QString Query_Schema(QStringList &pIOSchema, QString pIUserName, QString pIHost, int *pIONumberOfRows);
QString Set_Schema_Privileges(int pIJudgement, QString pISchema, QString pIUserName, QString pIHost);
QString Query_Shema_Variables(QString pIUserName, QString pIHost, QString pISchema);
QString Show_Schema_Privileges(QStringList &pIOSchemaPriv);
QString Revoke_Schema_Privileges(QString pIUserName, QString pIHost);
QString Delete_Entry(QString pIUserName, QString pIHost, QString pISchema);
QString GetMysqlPath();
void UseOneOfDB(QString pIDatabaseName);
bool ImportDatabaseByOne(QString pIOneSQL, QString Tip);
#endif /* mysqlQuery_h */