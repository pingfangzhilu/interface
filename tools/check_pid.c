#include "comshead.h"


#define READ_BUF_SIZE 1024
//���ڷ���-1�������ڷ���0
int judge_pid_exist(int get_pid_name(char *pid_name))
{
    DIR *dir;
    struct dirent *next;
    int i = 0;

    FILE *status;
    char buffer[READ_BUF_SIZE];
    char name[READ_BUF_SIZE];

    ///proc�а�����ǰ�Ľ�����Ϣ,��ȡ��Ŀ¼
    dir = opendir("/proc");
    if (!dir){
        printf("Cannot open /proc\n");
        return -1;
    }
	while ((next = readdir(dir)) != NULL){
		//����"."��".."�����ļ���
		if ((strcmp(next->d_name, "..") == 0) || (strcmp(next->d_name, ".") == 0)){
			   continue;
		}
		//����ļ�����������������
		if (!isdigit(*next->d_name)){
			   continue;
		}
		//�ж��Ƿ��ܴ�״̬�ļ�
		sprintf(buffer,"/proc/%s/status",next->d_name);
		if (!(status = fopen(buffer,"r"))){
			   continue;
		}
		//��ȡ״̬�ļ�
		if (fgets(buffer,READ_BUF_SIZE,status) == NULL){
			   fclose(status);
			   continue;
		}
		fclose(status);
		sscanf(buffer,"%*s %s",name);//��ȡPID��Ӧ�ĳ���������ʽΪName:  ������
  	  	if(!get_pid_name(name)){
			closedir(dir);
			return 0;
		}
	}
	closedir(dir);
	return -1;
}

