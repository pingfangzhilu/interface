#include <stdio.h>
#include <pthread.h>
#include "curldown.h"

char *url= "http://fdfs.xmcdn.com/group9/M08/A1/4A/wKgDZldzNWzRoXyzACZofeFKKKc093.mp3";

FILE *downfp=NULL;

static void StartDownFile(const char *filename,int streamLen){
	if(downfp==NULL){
		downfp= fopen(filename, "w+");
    	if (downfp ==NULL){
   			printf("Create file failed\n");
		}
	}
}

static void GetStreamData(const char *data,int size){
	printf("GetStreamData size = %d \n",size);
  	if(downfp){
    		fwrite((const void*)data, 1,size,downfp);
 	}
}

static void EndDownFile(int endSize){
	printf("endSize = %d \n",endSize);
 	if(downfp){
	    	fclose(downfp);
 	}
	downfp=NULL;
}

void *run_down(void *arg){
	setDowning();
	demoDownFile(url,15,StartDownFile,GetStreamData,EndDownFile);
}

int main(void){
	initCurl();
	pthread_t tid;
	pthread_create(&tid,NULL,run_down,NULL);

	sleep(10);
	return 0;
}

