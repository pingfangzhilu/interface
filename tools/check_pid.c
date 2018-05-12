#include "comshead.h"


#define READ_BUF_SIZE 1024
//存在返回-1，不存在返回0
int judge_pid_exist(int get_pid_name(char *pid_name))
{
    DIR *dir;
    struct dirent *next;
    int i = 0;

    FILE *status;
    char buffer[READ_BUF_SIZE];
    char name[READ_BUF_SIZE];

    ///proc中包括当前的进程信息,读取该目录
    dir = opendir("/proc");
    if (!dir){
        printf("Cannot open /proc\n");
        return -1;
    }
	while ((next = readdir(dir)) != NULL){
		//跳过"."和".."两个文件名
		if ((strcmp(next->d_name, "..") == 0) || (strcmp(next->d_name, ".") == 0)){
			   continue;
		}
		//如果文件名不是数字则跳过
		if (!isdigit(*next->d_name)){
			   continue;
		}
		//判断是否能打开状态文件
		sprintf(buffer,"/proc/%s/status",next->d_name);
		if (!(status = fopen(buffer,"r"))){
			   continue;
		}
		//读取状态文件
		if (fgets(buffer,READ_BUF_SIZE,status) == NULL){
			   fclose(status);
			   continue;
		}
		fclose(status);
		sscanf(buffer,"%*s %s",name);//读取PID对应的程序名，格式为Name:  程序名
  	  	if(!get_pid_name(name)){
			closedir(dir);
			return 0;
		}
	}
	closedir(dir);
	return -1;
}

