#include "comshead.h"

/****************************************************************
函数说明	:获取当前系统时间
入口参数	:
返回类型	:fmt   0-返回:yyyy-mm-dd hh24:mi:ss
		       1-返回:yyyy-mm-dd
		       2-返回:hh24:mi:ss
修改记录	:
用途功能	:获取当前系统时间,用做写Log日志
****************************************************************/
static int getTime(char *out, int fmt)
{
	if(out == NULL)
	{
	    return -1;
	}
	time_t t;
	struct tm *tp;
	t = time(NULL);


	tp = localtime(&t);
	if(fmt == 0)
	{
	    sprintf(out, "%2.2d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
	}
	else if(fmt == 1)
	{
	    sprintf(out, "%2.2d-%2.2d-%2.2d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday);
	}
	else if(fmt == 2)
	{
	    sprintf(out, "%2.2d:%2.2d:%2.2d", tp->tm_hour, tp->tm_min, tp->tm_sec);
	}
	return 0;
}

/****************************************************************
函数说明	:写入Log  日志
入口参数	:bLog  表明是否为日志文件,place 代码位置
返回类型	:
修改记录	:
用途功能	:如果是日志文件，将会在str前加上当前时间(格式如：2011-04-12 12:10:20)
****************************************************************/
int writeLog(const char *filename,const char *str)
{
    char curTime[100] = {0};
    int ret = -1;
    memset(curTime,0,100);
	if(filename==NULL)
	{
		return 0;
	}
	FILE *fp = fopen(filename,"a+");
	if(NULL == fp || NULL == str)	
    {
		return -1;
    }
    getTime(curTime, 0);
    ret = fprintf(fp, "[%s] : %s\n", curTime, str);
    if(ret < 0)
    {
       	return -1;
    }
  	fflush(fp);
	fclose(fp);
	return 0;
}

