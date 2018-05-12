#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "base/fileMes.h"
#include "base/cJSON.h"

typedef struct{
	int modifyTime;
	int fileNum;
}OnloadFile_t;

static OnloadFile_t *loadFile=NULL;

enum{
	ONLOAD_OK,				//需要更新
	ONLOAD_FAILED,			//加载失败
	ONLOAD_NEW_DATA,		//已经是最新数据
};

char *readFileBuf(const char *filename){
	FILE *fp = fopen(filename,"r");
    if(fp==NULL){
		return NULL;
	}
    fseek(fp,0,SEEK_END);
    int len = ftell(fp);
    fseek(fp,0,SEEK_SET);

    char *data = (char *)calloc(1,len+1);
    if(data==NULL){
    	free(data);
     	return NULL;
 	}
   	int rSize =0,gSize=0;
   	do{
		rSize =fread(data+gSize,len,1,fp);
      	gSize +=rSize;
	}while(gSize==len);
	fclose(fp);
	return data;
}

static void getJsonFile(const char *dirname,char JsonFile[]){
	int len = strlen(dirname);
	if(*(dirname+len-1)=='/'){
		strncat(JsonFile,dirname,len-1);
	}else{
		strncat(JsonFile,dirname,len);
	}
	strcat(JsonFile,".json");
}
static int GetModife(OnloadFile_t *loadFile,const char *dirpath,const char *jsonfile){
	cJSON* pRoot =NULL;
    	cJSON* pArray=NULL;
	if(access(jsonfile,F_OK)<0){
		return ONLOAD_OK;
	}
	char *json = readFileBuf((const char *)jsonfile);
	if(json==NULL){
		return ONLOAD_FAILED;
	}
    	pRoot = cJSON_Parse(json);
	int time = cJSON_GetObjectItem(pRoot, "time")->valueint;
	struct stat buf;
	if(stat(dirpath, &buf)){
		return ONLOAD_FAILED;
	}
	
	int mtime = (int)buf.st_mtime;
	if(mtime==time){
		printf("json data is new \n");
		loadFile->fileNum = cJSON_GetObjectItem(pRoot, "number")->valueint;
		return ONLOAD_NEW_DATA;
	}
	loadFile->modifyTime=mtime;
	return ONLOAD_OK;
}
static int onloadSdcardFile(OnloadFile_t *loadFile,const char *dirpath,const char *saveFile){
	int ret=ONLOAD_FAILED;
	DIR *dirptr = NULL;  
	struct dirent *entry;  
	char itemFile[64]={0};
	if((dirptr = opendir(dirpath)) == NULL)  {  
		perror("open dir failed!\n"); 
		return ONLOAD_FAILED;  
	}
    	cJSON* pRoot = cJSON_CreateObject();
    	cJSON* pArray = cJSON_CreateArray();
	char* szJSON = NULL;
	
	cJSON_AddNumberToObject(pRoot, "time", loadFile->modifyTime);
    	cJSON_AddItemToObject(pRoot, "mp3_info", pArray);
	cJSON* pItem =NULL;
	while (entry = readdir(dirptr)){  
		if(!strcmp(entry->d_name,".")||!strcmp(entry->d_name,"..")){
			continue;
		}
		snprintf(itemFile,64,"fileNum%d",loadFile->fileNum);
		pItem = cJSON_CreateObject();
    		cJSON_AddStringToObject(pItem, "name", entry->d_name);
    		cJSON_AddItemToArray(pArray, pItem);
		loadFile->fileNum++;
	}
	closedir(dirptr);   
	cJSON_AddNumberToObject(pRoot, "number", loadFile->fileNum);
	szJSON = cJSON_Print(pRoot);
	printf("szJSON :\n%s\n",szJSON);
	FILE *fp = fopen(saveFile,"w+");	
	if(fp==NULL){
		goto exit1;
	}
	fwrite(szJSON,strlen(szJSON),1,fp);
	fclose(fp);
	ret=ONLOAD_OK;
exit1:	
	cJSON_Delete(pItem);
	return ret;
}

/*
开机将t卡目录当中的文件加载 到成json文件
*/
int sysOnloadFile(const char *sdcard,const char *dirname){
	char JsonFile[128]={0};
	char dirpath[128]={0};
	loadFile = (OnloadFile_t *)calloc(1,sizeof(OnloadFile_t));
	if(loadFile==NULL){
		return ONLOAD_FAILED;
	}
	snprintf(JsonFile,128,"%s",sdcard);
	getJsonFile(dirname,JsonFile);
	snprintf(dirpath,128,"%s%s",sdcard,dirname);
	int ret =GetModife(loadFile,(const char *)dirpath,JsonFile);
	if(ret ==ONLOAD_OK){
		onloadSdcardFile(loadFile,(const char *)dirpath,JsonFile);
	}
	free(loadFile);
	return ONLOAD_OK;
}
static int pasreSdcardFile(const char *jsondata,char *musicName,int playMp3Num){
	cJSON* pRoot =NULL;
    cJSON* pArray=NULL;
    pRoot = cJSON_Parse(jsondata);
    pArray = cJSON_GetObjectItem(pRoot, "mp3_info");
    if (NULL == pArray) {
        return -1;
    }
	int num = cJSON_GetObjectItem(pRoot, "number")->valueint;
	if(num<playMp3Num){
		return -1;
	}
#ifdef LOG_JSON	
	int iCount = cJSON_GetArraySize(pArray);
    int i = 0;	
    for (; i < iCount; ++i) {
        cJSON* pItem = cJSON_GetArrayItem(pArray, i);
        if (NULL == pItem){
            continue;
        }
        char *strName = cJSON_GetObjectItem(pItem, "name")->valuestring;
        printf("---name=%s\n", strName);
    }
#else
	cJSON* pItem = cJSON_GetArrayItem(pArray, playMp3Num);
	char *Name = cJSON_GetObjectItem(pItem, "name")->valuestring;
	sprintf(musicName,"%s",Name);
#endif	
    	cJSON_Delete(pRoot);
	return 0;
}


int GetMp3Filename(const char *sdcard,const char *dirname,char *filename,int playMp3Num){
	char JsonFile[128]={0};
	char dirpath[128]={0};
	snprintf(JsonFile,128,"%s",sdcard);
	getJsonFile(dirname,JsonFile);
	char *json = readFileBuf((const char *)JsonFile);
	if(json==NULL){
		return -1;
	}
	
	snprintf(dirpath,128,"%s%s",sdcard,dirname);
	pasreSdcardFile((const char *)json,filename,playMp3Num);
	free(json);
	return 0;
}

int GetMp3Num(const char *sdcard,const char *dirname){
	char JsonFile[128]={0};
	char dirpath[128]={0};
	snprintf(JsonFile,128,"%s",sdcard);
	getJsonFile(dirname,JsonFile);
	char *json = readFileBuf((const char *)JsonFile);
	if(json==NULL){
		return -1;
	}	
	cJSON* pRoot =NULL;
    	cJSON* pArray=NULL;
    	pRoot = cJSON_Parse(json);
	int num = cJSON_GetObjectItem(pRoot, "number")->valueint;
	cJSON_Delete(pRoot);
	free(json);
	return num;
}

int main(int argc,char **argv){
	if(argc<2)
		return -1;

	sysOnloadFile("/home/",argv[1]);
	char getname[64]={0};
	GetMp3Filename("/home/",argv[1],getname,1);
	int num = GetMp3Num("/home/",argv[1]);
	printf("getname = %s  num =%d \n",getname,num);
	
	return 0;
}
