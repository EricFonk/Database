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
//#pragma comment(lib,"libmysql.lib")//mysqlԴ���

MYSQL mysql;//mysql���� ���ڽ���mysql�ӿڵĵ���
MYSQL_FIELD *fd;//mysql������
char field[32][32];
MYSQL_RES *res;//mysql�����
MYSQL_ROW column;//mysql�ж���
char query[150];

using namespace std;

//�������ݿ�
bool ConnectDatabase()
{
    mysql_init(&mysql);
    if(!(mysql_real_connect(&mysql,"localhost","root","123456","",3306,NULL,0)))
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

char* GetOneVariable(std::string variable)
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

QStandardItemModel* GetConnections(QStandardItemModel *model)
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

//���ݱ����������� ����������������������ÿ����¼������ֵ��������������λ��
bool SelectTest(QString table_name,QStringList &head,QStringList &data,int &row,int &col,QString mainKey,int &mainPos)
{
	QString myQuery="select * from "+table_name+";";
	char *mytemp;
	int sumRow=0;
	int sumCol=0;
	MYSQL_RES *res;
	MYSQL_ROW column;

	mysql_query(&mysql,myQuery.toStdString().c_str());
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

//���ݱ��������ݡ�Լ�������������ݸ���
string UpdateTest(QString table_name,QString data,QString con)
{
	QString myQuery="update "+table_name+" set ";
	myQuery = myQuery + data + " where " + con + ";";
	//qDebug("%s",myQuery);
	if(mysql_query(&mysql,myQuery.toStdString().c_str()))
	{
		qDebug("error %s",mysql_error(&mysql));
		return mysql_error(&mysql);
	}
	else
	{
		return "success";
	}
}

//���ݱ�����ȡ����������
bool GetMainKey(QString &mainKey,QString table_name)
{
    MYSQL_RES *res;
    MYSQL_ROW column;
    QString myQuery;
    myQuery = "select COLUMN_NAME from INFORMATION_SCHEMA.COLUMNS where table_name='"+table_name+"' AND COLUMN_KEY='PRI';";
    //��ȡ����ֵ
    mysql_query(&mysql,myQuery.toStdString().c_str());
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

//����sql������ֱ�Ӳ���
string ExecuteWithQuery(int type,QString query)
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
         mytest=GetMainKey(mainKey,"new_one");
         mytest=SelectTest("new_one",real_head,real_data,row,col,mainKey,mainPos);
         //RefreshTableData(real_data,row,col);
         }
         */
        return "success";
    }
}

//�洢������ز���
string FindPros(QString dbName,QStringList &pro_list)
{
    QString myQuery;
    
    myQuery = "select `name` from mysql.proc where db = '"+dbName+"' and `type` = 'PROCEDURE';";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //����ʹ�þֲ�����������
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

//�������ݿ������洢���������½��洢����
string CreateProcedure(QString dbName,QString pro_info)
{
    QString myQuery;
    
    myQuery="use "+dbName+";";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        myQuery=pro_info+";";
        if(mysql_query(&mysql,myQuery.toStdString().c_str()))
        {
            return mysql_error(&mysql);
        }
        else
        {
            return "success";
        }
    }
}

//�������ݿ������洢������ɾ���洢����
string DropProcedure(QString dbName,QString pro_name)
{
    QString myQuery;
    myQuery = "drop procedure `"+dbName+"`.`"+pro_name+"`;";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

//�������ݿ������洢��������ȡ�洢��������
QString GetProcedure(QString dbName,QString pro_name)
{
    QString myQuery;
    myQuery = "show create procedure "+dbName+"."+pro_name;
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //����ʹ�þֲ�����������
        MYSQL_RES *res;
        MYSQL_ROW column;
        res=mysql_store_result(&mysql);
        column=mysql_fetch_row(res);
        return column[2];
    }
}

//������ز���
string FindFuncs(QString dbName,QStringList &func_list)
{
    QString myQuery;
    
    myQuery = "select `name` from mysql.proc where db = '"+dbName+"' and `type` = 'FUNCTION';";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //����ʹ�þֲ�����������
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

//�������ݿ��������������½�����
string CreateFunction(QString dbName,QString func_info)
{
    QString myQuery;
    
    myQuery="use "+dbName+";";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        myQuery=func_info+";";
        if(mysql_query(&mysql,myQuery.toStdString().c_str()))
        {
            return mysql_error(&mysql);
        }
        else
        {
            return "success";
        }
    }
}

