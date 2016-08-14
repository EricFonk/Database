//
//  mysqlQuery.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/3.
//
//

#include "mysqlQuery.h"
#include <string>

//#pragma comment(lib,"wsock32.lib")
//#pragma comment(lib,"libmysql.lib")//mysql源码库

MYSQL mysql;//mysql对象 用于进行mysql接口的调用
MYSQL_FIELD *fd;//mysql返回域
char field[32][32];
MYSQL_RES *res;//mysql结果集
MYSQL_ROW column;//mysql行对象
char query[150];

using namespace std;

//连接数据库
bool ConnectDatabase()
{
    mysql_init(&mysql);
    if(!(mysql_real_connect(&mysql,"localhost","root","19960612","workbenchdb",3306,NULL,0)))
    {
        qDebug("error");
        return false;
    }
    else
    {
        printf("success");
        return true;
    }
}

char* getOneVariable(std::string variable)
{
    MYSQL_ROW sql_row;
    std::string query = "show variables like '" + variable + "'";
    //std::string strResult;
    
    const char* queryNew = query.c_str();
    int resInt = mysql_query(&mysql, queryNew);
    res = mysql_store_result(&mysql);
    sql_row = mysql_fetch_row(res);
    //sql_row[0];
    char* strResult = sql_row[1];
    
    mysql_free_result(res);
    
    return strResult;
}

QStandardItemModel* getConnections(QStandardItemModel *model)
{
    std::string query = "show PROCESSLIST";
    std::string strResult;
    
    MYSQL_ROW sql_row;
    MYSQL_FIELD* fld;
    
    const char* queryNew = query.c_str();
    int resInt = mysql_query(&mysql, queryNew);
    res = mysql_store_result(&mysql);
    int rowsNum = mysql_num_rows(res);
    int colsNum = mysql_num_fields(res);
    model = new QStandardItemModel(rowsNum, colsNum);
    
    for (int i = 0; (fld = mysql_fetch_field(res)); i++)
    {
        model->setHeaderData(i, Qt::Horizontal, fld->name);
    }
    strResult += '\n';
    sql_row = mysql_fetch_row(res);
    for (int i = 0; i < rowsNum; i++)
    {
        for (int j = 0; j < colsNum; j++)
        {
            if(sql_row[j]!=NULL)
            {
                QModelIndex index = model->index(i, j, QModelIndex());
                model->setData(index, QVariant(sql_row[j]));
            }
        }
        strResult += '\n';
        if (i != (rowsNum - 1))
        {
            sql_row = mysql_fetch_row(res);
        }
    }
    mysql_free_result(res);
    
    
    return model;
}

//根据表名搜索数据 返回行数、列数、列名、每条记录具体数值、主键名、主键位置
bool selectTest(QString table_name,QStringList &head,QStringList &data,int &row,int &col,QString mainKey,int &mainPos)
{
	QString myquery="select * from "+table_name+";";
	char *mytemp;
	int sumRow=0;
	int sumCol=0;
	MYSQL_RES *res;
	MYSQL_ROW column;

	mysql_query(&mysql,myquery.toStdString().c_str());
	res=mysql_store_result(&mysql);
	sumRow = mysql_affected_rows(&mysql);
	row = sumRow;
	sumCol = mysql_field_count(&mysql);
	col = sumCol;
	if(res->row_count!=0)
	{
		for(int i=0;i<sumCol;i++)
	{
		mytemp = mysql_fetch_field(res)->name;
		if(mainKey.toStdString().compare(mytemp)==0)
		{
			mainPos=i;			
		}
		head<<mytemp;
	}	
	qDebug("in %d",mainPos);
	while(column=mysql_fetch_row(res))
	{
		string temp;
		qDebug("%d %d",sumRow,sumCol);
		for(int j=0;j<col;j++)
		{
			if(column[j])
			{
				temp += column[j];
			}			
			if(j!=(col-1))
			{
				temp += "//";
			}			
		}
		
		data<<temp.c_str();
	}

	//qDebug("main %s %d",mainKey.toStdString().c_str(),mainPos);
	}
	
	
	return true;
}

