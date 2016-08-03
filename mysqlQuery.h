//
//  mysqlQuery.hpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/3.
//
//

#ifndef mysqlQuery_hpp
#define mysqlQuery_hpp

#include "mysqlQuery.h"
//#include <WinSock.h>
#include <QString>
#include "mysql.h"
#include "tablewidget.h"
#include <string>

using namespace std;

bool ConnectDatabase();
bool getMainKey(QString &mainKey,QString table_name);
bool selectTest(QString,QStringList&,QStringList&,int&,int&,QString,int&);
string updateTest(QString,QString,QString);
string executeWithQuery(int,QString);
bool getAllDatabases(QStringList&);
string createDBTest(QString);
string dropDBTest(QString);
string getAllTables(QStringList&,QString);
string createNewTable(QString,QString,QStringList,QString,QStringList);
string dropTableWithQuery(QString query);
void getTableAllCols(QString tableName,QStringList &col_name,QStringList &col_default,QStringList &col_isNull,
                     QStringList &col_type,QStringList &col_key,QStringList &col_extra);
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
QString AlterTable_DropColumn(QString tName,QString drop_colName);

#endif /* mysqlQuery_hpp */