string DropFunction(QString dbName,QString func_name)
{
    QString myQuery;
    myQuery = "drop function `"+dbName+"`.`"+func_name+"`;";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

QString GetFunction(QString dbName,QString func_name)
{
    QString myQuery;
    myQuery = "show create function "+dbName+"."+func_name;
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //����ʹ�þֲ�����������
        MYSQL_RES *res;
        MYSQL_ROW column;
        res=mysql_store_result(&mysql);
        column=mysql_fetch_row(res);
        return column[2];
    }
}

//��ͼ��ز���
//����database���ֲ�����ͼ
string FindViews(QString dbName,QStringList &view_list)
{
    QString myQuery;
    myQuery = "SELECT TABLE_NAME from information_schema.VIEWS where TABLE_SCHEMA='"+dbName+"';";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //����ʹ�þֲ�����������
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
string CreateView(QString dbName,QString view_info)
{
    QString myQuery;
    
    myQuery="use "+dbName+";";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        myQuery=view_info+";";
        if(mysql_query(&mysql,myQuery.toStdString().c_str()))
        {
            return mysql_error(&mysql);
        }
        else
        {
            return "success";
        }
    }
}

string DropView(QString dbName,QString view_name)
{
    QString myQuery;
    myQuery = "drop view `"+dbName+"`.`"+view_name+"`;";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

QString GetView(QString dbName,QString view_name)
{
    QString myQuery;
    myQuery = "show create view "+dbName+"."+view_name;
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        return mysql_error(&mysql);
    }
    else
    {
        //����ʹ�þֲ�����������
        MYSQL_RES *res;
        MYSQL_ROW column;
        res=mysql_store_result(&mysql);
        column=mysql_fetch_row(res);
        QString xx = column[1];
        //���������ͼ��ʽ
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
string Test(QString newDB)
{
    if(mysql_query(&mysql,"select * from test_one;"))
    {
        return "success";
    }
}

//��ȡȫ��schema����
bool GetAllDatabases(QStringList &list)
{
    //����ʹ�þֲ�����������
    MYSQL_RES *res;
    MYSQL_ROW column;
    string myQuery = "Select SCHEMA_NAME From information_schema.SCHEMATA;";
    if(mysql_query(&mysql,myQuery.c_str()))
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

//�������ݿ����½����ݿ�
string CreateDBTest(QString newDB)
{
    string myQuery = "create database "+newDB.toStdString()+";";
    if(mysql_query(&mysql,myQuery.c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

//�������ݿ���ɾ�����ݿ�
string DropDBTest(QString oldDB)
{
    string myQuery = "drop database "+oldDB.toStdString()+";";
    if(mysql_query(&mysql,myQuery.c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

//�������ݿ��������������ڸ�schema�ı���
string GetAllTables(QStringList &list,QString currentDB)
{
    //����ʹ�þֲ�����������
    MYSQL_RES *res;
    MYSQL_ROW column;
    string myQuery = "select table_name from information_schema.tables where table_schema='"+currentDB.toStdString()+"';";
    if(mysql_query(&mysql,myQuery.c_str()))
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

//�������ݿ���������������䡢������䡢ΨһԼ������½���
string CreateNewTable(QString dbName,QString tableName,QStringList createOpt,QString pkOption,QStringList uniqOpts)
{
    QString myQuery;
    myQuery = "create table ";
    if(dbName!=NULL)
    {
        myQuery = myQuery+"`"+dbName+"`.";
    }
    if(tableName!=NULL)
    {
        myQuery = myQuery+"`"+tableName+"`"+"(";
    }
    for(int i=0;i<createOpt.length();i++)
    {
        myQuery=myQuery+createOpt[i];
        if(i!=(createOpt.length()-1))
        {
            myQuery = myQuery + ",";
        }
    }
    //��������Լ��
    if(pkOption!=NULL)
    {
        myQuery = myQuery +","+pkOption;
    }
    //����ΨһԼ��
    if(uniqOpts.length()!=0)
    {
        myQuery = myQuery +",";
        for(int i=0;i<uniqOpts.length();i++)
        {
            myQuery=myQuery+uniqOpts[i];
            if(i!=(uniqOpts.length()-1))
            {
                myQuery = myQuery + ",";
            }
        }
    }
    
    myQuery = myQuery + ");";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
        return mysql_error(&mysql);
    }
    else
    {
        return "success";
    }
}

string DropTableWithQuery(QString query)
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

//���ݱ�����ȡ����������Ϣ
void GetTableAllCols(QString tableName,QStringList &col_name,QStringList &col_default,QStringList &col_isNull,
					QStringList &col_type,QStringList &col_key,QStringList &col_extra,QStringList &col_uni)
{
    QString myQuery;
    myQuery = "SELECT * FROM information_schema.COLUMNS where table_name='"+tableName+"';";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
    {
        qDebug("error %s",mysql_error(&mysql));
    }
    else
    {
        //����ʹ�þֲ�����������
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
	QString myQuery;
	myQuery = "SELECT ";
}

QString AlterTableAddColumns(QString dbName,QString tName,QStringList allColNames,QStringList allColTypes,QStringList allColOpts,QStringList allPkOpts,QStringList allIndexOpts)
{
    QString myQuery;
    QString flag;
    QString pkQuery;
    QString indexQuery;
    myQuery = "ALTER TABLE "+dbName+"."+tName;
    
    for(int i=0;i<allColNames.length();i++)
    {
        myQuery += " ADD "+allColNames[i]+" "+allColTypes[i]+" "+allColOpts[i];
        if(i!=allColNames.length()-1)
        {
            myQuery+=",";
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
        myQuery += ","+pkQuery;
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
        myQuery += ","+indexQuery;
    }
    myQuery +=";";
    flag = QString::fromStdString(ExecuteWithQuery(0,myQuery));
    return flag;
}

QString AlterTableDropColumn(QString dbName, QString tName,QString drop_colName)
{
	QString myQuery;
	QString flag;
	myQuery = "ALTER TABLE "+dbName+"."+tName+" DROP "+drop_colName+";";
	if(mysql_query(&mysql,myQuery.toStdString().c_str()))
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

//���ı��� ���sql�﷨�Ƿ���ȷ
QString AlterTableRenameTable(QString tName,QString newTName)
{
    QString myQuery;
    QString flag;
    myQuery = "ALTER TABLE "+tName+" RENAME "+newTName+";";
    if(mysql_query(&mysql,myQuery.toStdString().c_str()))
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

//������д��
//�������������ݿ⣬�Ѿ�д��


//��ѯ�û����е�user��host,���ؽ����
QString QueryUserHost(QStringList &pIOUserHost, int *pIONumberOfColumns, int *pIONumberOfRows)
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
			*pIONumberOfColumns = mysql_num_fields(queryResult); //��ȡ�е���Ŀ
			*pIONumberOfRows = mysql_affected_rows(&mysql); //��ȡ�е���Ŀ

			for (int m = 0; m < *pIONumberOfRows; m++) //���м���,
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
//��ѯ��ǰ�û�������
QString QueryAuthentication(QStringList &pIOPassword,QString pIUserName)
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
//�������û�
QString CreateUser(QString pIUserName, QString pIHost, QString pIPassword)
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

//ɾ���û�
QString DeleteUser(QString pIUserName)
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

//��ѯȨ�ޣ���Ȩ��ֵ������Ӧ�ı���
QString QueryPrivilegesVariables(QString pIUserName, QString pIHost)
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
//��ѯȨ�ޱ���
QString QueryVariables(QStringList &pIOUserPriv)
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
//�����û�Ȩ��
QString RevokePrivileges(QString pIUserName, QString pIHost)
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
//�û���Ȩ
//��ʮ�Ÿ�Ȩ����Ȩ�����ڸ��ӣ����Ҵ����ظ�����
QString AuthorityManagement(int pIJudgement,QString pIUserName,QString pIHost)
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

//��ѯ��ǰ�û���Դ
QString QueryUserResource(QStringList &pIOUserResource,QString pIUserName, QString pIHost)
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

//�����û���Դ
QString SetUserResource(QString pIUserName, QString pIHost, QString pIQuestions, QString pIUpdates, QString pIConnections, QString pIUserConnections)
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

//��ѯ��ǰ�û�������Ȩ�޵�pISchema
QString QuerySchema(QStringList &pIOSchema, QString pIUserName, QString pIHost,int *pIONumberOfRows)
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
			*pIONumberOfRows = mysql_affected_rows(&mysql); //��ȡ�е���Ŀ

			for (int j = 0; j < *pIONumberOfRows; j++) //��ѭ��
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

//���õ�ǰ�û����ݿ�SchemaȨ��
QString SetSchemaPrivileges(int pIJudgement, QString pISchema, QString pIUserName, QString pIHost)
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
QString QueryShemaVariables(QString pIUserName, QString pIHost, QString pISchema)
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
QString ShowSchemaPrivileges(QStringList &pIOSchemaPriv)
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
QString RevokeSchemaPrivileges(QString pIUserName, QString pIHost)
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
QString DeleteEntry(QString pIUserName, QString pIHost, QString pISchema)
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
