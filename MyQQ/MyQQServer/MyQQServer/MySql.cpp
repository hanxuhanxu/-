#include "stdafx.h"
#include "MySql.h"


MYSQL* sock = new MYSQL;
CMySql::CMySql(void)
{
    /*�����������������߳�ʼ��һ��MYSQL������������mysql����ˡ�
    ����㴫��Ĳ�����NULLָ�룬�����Զ�Ϊ�����һ��MYSQL����
    ������MYSQL���������Զ�����ģ���ô�ڵ���mysql_close��ʱ�򣬻��ͷ��������*/
    
   mysql_init(sock );  
    mysql_set_character_set(sock,"gb2312"); //gb2312 �л����񹲺͹������ֱ�׼
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
        //���Ӵ���
		TRACE("%d\n",GetLastError());
		return false;
	}	

    return true;
}

bool CMySql::SelectMySql(char* szSql,int nColumn,list<string>& lstStr)
{
    //mysql_query() ���������� MySQL ���Ͳ�ִ�� SQL ���
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


