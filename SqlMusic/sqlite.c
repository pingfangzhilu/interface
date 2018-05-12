#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <pthread.h>

#include "musicList.h"

static sqlite3 *db = NULL;
static char *errmsg = NULL;
#ifdef SQL_LOCK
static pthread_mutex_t sqlite_mutex;
#endif
extern int sqlite3_callback_GetCity( void * para, int n_column, char ** column_value, char ** column_name );

static void SqlLock(void){
#ifdef SQL_LOCK	
	pthread_mutex_lock(&sqlite_mutex);
#endif
}
static void SqlunLock(void){
#ifdef SQL_LOCK	
	pthread_mutex_unlock(&sqlite_mutex);
#endif
}
int OpenSql(const char *dbname){
	if(db != NULL)
		return -1;
	if(sqlite3_open(dbname,&db) != SQLITE_OK){
		perror("sqlite open failed!");
		db = NULL;
		return -1;
	}
	return 0;
}
/****************************************
�ر����ݿ�
****************************************/
void CloseSql(void){
	if(db != NULL){	
		sqlite3_free(errmsg);
		sqlite3_close(db);
		errmsg = NULL;
		db = NULL;
	}
}
/****************************************
������
****************************************/
int CreateTable(const char *sql){
	if(db == NULL)
		return -1;
	if(sqlite3_exec(db,sql,0,0,&errmsg) != SQLITE_OK){
		perror("create table failed!\n");
		return -1;
	}
	return 0;
}
/******************************
�����û����ݿ�
******************************/
int InsertSql(const char *table_name,const char *name){
	if(db == NULL)
		return -1;
	SqlLock();
	const char *sql = sqlite3_mprintf("insert into '%s' (name)values('%s');",table_name,name);
	if(sqlite3_exec(db,sql,0,0,&errmsg) != SQLITE_OK){
		printf("errmsg:%s\n",errmsg);
		SqlunLock();
		return -1;
	}
	SqlunLock();
	sql = NULL;
	return 0;
}
int InsertMusicMessageSQL(const char *table_name,const char *name,int num,int loadtime){
	if(db == NULL)
		return -1;
	SqlLock();
	const char *sql = sqlite3_mprintf("insert into '%s' (name,number,DirTime)values('%s','%d','%d');",table_name,name,num,loadtime);
	if(sqlite3_exec(db,sql,0,0,&errmsg) != SQLITE_OK){
		printf("errmsg:%s\n",errmsg);
		SqlunLock();
		return -1;
	}
	SqlunLock();
	sql = NULL;
	return 0;
}
static int CallGetMessage( void * para, int n_column, char ** column_value, char ** column_name ){
#if 0 	
 	int i;
	printf("n_column = %d\n", n_column );
	for( i = 0 ; i < n_column; i ++ )
	    printf( "column_name:%s  column_value:%s\n",  column_name[i], column_value[i] ); 
#endif
	List_t *list = (List_t *)para;
	if(para != NULL){
		//printf("column_value[1] = %s column_value[2]=%s\n",column_value[1],column_value[2]);
		list->Nums =atoi(column_value[1]);
		list->DirTime=atoi(column_value[2]);
	}
	return 0;
}
int GetMusicMessageSQL(const char *table_name,List_t *list){
	if(db == NULL)
		return -1;
	const char *sql = NULL;
	SqlLock();
	sql = sqlite3_mprintf("select  * from '%s' where name='%s';",table_name,list->listname);
	//printf("GetTableSqlById =%s\n",sql);
	if(sqlite3_exec(db,sql,CallGetMessage,(void *)list,&errmsg)!= SQLITE_OK){
		printf("execute get_passwd sql  failed!:%s\n",errmsg);
		SqlunLock();
		return -1;

	}
	SqlunLock();
	return 0;
}

static int call_get_vaule( void * para, int n_column, char ** column_value, char ** column_name ){
	//para������ sqlite3_exec �ﴫ��� void * ����
   	//ͨ��para����������Դ���һЩ�����ָ�루������ָ�롢�ṹָ�룩��Ȼ����������ǿ��ת���ɶ�Ӧ�����ͣ���������void*���ͣ�����ǿ��ת����������Ͳſ��ã���Ȼ�������Щ����
   	//n_column����һ����¼�ж��ٸ��ֶ� (��������¼�ж�����)
  	// char ** column_value �Ǹ��ؼ�ֵ������������ݶ������������ʵ�����Ǹ�1ά���飨��Ҫ��Ϊ��2ά���飩��ÿһ��Ԫ�ض���һ�� char * ֵ����һ���ֶ����ݣ����ַ�������ʾ����/0��β��
	 //char ** column_name �� column_value�Ƕ�Ӧ�ģ���ʾ����ֶε��ֶ�����    
#if 0 	
 	int i;
	printf("n_column = %d\n", n_column );
	for( i = 0 ; i < n_column; i ++ )
	    printf( "column_name:%s  column_value:%s\n",  column_name[i], column_value[i] ); 
#endif
	if(para != NULL){
		strcpy((char *)para,column_value[1]);
		//printf( "call_get_vaule:%s\n",(char *)para );
	}
	return 0;
}


