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

//获取SD卡信息
int GetStorageInfo(char * MountPoint,  //SD卡随便一个分区
                                 int *Capacity,  //  想要获取的空间大小
                                 int type) //获取什么类型的空间
{
  struct statfs statFS; //系统stat的结构体
  U64 usedBytes = 0;
  U64 freeBytes = 0;
  U64 totalBytes = 0;
  U64 endSpace = 0;
 
   if (statfs(MountPoint, &statFS) == -1){	 //获取分区的状态
		 printf("statfs failed for path->[%s]\n", MountPoint);
		 return -1;
   }
 
   totalBytes = (U64)statFS.f_blocks * (U64)statFS.f_frsize; //详细的分区总容量， 以字节为单位
   freeBytes = (U64)statFS.f_bfree * (U64)statFS.f_frsize; //详细的剩余空间容量，以字节为单位
   usedBytes = (U64)(totalBytes - freeBytes); //详细的使用空间容量，以字节为单位
 
   switch( type ){
		 case ALL:
				 endSpace = totalBytes/1024/1024; //以MB为单位的总容量
				 break;
		 case USR:
				 endSpace = usedBytes/1024/1024; //以MB为单位的使用空间
				 break;
		 case FREE:
				 endSpace = freeBytes/1024/1024; //以MB为单位的剩余空间
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
