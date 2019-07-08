#pragma once
#include<mysql.h>

#pragma comment(lib,"libmysql.lib")

#include <list>
using namespace std;


class CMySql
{
public:
    CMySql(void);
    ~CMySql(void);
public:
    bool  ConnectMySql(char *host,char *user,char *pass,char *db);
    void  DisConnect();
    bool  SelectMySql(char* szSql,int nColumn,list<string>& lstStr);
   
    //���£�ɾ�������롢�޸�
    bool  UpdateMySql(char* szSql);
 
private:
    //static MYSQL *sock;   
	MYSQL_RES *results;   
	MYSQL_ROW record; 
   
};