int GetTableSqlById(const char *table_name,int id,char *name){	
	if(db == NULL)
		return -1;	
	const char *sql = NULL;
	SqlLock();
	sql = sqlite3_mprintf("select  * from '%s' where id='%d';",table_name,id);
	//printf("GetTableSqlById =%s\n",sql);
	if(sqlite3_exec(db,sql,call_get_vaule,(void *)name,&errmsg)!= SQLITE_OK){
		printf("execute get_passwd sql  failed!:%s\n",errmsg);
		SqlunLock();
		return -1;

	}
	SqlunLock();
	return 0;
}
int GetTableSql(const char *table_name,void *args,int table_result(void *args,int nRow,int nColumn,char **dbResult)){		
	char *errmsg = NULL;
	int result,nRow, nColumn;
	const char *sql 	= NULL;
	char **dbResult;	
	int ret;
	if(db == NULL)
		return -1;
	sql = sqlite3_mprintf("select * from '%s' ;",table_name);	
	result = sqlite3_get_table( db,sql, &dbResult, &nRow, &nColumn, &errmsg );	
	if( SQLITE_OK == result ){	
		ret = table_result(args,nRow, nColumn,dbResult);
		sqlite3_free_table( dbResult );  
		return ret;
	}
	return -1;
}
int UpdateSql(const char *table_name,const char *usrname,const char *newname){
	if(db == NULL)
		return -1;	
	SqlLock();
	const char *sql = sqlite3_mprintf("update '%s' set name='%s' where name='%s';",table_name,newname,usrname);	
	if(sqlite3_exec(db,sql,0,0,&errmsg) != SQLITE_OK){
		perror(" updata failed!");
		SqlunLock();
		return -1;
	}	
	SqlunLock();
	return 0;
}

int UpdateSqlByMessage(const char *table_name,const char *name,int number,int DirTime){
	if(db == NULL)
		return -1;
	SqlLock();
	const char *sql = sqlite3_mprintf("update '%s' set number='%d',DirTime='%d' where name='%s';",table_name,number,DirTime,name);	
	if(sqlite3_exec(db,sql,0,0,&errmsg) != SQLITE_OK){
		perror(" updata failed!");
		SqlunLock();
		return -1;
	}	
	SqlunLock();
	return 0;
}

int del_DBdata(const char *table_name,const char *usrname){
	if(db == NULL)
		return -1;	
	SqlLock();
	const char *sql = sqlite3_mprintf("delete from '%s' where name='%s';",table_name,usrname);
	if(sqlite3_exec(db,sql,0,0,&errmsg) != SQLITE_OK){
		SqlunLock();
		perror("delete user data failed !");
		return -1;
	}
	SqlunLock();
	return 0;
}

int get_table(const char *table_name,void *Para,int (*sqlite3_callback)( void * para, int n_column, char ** column_value, char ** column_name )){
	int ret;
	const char *sql = NULL;
	if(db == NULL)
		return -1;

	SqlLock();
	
	sql = sqlite3_mprintf("select  * from '%s' ;",table_name);
	printf("sqlite3_exec=%s\n",sql);
	ret = sqlite3_exec(db,sql,sqlite3_callback,Para,&errmsg);	
	sql = NULL;
	if(ret != SQLITE_OK){
		printf("get table  failed!:%s\n",errmsg);
		SqlunLock();
		return -1;
	}
	SqlunLock();
	printf("get table  succese!\n");
	return 0;
}

//#define SHOW_SQL_DB
#ifdef SHOW_SQL_DB
int show_table(const char *table_name){
	int result, i, j, index;
	const char *sql = NULL;
	char **dbResult; 		//�� char ** ���ͣ�����*��  
	int nRow, nColumn;  
	if(db == NULL)
		return -1;

	SqlLock();
	sql = sqlite3_mprintf("select * from '%s' ;",table_name);	
	result = sqlite3_get_table( db,sql, &dbResult, &nRow, &nColumn, &errmsg );  
	if( SQLITE_OK == result )	
	{	
	//��ѯ�ɹ�
	// dbResult ǰ���һ���������ֶ����ƣ��� nColumn ������ʼ��������������  
	printf( "all cloumn: %d records\n", nRow );	
	index = nColumn; 
	for( i = 0; i < nRow ; i++ )  
	{  
		for( j = 0 ; j < nColumn; j++ )  
		{  
			printf( "char: %s\t value: %s\n", dbResult[j], dbResult [index] );	
			++index;
			//dbResult ���ֶ�ֵ�������ģ��ӵ�0�������� nColumn - 1���������ֶ����ƣ��ӵ� nColumn ������ʼ�����涼���ֶ�ֵ��
			//����һ����ά�ı���ͳ�����б�ʾ������һ����ƽ����ʽ����ʾ  
		}  
	}
	sqlite3_free_table( dbResult );  
	
	}
	SqlunLock();
	return 0;
}
int __sql_del_table(const char *table_name){
	const char *sql = NULL;
	if(db == NULL)
		return -1;

	SqlLock();
	sql = sqlite3_mprintf("DROP TABLE '%s';",table_name);

	if(sqlite3_exec(db,sql,0,0,&errmsg) != SQLITE_OK)
	{
		SqlunLock();
		perror("delete user data failed !");
		return -1;
	}
	SqlunLock();
	return 0;

}
#endif
#ifdef SQL_LOCK
void init_sqlite3(void){
	pthread_mutex_init(&sqlite_mutex,NULL); 
}

void clean_sqlite3(void){
	pthread_mutex_destroy(&sqlite_mutex);
}
#endif
#if 0
int init_usrdb(void){
	if(OpenSql(DATABASE_NAME) != 0)
		return -1;
	char buf[125];
	char *sql = buf;
	char *music = "music";
	strcpy(sql,"create table if not exists ");
	strcat(sql,music);
	strcat(sql, "(id INTEGER PRIMARY KEY,name VARCHAR(64) not null unique);");
	printf("sql == %s\n",sql);               
	CreateTable(sql);
	sql = NULL;
	InsertSql(music,"0003");
	char name[64]={0};
	GetTableSqlById((const char *)music,1,name);
	printf("name = %s\n",name);
	show_table(music);
	CloseSql();
	
	return 1;
}

int main(void){
	init_usrdb();
	return 0;
}

#endif