//根据表名、数据、约束条件进行数据更新
string updateTest(QString table_name,QString data,QString con)
{
	QString myquery="update "+table_name+" set ";
	myquery = myquery + data + " where " + con + ";";
	//qDebug("%s",myquery);
	if(mysql_query(&mysql,myquery.toStdString().c_str()))
	{
		qDebug("error %s",mysql_error(&mysql));
		return mysql_error(&mysql);
	}
	else
	{
		return "success";
	}
}

//根据表名获取主键的名称
bool getMainKey(QString &mainKey,QString table_name)
{
    MYSQL_RES *res;
    MYSQL_ROW column;
    QString myquery;
    myquery = "select COLUMN_NAME from INFORMATION_SCHEMA.COLUMNS where table_name='"+table_name+"' AND COLUMN_KEY='PRI';";
    //获取主键值
    mysql_query(&mysql,myquery.toStdString().c_str());
    res=mysql_store_result(&mysql);
    
    if(res->row_count!=0)
    {
        column=mysql_fetch_row(res);
        string tmp = column[0];
        mainKey=QString::fromStdString(tmp);
    }
    else
    {
        
        mainKey="";
    }
    
    return true;
}

//根据sql语句进行直接操作
string executeWithQuery(int type,QString query)
{
    if(mysql_query(&mysql,query.toStdString().c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
        return mysql_error(&mysql);
    }
    else
    {
        /*
         //update
         if(type==0)
         {
         QStringList real_head,real_data;
         QString mainKey;
         int row,col,mainPos;
         bool mytest;
         mytest=getMainKey(mainKey,"new_one");
         mytest=selectTest("new_one",real_head,real_data,row,col,mainKey,mainPos);
         //RefreshTableData(real_data,row,col);
         }
         */
        return "success";
    }
}

//存储过程相关操作
string find_pros(QString dbName,QStringList &pro_list)
{
    QString myquery;
    
    myquery = "select `name` from mysql.proc where db = '"+dbName+"' and `type` = 'PROCEDURE';";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //必须使用局部变量！！！
        MYSQL_RES *res;
        MYSQL_ROW column;
        res=mysql_store_result(&mysql);
        while(column=mysql_fetch_row(res))
        {
            pro_list<<column[0];
        }
        return "success";
    }
}

//根据数据库名、存储过程内容新建存储过程
string create_procedure(QString dbName,QString pro_info)
{
    QString myquery;
    
    myquery="use "+dbName+";";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        myquery=pro_info+";";
        if(mysql_query(&mysql,myquery.toStdString().c_str()))
        {
            return mysql_error(&mysql);
        }
        else
        {
            return "success";
        }
    }
}

//根据数据库名、存储过程名删除存储过程
string drop_procedure(QString dbName,QString pro_name)
{
    QString myquery;
    myquery = "drop procedure `"+dbName+"`.`"+pro_name+"`;";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

//根据数据库名、存储过程名获取存储过程内容
QString get_procedure(QString dbName,QString pro_name)
{
    QString myquery;
    myquery = "show create procedure "+dbName+"."+pro_name;
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //必须使用局部变量！！！
        MYSQL_RES *res;
        MYSQL_ROW column;
        res=mysql_store_result(&mysql);
        column=mysql_fetch_row(res);
        return column[2];
    }
}

//函数相关操作
string find_funcs(QString dbName,QStringList &func_list)
{
    QString myquery;
    
    myquery = "select `name` from mysql.proc where db = '"+dbName+"' and `type` = 'FUNCTION';";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //必须使用局部变量！！！
        MYSQL_RES *res;
        MYSQL_ROW column;
        res=mysql_store_result(&mysql);
        while(column=mysql_fetch_row(res))
        {
            func_list<<column[0];
        }
        return "success";
    }
}

//根据数据库名、函数内容新建函数
string create_function(QString dbName,QString func_info)
{
    QString myquery;
    
    myquery="use "+dbName+";";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        myquery=func_info+";";
        if(mysql_query(&mysql,myquery.toStdString().c_str()))
        {
            return mysql_error(&mysql);
        }
        else
        {
            return "success";
        }
    }
}

