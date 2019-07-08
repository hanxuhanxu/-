#include "stdafx.h"
#include "MySql.h"


MYSQL* sock = new MYSQL;
CMySql::CMySql(void)
{
    /*这个函数用来分配或者初始化一个MYSQL对象，用于连接mysql服务端。
    如果你传入的参数是NULL指针，它将自动为你分配一个MYSQL对象，
    如果这个MYSQL对象是它自动分配的，那么在调用mysql_close的时候，会释放这个对象*/
    
   mysql_init(sock );  
    mysql_set_character_set(sock,"gb2312"); //gb2312 中华人民共和国简体字标准
}


CMySql::~CMySql(void)
{
    if(sock)
    {
        delete sock;
        sock = NULL;
    }    
}

void CMySql::DisConnect()
{
    mysql_close(sock);
}

bool CMySql::ConnectMySql(char *host,char *user,char *pass,char *db)
{
	if (!mysql_real_connect(sock, host, user, pass, db, 0, NULL, CLIENT_MULTI_STATEMENTS))
	{
        //连接错误
		TRACE("%d\n",GetLastError());
		return false;
	}	

    return true;
}

bool CMySql::SelectMySql(char* szSql,int nColumn,list<string>& lstStr)
{
    //mysql_query() 函数用于向 MySQL 发送并执行 SQL 语句
     if(mysql_query(sock,szSql))
	{
		TRACE("%d\n",GetLastError());
		return false;
	}

	results=mysql_store_result(sock);
    if(NULL == results)
	{
		TRACE("%d\n",GetLastError());
		return false;
	}
	while (record = mysql_fetch_row(results))
	{
        
		 for(int i = 0;i < nColumn;i++)
         {
			if(!record[i])
				record[i] = "";
             lstStr.push_back(record[i]);
         }
	}
    return true;
}

 bool  CMySql::UpdateMySql(char* szSql)
 {
    if(!szSql)return false;

    if(mysql_query(sock,szSql))
	{
		TRACE("%d\n",GetLastError());
		return false;
	}
    return true;
 }


