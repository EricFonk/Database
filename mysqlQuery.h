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

bool ConnectDatabase();

QStandardItemModel* GetConnections(QStandardItemModel *model);
char* GetOneVariable(std::string variable);

bool GetMainKey(QString &mainKey,QString table_name);
bool SelectTest(QString,QStringList&,QStringList&,int&,int&,QString,int&);
string UpdateTest(QString,QString,QString);
string ExecuteWithQuery(int,QString);
bool GetAllDatabases(QStringList&);
string CreateDBTest(QString);
string DropDBTest(QString);
string GetAllTables(QStringList&,QString);
string CreateNewTable(QString,QString,QStringList,QString,QStringList);
string DropTableWithQuery(QString query);
void GetTableAllCols(QString tableName,QStringList &col_name,QStringList &col_default,QStringList &col_isNull,QStringList &col_type,QStringList &col_key,QStringList &col_extra,QStringList &col_uni);
string CreateFunction(QString dbName,QString func_info);
string DropFunction(QString dbName,QString func_name);
QString GetFunction(QString dbName,QString func_name);
string CreateView(QString dbName,QString view_info);
string DropView(QString dbName,QString view_name);
QString GetView(QString dbName,QString view_name);
string CreateProcedure(QString dbName,QString pro_info);
string DropProcedure(QString dbName,QString pro_name);
QString GetProcedure(QString dbName,QString pro_name);
string FindViews(QString dbName,QStringList &view_list);
string FindPros(QString dbName,QStringList &func_list);
string FindFuncs(QString dbName,QStringList &func_list);
string Test(QString);
QString AlterTableRenameTable(QString tName,QString newTName);
QString AlterTableAddColumns(QString dbName,QString tName,QStringList allColNames,QStringList allColTypes,QStringList allColOpts,QStringList allPkOpts,QStringList allIndexOpts);
QString AlterTableDropColumn(QString dbName,QString tName,QString drop_colName);

//这是新增函数声明
QString QueryUserHost(QStringList &pIOUserHost, int *pIONumberOfColumns, int *pIONumberOfRows);
QString QueryAuthentication(QStringList &pIOPassword, QString pIUserName);
QString CreateUser(QString pIUserName, QString pIHost, QString pIPassword);
QString DeleteUser(QString pIUserName);
QString QueryPrivilegesVariables(QString pIUserName, QString pIHost);
QString QueryVariables(QStringList &pIOUserPriv);
QString RevokePrivileges(QString pIUserName, QString pIHost);
QString AuthorityManagement(int pIJudgement, QString pIUserName, QString pIHost);
QString QueryUserResource(QStringList &pIOUserResource, QString pIUserName, QString pIHost);
QString SetUserResource(QString pIUserName, QString pIHost, QString pIQuestions, QString pIUpdates, QString pIConnections, QString pIUserConnections);
QString QuerySchema(QStringList &pIOSchema, QString pIUserName, QString pIHost, int *pIONumberOfRows);
QString SetSchemaPrivileges(int pIJudgement, QString pISchema, QString pIUserName, QString pIHost);
QString QueryShemaVariables(QString pIUserName, QString pIHost, QString pISchema);
QString ShowSchemaPrivileges(QStringList &pIOSchemaPriv);
QString RevokeSchemaPrivileges(QString pIUserName, QString pIHost);
QString DeleteEntry(QString pIUserName, QString pIHost, QString pISchema);
#endif /* mysqlQuery_h */