string drop_function(QString dbName,QString func_name)
{
    QString myquery;
    myquery = "drop function `"+dbName+"`.`"+func_name+"`;";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

QString get_function(QString dbName,QString func_name)
{
    QString myquery;
    myquery = "show create function "+dbName+"."+func_name;
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //必须使用局部变量！！！
        MYSQL_RES *res;
        MYSQL_ROW column;
        res=mysql_store_result(&mysql);
        column=mysql_fetch_row(res);
        return column[2];
    }
}

//视图相关操作
//根据database名字查找视图
string find_views(QString dbName,QStringList &view_list)
{
    QString myquery;
    myquery = "SELECT TABLE_NAME from information_schema.VIEWS where TABLE_SCHEMA='"+dbName+"';";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //必须使用局部变量！！！
        MYSQL_RES *res;
        MYSQL_ROW column;
        res=mysql_store_result(&mysql);
        while(column=mysql_fetch_row(res))
        {
            view_list<<column[0];
        }
        return "success";
    }
}
string create_view(QString dbName,QString view_info)
{
    QString myquery;
    
    myquery="use "+dbName+";";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        myquery=view_info+";";
        if(mysql_query(&mysql,myquery.toStdString().c_str()))
        {
            return mysql_error(&mysql);
        }
        else
        {
            return "success";
        }
    }
}

string drop_view(QString dbName,QString view_name)
{
    QString myquery;
    myquery = "drop view `"+dbName+"`.`"+view_name+"`;";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

QString get_view(QString dbName,QString view_name)
{
    QString myquery;
    myquery = "show create view "+dbName+"."+view_name;
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //必须使用局部变量！！！
        MYSQL_RES *res;
        MYSQL_ROW column;
        res=mysql_store_result(&mysql);
        column=mysql_fetch_row(res);
        QString xx = column[1];
        //调整输出视图格式
        /*
         string xxx = xx.toStdString();
         int a1 = xxx.find_first_of("ALGORITHM");
         xxx.insert(a1,"\n");
         int a2 = xxx.find_first_of("DEFINER");
         xxx.insert(a2,"\n");
         int a3 = xxx.find_first_of("SQL");
         xxx.insert(a3,"\n");
         int a4 = xxx.find_first_of("VIEW");
         xxx.insert(a4,"\n");
         int a5 = xxx.find_first_of("AS");
         xxx.insert(a5,"\n");
         QString resxxx = QString::fromStdString(xxx);
         */
        return column[1];
    }
}

//DML
//database handler
string test(QString newDB)
{
    if(mysql_query(&mysql,"select * from test_one;"))
    {
        return "success";
    }
}

