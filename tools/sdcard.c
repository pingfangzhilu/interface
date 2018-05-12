#include "comshead.h"

typedef long U64;
enum{ALL,USR,FREE};

int __safe_write(FILE *fp,const void *data,int input_size){
	int ret=0,w_size=0;
	if(fp==NULL){
		return -1;
	}
	while(1){
		ret = fwrite(data+w_size,1,input_size-w_size,fp);
		w_size +=ret;
		if(w_size==input_size){
			break;
		}
	}
	return 0;
}

//��ȡSD����Ϣ
int GetStorageInfo(char * MountPoint,  //SD�����һ������
                                 int *Capacity,  //  ��Ҫ��ȡ�Ŀռ��С
                                 int type) //��ȡʲô���͵Ŀռ�
{
  struct statfs statFS; //ϵͳstat�Ľṹ��
  U64 usedBytes = 0;
  U64 freeBytes = 0;
  U64 totalBytes = 0;
  U64 endSpace = 0;
 
   if (statfs(MountPoint, &statFS) == -1){	 //��ȡ������״̬
		 printf("statfs failed for path->[%s]\n", MountPoint);
		 return -1;
   }
 
   totalBytes = (U64)statFS.f_blocks * (U64)statFS.f_frsize; //��ϸ�ķ����������� ���ֽ�Ϊ��λ
   freeBytes = (U64)statFS.f_bfree * (U64)statFS.f_frsize; //��ϸ��ʣ��ռ����������ֽ�Ϊ��λ
   usedBytes = (U64)(totalBytes - freeBytes); //��ϸ��ʹ�ÿռ����������ֽ�Ϊ��λ
 
   switch( type ){
		 case ALL:
				 endSpace = totalBytes/1024/1024; //��MBΪ��λ��������
				 break;
		 case USR:
				 endSpace = usedBytes/1024/1024; //��MBΪ��λ��ʹ�ÿռ�
				 break;
		 case FREE:
				 endSpace = freeBytes/1024/1024; //��MBΪ��λ��ʣ��ռ�
				 break;
		 default:
				 return -1;
   }
   *Capacity = endSpace;
   return 0;
}
#define MAIN_TEST
#ifdef MAIN_TEST
int main(int argc,char **argv){
	int Capacity =0;
	GetStorageInfo(argv[1],&Capacity,ALL);
	printf("%s  size %d Mb\n",argv[1],Capacity);
	return 0;
}
#endif
