//
//  mysqlQuery.cpp
//  WorkBench
//
//  Created by Vaaaas on 16/8/3.
//
//

#include "mysqlQuery.h"

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
        while((column=mysql_fetch_row(res)))
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
        
        //qDebug("main %s %d",mainKey.toStdString(),mainPos);
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
                     QStringList &col_type,QStringList &col_key,QStringList &col_extra)
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

QString AlterTable_DropColumn(QString tName,QString drop_colName)
{
    QString myquery;
    QString flag;
    myquery = "ALTER TABLE "+tName+" DROP "+drop_colName+";";
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