//获取全部schema名称
bool getAllDatabases(QStringList &list)
{
    //必须使用局部变量！！！
    MYSQL_RES *res;
    MYSQL_ROW column;
    string myquery = "Select SCHEMA_NAME From information_schema.SCHEMATA;";
    if(mysql_query(&mysql,myquery.c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
        return false;
    }
    else
    {
        res=mysql_store_result(&mysql);
        while(column=mysql_fetch_row(res))
        {
            list<<column[0];
        }
        return true;
    }
}

//根据数据库名新建数据库
string createDBTest(QString newDB)
{
    string myquery = "create database "+newDB.toStdString()+";";
    if(mysql_query(&mysql,myquery.c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

//根据数据库名删除数据库
string dropDBTest(QString oldDB)
{
    string myquery = "drop database "+oldDB.toStdString()+";";
    if(mysql_query(&mysql,myquery.c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

//根据数据库名查找所有属于该schema的表名
string getAllTables(QStringList &list,QString currentDB)
{
    //必须使用局部变量！！！
    MYSQL_RES *res;
    MYSQL_ROW column;
    string myquery = "select table_name from information_schema.tables where table_schema='"+currentDB.toStdString()+"';";
    if(mysql_query(&mysql,myquery.c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
        return mysql_error(&mysql);
    }
    else
    {
        res=mysql_store_result(&mysql);
        while(column=mysql_fetch_row(res))
        {
            list<<column[0];
        }
        return "success";
    }
}

//根据数据库名、表名、列语句、主键语句、唯一约束语句新建表
string createNewTable(QString dbName,QString tableName,QStringList createOpt,QString pkOption,QStringList uniqOpts)
{
    QString myquery;
    myquery = "create table ";
    if(dbName!=NULL)
    {
        myquery = myquery+"`"+dbName+"`.";
    }
    if(tableName!=NULL)
    {
        myquery = myquery+"`"+tableName+"`"+"(";
    }
    for(int i=0;i<createOpt.length();i++)
    {
        myquery=myquery+createOpt[i];
        if(i!=(createOpt.length()-1))
        {
            myquery = myquery + ",";
        }
    }
    //设置主键约束
    if(pkOption!=NULL)
    {
        myquery = myquery +","+pkOption;
    }
    //设置唯一约束
    if(uniqOpts.length()!=0)
    {
        myquery = myquery +",";
        for(int i=0;i<uniqOpts.length();i++)
        {
            myquery=myquery+uniqOpts[i];
            if(i!=(uniqOpts.length()-1))
            {
                myquery = myquery + ",";
            }
        }
    }
    
    myquery = myquery + ");";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

string dropTableWithQuery(QString query)
{
    if(mysql_query(&mysql,query.toStdString().c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

//根据表名获取其所有列信息
void getTableAllCols(QString tableName,QStringList &col_name,QStringList &col_default,QStringList &col_isNull,
					QStringList &col_type,QStringList &col_key,QStringList &col_extra,QStringList &col_uni)
{
    QString myquery;
    myquery = "SELECT * FROM information_schema.COLUMNS where table_name='"+tableName+"';";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
    }
    else
    {
        //必须使用局部变量！！！
        MYSQL_RES *res;
        res=mysql_store_result(&mysql);
        while(column=mysql_fetch_row(res))
        {
            col_name<<column[3];
            col_default<<column[5];
            col_isNull<<column[6];
            col_type<<column[7];
            col_key<<column[15];
            col_extra<<column[16];
        }
    }
}

void getOneTableUniqueChecks(QString dbName, QString tName,QStringList &unique)
{
	QString myquery;
	myquery = "SELECT ";
}

QString AlterTable_AddColumns(QString dbName,QString tName,QStringList allColNames,QStringList allColTypes,QStringList allColOpts,QStringList allPkOpts,QStringList allIndexOpts)
{
    QString myquery;
    QString flag;
    QString pkQuery;
    QString indexQuery;
    myquery = "ALTER TABLE "+dbName+"."+tName;
    
    for(int i=0;i<allColNames.length();i++)
    {
        myquery += " ADD "+allColNames[i]+" "+allColTypes[i]+" "+allColOpts[i];
        if(i!=allColNames.length()-1)
        {
            myquery+=",";
        }
    }
    if(allPkOpts.length()!=0)
    {
        pkQuery = "ADD CONSTRAINT pk_"+tName+" PRIMARY KEY(";
        for(int i=0;i<allPkOpts.length();i++)
        {
            pkQuery+="`"+allPkOpts[i]+"`";
            if(i!=allPkOpts.length()-1)
            {
                pkQuery+=",";
            }
            else
            {
                pkQuery+=")";
            }
        }
        myquery += ","+pkQuery;
    }
    if(allIndexOpts.length()!=0)
    {
        for(int i=0;i<allIndexOpts.length();i++)
        {
            indexQuery += "ADD INDEX index_"+tName+"_"+allIndexOpts[i]+" (`"+allIndexOpts[i]+"`)";
            if(i!=allIndexOpts.length()-1)
            {
                indexQuery+=",";
            }
        }
        myquery += ","+indexQuery;
    }
    myquery +=";";
    flag = QString::fromStdString(executeWithQuery(0,myquery));
    return flag;
}

QString AlterTable_DropColumn(QString dbName, QString tName,QString drop_colName)
{
	QString myquery;
	QString flag;
	myquery = "ALTER TABLE "+dbName+"."+tName+" DROP "+drop_colName+";";
	if(mysql_query(&mysql,myquery.toStdString().c_str()))
	{
		qDebug("error %s",mysql_error(&mysql));
		flag = mysql_error(&mysql);
	}
	else
	{
		flag = "OK";
	}
	return flag;
}

//更改表名 检查sql语法是否正确
QString AlterTable_RenameTable(QString tName,QString newTName)
{
    QString myquery;
    QString flag;
    myquery = "ALTER TABLE "+tName+" RENAME "+newTName+";";
    if(mysql_query(&mysql,myquery.toStdString().c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
        flag = mysql_error(&mysql);
    }
    else
    {
        flag = "OK";
    }
    return flag;
}

//这是我写的
//首先是连接数据库，已经写了


//查询用户表中的user和host,返回结果集
QString Query_User_Host(QStringList &pIOUserHost, int *pIONumberOfColumns, int *pIONumberOfRows)
{
	QString querySql;
	QString queryStatus;
	MYSQL_RES *queryResult;
	MYSQL_ROW fetchRow;
	querySql = "select user,host from mysql.user";
	if (mysql_query(&mysql, querySql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		queryStatus = mysql_error(&mysql);
	}
	else
	{
		queryResult = mysql_store_result(&mysql);
		if (queryResult)
		{
			*pIONumberOfColumns = mysql_num_fields(queryResult); //获取列的数目
			*pIONumberOfRows = mysql_affected_rows(&mysql); //获取行的数目

			for (int m = 0; m < *pIONumberOfRows; m++) //几行几列,
			{
				fetchRow = mysql_fetch_row(queryResult);
				if (fetchRow != NULL)
				{
					for (int n = 0; n < *pIONumberOfColumns; n++)
					{
						pIOUserHost<<fetchRow[n];
					}
				}
			}
			queryStatus = "OK";
		}
	}
	return queryStatus;
}
//查询当前用户的密码
QString Query_Authentication(QStringList &pIOPassword,QString pIUserName)
{
	QString querySql;
	QString queryStatus;
	MYSQL_RES *queryResult;
	MYSQL_ROW fetchRow;

	querySql = "select authentication_string from mysql.user where user='" + pIUserName + "';";
	if (mysql_query(&mysql, querySql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		queryStatus = mysql_error(&mysql);
	}
	else
	{
		queryResult = mysql_store_result(&mysql);
		fetchRow = mysql_fetch_row(queryResult);
		pIOPassword << fetchRow[0];
		queryStatus = "OK";
	}
	return queryStatus;
}
//创建新用户
QString Create_User(QString pIUserName, QString pIHost, QString pIPassword)
{
	QString createSql;
	QString createStatus;
	createSql = "create user' " + pIUserName + " '@'" + pIHost + "'identified by '" + pIPassword + "';";
	if (mysql_query(&mysql, createSql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		createStatus = mysql_error(&mysql);
	}
	else
	{
		createStatus = "OK";
	}
	return createStatus;
}

//删除用户
QString Delete_User(QString pIUserName)
{
	QString deleteSql;
	QString deleteStatus;
	deleteSql = "delete from mysql.user where user='" + pIUserName + "';";
	if (mysql_query(&mysql, deleteSql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		deleteStatus = mysql_error(&mysql);
	}
	else
	{
		deleteStatus = "OK";
	}
	return deleteStatus;
}

//查询权限，将权限值赋给对应的变量
QString Query_Privileges_Variables(QString pIUserName, QString pIHost)
{
	QString querySql;
	QString queryStatus;
	querySql = "select Select_priv,Insert_priv,Update_priv,Delete_priv,Create_priv,Drop_priv,Reload_priv,Shutdown_priv,Process_priv,File_priv,Grant_priv,References_priv,Index_priv,Alter_priv,Show_db_priv,Super_priv,Create_tmp_table_priv,Lock_tables_priv,Execute_priv,Repl_slave_priv,Repl_client_priv,Create_view_priv,Show_view_priv,Create_routine_priv,Alter_routine_priv,Create_user_priv,Event_priv,Trigger_priv,Create_tablespace_priv into @select_priv,@insert_priv,@update_priv,@delete_priv,@create_priv,@drop_priv,@reload_priv,@shutdown_priv,@process_priv,@file_priv,@grant_priv,@references_priv,@index_priv,@alter_priv,@show_db_priv,@super_priv,@create_tmp_table_priv,@lock_tables_priv,@execute_priv,@repl_slave_priv,@repl_client_priv,@create_view_priv,@show_view_priv,@create_routine_priv,@alter_routine_priv,@create_user_priv,@event_priv,@trigger_priv,@create_tablespace_priv from mysql.user where user='" + pIUserName + "' and host='" + pIHost + "'";
	if (mysql_query(&mysql, querySql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		queryStatus = mysql_error(&mysql);
	}
	else
	{
		queryStatus = "OK";
	}
	return queryStatus;
}
//查询权限变量
QString Query_Variables(QStringList &pIOUserPriv)
{
	QString querySql;
	QString queryStatus;
	MYSQL_RES *queryResult;
	MYSQL_ROW fetchRow;
	querySql = "select @select_priv,@insert_priv,@update_priv,@delete_priv,@create_priv,@drop_priv,@reload_priv,@shutdown_priv,@process_priv,@file_priv,@grant_priv,@references_priv,@index_priv,@alter_priv,@show_db_priv,@super_priv,@create_tmp_table_priv,@lock_tables_priv,@execute_priv,@repl_slave_priv,@repl_client_priv,@create_view_priv,@show_view_priv,@create_routine_priv,@alter_routine_priv,@create_user_priv,@event_priv,@trigger_priv,@create_tablespace_priv;";
	if (mysql_query(&mysql, querySql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		queryStatus = mysql_error(&mysql);
	}
	else
	{
		queryResult = mysql_store_result(&mysql);
		if (queryResult)
		{
			fetchRow = mysql_fetch_row(queryResult);
			for (int i = 0; i < 29; i++)
			{
				pIOUserPriv << fetchRow[i];
			}
			queryStatus = "OK";
		}
	}
	return queryStatus;
}
//撤销用户权限
QString Revoke_Privileges(QString pIUserName, QString pIHost)
{
	QString revokeSql;
	QString revokeStatus;
	revokeSql = "revoke alter,alter routine,create,create routine,create tablespace,create temporary tables,create user,create view,delete,drop,event,execute,file,grant option,index,insert,lock tables,process, references,reload,replication client,replication slave,select,show databases,show view,shutdown,super,trigger,update on *.* from '" + pIUserName + "'@'" + pIHost + "';";
	if (mysql_query(&mysql, revokeSql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		revokeStatus = mysql_error(&mysql);
	}
	else
	{
		revokeStatus = "OK";
	}
	return revokeStatus;
}
//用户授权
//二十九个权限授权，过于复杂，并且代码重复过多
QString Authority_Management(int pIJudgement,QString pIUserName,QString pIHost)
{
	int res;
	QString grantSql;
	QString grantStatus;
	enum PrivEnum
	{
		Select = 0,
		Insert,
		Update,
		Delete,
		Create,
		Drop,
		Reload,
		ShutDown,
		Process,
		File,
		Option,
		References,
		Index,
		Alter,
		ShowDatabases,
		Super,
		TemporarTables,
		LockTables,
		Execute,
		Slave,
		Client,
		CreateView,
		ShowView,
		CreateRoutine,
		AlterRoutine,
		CreateUser,
		Event,
		Trigger,
		CreateTableSpace
	};
	switch (pIJudgement)
	{
	case Select:grantSql = "grant select on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Insert:grantSql = "grant insert on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Update:grantSql = "grant update on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Delete:grantSql = "grant delete on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Create:grantSql = "grant create on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Drop:grantSql = "grant drop on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Reload:grantSql = "grant reload on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case ShutDown:grantSql = "grant shutdown on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Process:grantSql = "grant process on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case File:grantSql = "grant file on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Option:grantSql = "grant grant option on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case References:grantSql = "grant references on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Index:grantSql = "grant index on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Alter:grantSql = "grant alter on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case ShowDatabases:grantSql = "grant show databases on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Super:grantSql = "grant super on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case TemporarTables:grantSql = "grant create temporary tables on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case LockTables:grantSql = "grant lock tables on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Execute:grantSql = "grant execute on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Slave:grantSql = "grant replication slave on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Client:grantSql = "grant replication client on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case CreateView:grantSql = "grant create view on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case ShowView:grantSql = "grant show view on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case CreateRoutine:grantSql = "grant create routine on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case AlterRoutine:grantSql = "grant alter routine on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case CreateUser:grantSql = "grant create user on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Event:grantSql = "grant event on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case Trigger:grantSql = "grant trigger on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	case CreateTableSpace:grantSql = "grant create tablespace on *.* to '" + pIUserName + "'@'" + pIHost + "';"; break;
	default:
		break;
	}
	res = mysql_query(&mysql, grantSql.toStdString().c_str());
	if (!res)
	{
		grantStatus = "OK";
	}
	return grantStatus;

}

//查询当前用户资源
QString Query_UserResource(QStringList &pIOUserResource,QString pIUserName, QString pIHost)
{
	MYSQL_RES *queryResult;
	QString querySql;
	QString queryStatus;
	MYSQL_ROW fetchRow;
	querySql = "select max_questions,max_updates,max_connections,max_user_connections from mysql.user where user='" + pIUserName + "' and host='" + pIHost + "';";
	if (mysql_query(&mysql, querySql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		queryStatus = mysql_error(&mysql);
	}
	else
	{
		queryResult = mysql_store_result(&mysql);
		fetchRow = mysql_fetch_row(queryResult);
		for (int i = 0; i < 4; i++)
		{
			pIOUserResource << fetchRow[i];
		}
		queryStatus = "OK";
	}
	return queryStatus;
}

//设置用户资源
QString Set_UserResource(QString pIUserName, QString pIHost, QString pIQuestions, QString pIUpdates, QString pIConnections, QString pIUserConnections)
{
	QString setSql;
	QString setStatus;
	setSql = "GRANT USAGE ON *.* TO '" + pIUserName + "'@'" + pIHost + "' WITH MAX_QUERIES_PER_HOUR " + pIQuestions + " MAX_UPDATES_PER_HOUR " + pIUpdates + " MAX_CONNECTIONS_PER_HOUR " + pIConnections + " MAX_USER_CONNECTIONS " + pIUserConnections + ";";
	if (mysql_query(&mysql, setSql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		setStatus = mysql_error(&mysql);
	}
	else
	{
		setStatus = "OK";
	}
	return setStatus;
}

//查询当前用户赋予了权限的pISchema
QString Query_Schema(QStringList &pIOSchema, QString pIUserName, QString pIHost,int *pIONumberOfRows)
{
	QString querySql;
	QString queryStatus;
	MYSQL_RES *queryResult;
	MYSQL_ROW fetchRow;
	querySql = "select db from mysql.db where user='" + pIUserName + "' and host='" + pIHost + "';";
	if (mysql_query(&mysql, querySql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		queryStatus = mysql_error(&mysql);
	}
	else
	{
		queryResult = mysql_store_result(&mysql);
		if (queryResult)
		{
			*pIONumberOfRows = mysql_affected_rows(&mysql); //获取行的数目

			for (int j = 0; j < *pIONumberOfRows; j++) //行循环
			{
				fetchRow = mysql_fetch_row(queryResult);
				if (fetchRow != NULL)
				{
					pIOSchema << fetchRow[0];
				}
			}
			queryStatus = "OK";
		}
	}
	return queryStatus;
}

//设置当前用户数据库Schema权限
QString Set_Schema_Privileges(int pIJudgement, QString pISchema, QString pIUserName, QString pIHost)
{
	int res;
	QString grantSql;
	QString grantStatus;
	enum PrivEnum
	{
		Select = 0,
		Insert,
		Update,
		Delete,
		Execute,
		ShowView,
		Create,
		Alter,
		References,
		Index,
		CreateView,
		CreateRoutine,
		AlterRoutine,
		Event,
		Drop,
		Trigger,
		GrantOption,
		CreateTempTables,
		LockTables
	};
	switch (pIJudgement)
	{
	case Select:grantSql = "GRANT SELECT ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case Insert:grantSql = "GRANT INSERT ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case Update:grantSql = "GRANT UPDATE ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case Delete:grantSql = "GRANT DELETE ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case Execute:grantSql = "GRANT EXECUTE ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case ShowView:grantSql = "GRANT SHOW VIEW ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case Create:grantSql = "GRANT CREATE ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case Alter:grantSql = "GRANT ALTER ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case References:grantSql = "GRANT REFERENCES ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case Index:grantSql = "GRANT INDEX ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case CreateView:grantSql = "GRANT CREATE VIEW ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case CreateRoutine:grantSql = "GRANT CREATE ROUTINE ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case AlterRoutine:grantSql = "GRANT ALTER ROUTINE ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case Event:grantSql = "GRANT EVENT ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case Drop:grantSql = "GRANT DROP ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case Trigger:grantSql = "GRANT TRIGGER ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case GrantOption:grantSql = "GRANT GRANT OPTION ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case CreateTempTables:grantSql = "GRANT CREATE TEMPORARY TABLES ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	case LockTables:grantSql = "GRANT LOCK TABLES ON " + pISchema + ".* TO " + pIUserName + "@" + pIHost + ";"; break;
	default:
		break;
	}
	res = mysql_query(&mysql, grantSql.toStdString().c_str());
	if (!res)
	{
		grantStatus = "OK";
	}
	return grantStatus;
}
QString Query_Shema_Variables(QString pIUserName, QString pIHost, QString pISchema)
{
	QString querySql;
	QString queryStatus;
	querySql = "select Select_priv,Insert_priv,Update_priv,Delete_priv,Create_priv,Drop_priv,Grant_priv,References_priv,Index_priv,Alter_priv,Create_tmp_table_priv,Lock_tables_priv,Create_view_priv,Show_view_priv,Create_routine_priv,Alter_routine_priv,Execute_priv,Event_priv,Trigger_priv into @select_priv,@insert_priv,@update_priv,@delete_priv,@create_priv,@drop_priv,@grant_priv,@references_priv,@index_priv,@alter_priv,@create_tmp_table_priv,@lock_tables_priv,@create_view_priv,@show_view_priv,@create_routine_priv,@alter_routine_priv,@execute_priv,@event_priv,@trigger_priv from mysql.db where user='" + pIUserName + "' and host='" + pIHost + "'and db='" + pISchema + "';";
	if (mysql_query(&mysql, querySql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		queryStatus = mysql_error(&mysql);
	}
	else
	{
		queryStatus = "OK";
	}
	return queryStatus;
}
QString Show_Schema_Privileges(QStringList &pIOSchemaPriv)
{
	QString querySql;
	QString queryStatus;
	MYSQL_RES *queryResult;
	MYSQL_ROW fetchRow;
	querySql = "select @select_priv,@insert_priv,@update_priv,@delete_priv,@create_priv,@drop_priv,@grant_priv,@references_priv,@index_priv,@alter_priv,@create_tmp_table_priv,@lock_tables_priv,@create_view_priv,@show_view_priv,@create_routine_priv,@alter_routine_priv,@execute_priv,@event_priv,@trigger_priv;";
	if (mysql_query(&mysql, querySql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		queryStatus = mysql_error(&mysql);
	}
	else
	{
		queryResult = mysql_store_result(&mysql);
		if (queryResult)
		{
			fetchRow = mysql_fetch_row(queryResult);
			for (int i = 0; i < 19; i++)
			{
				pIOSchemaPriv << fetchRow[i];
			}
			queryStatus = "OK";
		}
	}
	return queryStatus;
}
QString Revoke_Schema_Privileges(QString pIUserName, QString pIHost)
{
	QString revokeStatus;
	QString revokeSql;
	revokeSql = "delete from mysql.db where user='" + pIUserName + "' and host='" + pIHost + "';";
	if (mysql_query(&mysql, revokeSql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		revokeStatus = mysql_error(&mysql);
	}
	else
	{
		revokeStatus = "OK";
	}
	return revokeStatus;
}
QString Delete_Entry(QString pIUserName, QString pIHost, QString pISchema)
{
	QString deleteStatus;
	QString deleteSql;
	deleteSql = "delete from mysql.db where user='" + pIUserName + "' and host='" + pIHost + "' and db='" + pISchema + "';";
	if (mysql_query(&mysql, deleteSql.toStdString().c_str()))
	{
		qDebug("error %s", mysql_error(&mysql));
		deleteStatus = mysql_error(&mysql);
	}
	else
	{
		deleteStatus = "OK";
	}
	return deleteStatus;
}
