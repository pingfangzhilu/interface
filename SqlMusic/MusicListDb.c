#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "musicList.h"

#define DATABASE_NAME	"music.db"  	//���ݿ���
#define MESSAGE_TABLE	"message"	//��¼������Ϣ����	

typedef struct{
	List_t list[MUSIC_LIST];
}MusicList_t;

MusicList_t *Mlist=NULL;


//���ݿ��:�������ID�ź����� (���������б���)
static void CreateMusicTable(const char *tableMusic){
	char sql[128]={0};
	snprintf(sql,128,"create table if not exists %s (id INTEGER PRIMARY KEY,name VARCHAR(64) not null unique);",tableMusic);            
	CreateTable(sql);
}
//��������б���Ϣ,���沥����ʷ��¼ֵ
static void CreateMusicListMesage(const char *tableMusic){
	char sql[128]={0};
	snprintf(sql,128,"create table if not exists %s (name VARCHAR(64) not null unique,number INTEGER,DirTime INTEGER);",tableMusic);            
	CreateTable(sql);
}

static void checkpath(const char *path,char *getPath){
	int len = strlen(path);
	if(*(path+len-1)=='/'){
		strncat(getPath,path,len);
	}else{
		strncat(getPath,path,len);
		strncat(getPath,"/",1);
	}
}
//����Ŀ¼����ȡ������ȥ������� / ����
static void GetTableName(const char *path,char *destPath){
	int len = strlen(path);
	if(*(path+len-1)=='/'){
		strncat(destPath,path,len-1);
	}else{
		strncat(destPath,path,len);
	}
}
static int onloadSdcardFile(const char *sdcard,List_t *list){
	DIR *dirptr = NULL;  
	char getPath[128]={0};
	int fileNum=0;
	checkpath(sdcard,getPath);
	strcat(getPath,list->listname);
	
	struct stat buf;
	if(stat(getPath, &buf)){
		return -1;
	}
	
	int mtime = (int)buf.st_mtime;
	if(mtime==list->DirTime){
		printf("%s dir  data is new \n",list->listname);
		return -1;
	}
	list->DirTime = mtime;//�������ݿ⵱��Ŀ¼����ʱ��
	struct dirent *entry;  
	if((dirptr = opendir(getPath)) == NULL)  {  
		perror("open dir failed!\n"); 
		return -1;  
	}

	while (entry = readdir(dirptr)){  
		if(!strcmp(entry->d_name,".")||!strcmp(entry->d_name,"..")){
			continue;
		}
		printf("list->listname = %s\n",list->listname);
		InsertSql(list->listname,entry->d_name);
		fileNum++;			
	}
	list->Nums =fileNum;
	closedir(dirptr);   
	return 0;
}
static enum{
	mp3_N=1,
	story_N,
	english_N,
	guoxue_N,
};

void SaveSystemPlayNum(void){
	List_t cachelist;
	int i=0;
	for(i=0;i<MUSIC_LIST;i++){
		snprintf(cachelist.listname,24,"%s%s","cache",Mlist->list[i].listname);
		if(InsertMusicMessageSQL(MESSAGE_TABLE,cachelist.listname,Mlist->list[i].playindex,0)){
			UpdateSqlByMessage(MESSAGE_TABLE,cachelist.listname,Mlist->list[i].playindex,0);
		}
		printf("Mlist->list[%d].playindex =%d\n",i,Mlist->list[i].playindex);
	}
#ifdef DOWN_URL_MUSIC
	SaveXiaiMusicPlayIndex();
#endif
}
/*
������ȡĿ¼�ļ������ص����ݿ⵱��
*/
int SysOnloadMusicList(const char *sdcard,const char *mp3Music,const char *story,const char *english,const char *guoxue){
	char sqlPath[128]={0};
	
	checkpath(sdcard,sqlPath);
	strcat(sqlPath,DATABASE_NAME);
	if(OpenSql(sqlPath) != 0)
		return -1;

	int i=0,listNum=MUSIC_LIST;
	GetTableName(mp3Music,Mlist->list[0].listname);
	GetTableName(story,Mlist->list[1].listname);
	GetTableName(english,Mlist->list[2].listname);
	GetTableName(guoxue,Mlist->list[3].listname);

	List_t cachelist;
	for(i=0;i<MUSIC_LIST;i++){
		// ����Ŀ¼���е� cachemp3 �ֶ����棬Nums��¼���ǹػ�֮�󣬲��ŵ�num���
		memset(&cachelist,0,sizeof(List_t));
		snprintf(cachelist.listname,24,"%s%s","cache",Mlist->list[i].listname);
		GetMusicMessageSQL(MESSAGE_TABLE,&cachelist);
		Mlist->list[i].playindex=cachelist.Nums;
		printf("Mlist->list[%d].playindex =%d\n",i,Mlist->list[i].playindex);
	}


#ifdef DOWN_URL_MUSIC	
	CreateMusicTable(XIMALA_MUSIC);
	GetXiaiMusicPlayIndex();
#endif	
	CreateMusicListMesage(MESSAGE_TABLE);
	for(i=0;i<listNum;i++){
		GetMusicMessageSQL(MESSAGE_TABLE,&(Mlist->list[i]));
		CreateMusicTable(Mlist->list[i].listname);
		//printf("Mlist->list[0].listname =%s\n",Mlist->list[i].listname);
		if(!onloadSdcardFile(sdcard,&(Mlist->list[i]))){
			if(InsertMusicMessageSQL(MESSAGE_TABLE,Mlist->list[i].listname,Mlist->list[i].Nums,Mlist->list[i].DirTime)){
				UpdateSqlByMessage(MESSAGE_TABLE,Mlist->list[i].listname,Mlist->list[i].Nums,Mlist->list[i].DirTime);
			}
		}
	}
	return 0;
}

/*

*/
int GetSdcardMusic(const char *sdcard,const char *musicDir,char *getMusicname,unsigned char Mode){
	int i=0;
	for(i=0;i<MUSIC_LIST;i++){
		if(!strncmp(Mlist->list[i].listname,musicDir,strlen(Mlist->list[i].listname))){
			break;
		}
		//printf("musicDir = %s Mlist->list[i].listname=%s\n",musicDir,Mlist->list[i].listname);
	}
	if(i>=MUSIC_LIST){
		printf("warning is GetSdcardMusic \n");
		return -1;
	}
	switch(Mode){
		case PLAY_NEXT:
			++Mlist->list[i].playindex;
			if(Mlist->list[i].playindex>Mlist->list[i].Nums){
				Mlist->list[i].playindex=1;//���ݿ⵱�е�ID �Ǵ�1��ʼ
			}
			break;
		case PLAY_PREV:
			--Mlist->list[i].playindex;
			if(Mlist->list[i].playindex<=0){
				Mlist->list[i].playindex=Mlist->list[i].Nums;
			}
			break;
		default:
			return -1;
	}
	printf("GetSdcardMusic : playindex=%d (%d)\n",Mlist->list[i].playindex,Mlist->list[i].Nums);
	return GetTableSqlById(Mlist->list[i].listname,Mlist->list[i].playindex,getMusicname);
}
#ifdef DOWN_URL_MUSIC
typedef struct{
	unsigned char playMode;
	char listname[24];
	int Nums;
	int playindex;
}NetMusic_t;
static NetMusic_t *netmusic=NULL;
int InsertXimalayaMusic(const char *musicDir,const char *musicName){
	return InsertSql(musicDir,musicName);
}
int DelXimalayaMusic(const char *musicDir,const char *musicName){
	return del_DBdata(musicDir,musicName);
}
static int table_result(void *args,int nRow,int nColumn,char **dbResult){
	char *getMusicname = (char *)args;
	int result, i, j, index;
	// dbResult ǰ���һ���������ֶ����ƣ��� nColumn ������ʼ��������������  
	if(nRow==0){
		return -1;
	}
	index = nColumn; 
	
	if(netmusic->playMode==PLAY_NEXT){
		if(++netmusic->playindex>=nRow){
			netmusic->playindex=0;
		}
	}else if(netmusic->playMode==PLAY_PREV){
		if(--netmusic->playindex<0){
			netmusic->playindex=nRow-1;
		}
	}
	int res = netmusic->playindex*nColumn+nColumn-1;
	
	//printf( "all cloumn: %d records nColumn=%d  netmusic->playindex = %d res =%d dbResult [res]=%s\n", nRow ,nColumn,netmusic->playindex,res,dbResult [res]);	
	snprintf(getMusicname,128,dbResult [res+2]);	
	//printf("dbResult[1] = %s dbResult[3]=%s dbResult[5] =%s dbResult[7] =%s\n",dbResult[1] ,dbResult[3] ,dbResult[5] ,dbResult[7] );
#if 1	
	for( i = 0; i < nRow ; i++ )  { 		
			for( j = 0 ; j < nColumn; j++ )  {  
				//if(netmusic->playindex==i)
				{
				printf( "char: %s\t value: %s\n", dbResult[j], dbResult [index] );	
				//dbResult ���ֶ�ֵ�������ģ��ӵ�0�������� nColumn - 1���������ֶ����ƣ��ӵ� nColumn ������ʼ�����涼���ֶ�ֵ��
				//����һ����ά�ı���ͳ�����б�ʾ������һ����ƽ����ʽ����ʾ  
			}  
			++index;	
		} 
	}
#endif	
	return 0;
}
int PlayxiaiMusic(const char *sdcard,const char *musicDir,char *getMusicname,unsigned char Mode){
	netmusic->playMode = Mode;
	return GetTableSql(musicDir,getMusicname,table_result);	
}
void SaveXiaiMusicPlayIndex(void){
	char cacheName[24]={0};
	snprintf(cacheName,24,"%s%s","cache",XIMALA_MUSIC);
	if(InsertMusicMessageSQL(MESSAGE_TABLE,cacheName,netmusic->playindex,0)){
		UpdateSqlByMessage(MESSAGE_TABLE,cacheName,netmusic->playindex,0);
	}
}
void GetXiaiMusicPlayIndex(void){
	List_t cachelist;
	memset(&cachelist,0,sizeof(List_t));
	// ����Ŀ¼���е� cachemp3 �ֶ����棬Nums��¼���ǹػ�֮�󣬲��ŵ�num���
	snprintf(cachelist.listname,24,"%s%s","cache",XIMALA_MUSIC);
	GetMusicMessageSQL(MESSAGE_TABLE,&cachelist);
	netmusic->playindex=cachelist.Nums;
	printf("netmusic->playindex=%d\n",netmusic->playindex);
}
#endif
int InitMusicList(void){
	Mlist = (MusicList_t *)calloc(1,sizeof(MusicList_t));
	if(Mlist==NULL){
		return -1;
	}
#ifdef DOWN_URL_MUSIC	
	netmusic= (NetMusic_t *)calloc(1,sizeof(NetMusic_t));
	if(netmusic==NULL){
		return -1;
	}
#endif
	return 0;
}
void CleanMusicList(void){
	if(Mlist){
		free(Mlist);
		Mlist=NULL;
	}
	if(netmusic){
		free(netmusic);
		netmusic=NULL;
	}
	CloseSql();
}

int checkMusicDb(const char *sdcard,const char *musicDir){

}
#define MAIN_TEST
	
#ifdef MAIN_TEST
void testPlayList(void){
	char musicname[128]={0};
	int i=0;
	for(i=0;i<7;i++)	{
		//GetSdcardMusic((const char *)"sdcard",(const char *)"keji",musicname,PLAY_NEXT);
		GetSdcardMusic((const char *)"sdcard/",(const char *)"why/",musicname,PLAY_PREV);
		printf("musicname[%d] = %s\n",i,musicname);
		memset(musicname,0,128);
	}
}
void test_NetMusic(void){
	char getMusicname[128]={0};
	int i=0;
	int ret=-1;
	for(i=0;i<8;i++)	{
		ret = PlayxiaiMusic((const char *)"sdcard/",(const char *)XIMALA_MUSIC,getMusicname,PLAY_NEXT);
		if(ret){
			printf("get XIMALA_MUSIC failed \n");
			continue;
		}
		printf("XIMALA_MUSIC[%d] = %s\n",i,getMusicname);
		memset(getMusicname,0,128);
	}
}
static void *InterfaceCmd(void){
	char cmd[20]={0};
	char getMusicname[128]={0};
	printf("InterfaceCmd \n");
	while(1){
		fgets(cmd,20,stdin);
		printf("cmd =%s \n",cmd);
		if(!strncmp(cmd,"1",1)){
			PlayxiaiMusic((const char *)"sdcard/",(const char *)XIMALA_MUSIC,getMusicname,PLAY_NEXT);
		}else if(!strncmp(cmd,"2",1)){
			PlayxiaiMusic((const char *)"sdcard/",(const char *)XIMALA_MUSIC,getMusicname,PLAY_PREV);
		}else if(!strncmp(cmd,"q",1)){
			exit(0);
		}
		printf("getMusicname = %s\n",getMusicname);
		memset(getMusicname,0,128);
		memset(cmd,0,20);
	}
	return NULL;
}
int main(void){
	InitMusicList();
	SysOnloadMusicList((const char *)"sdcard/",(const char *)"keji/",(const char *)"why/",(const char *)"english/",(const char *)"guoxue/");
	//testPlayList();
	pthread_t tid;
	
	pthread_create(&tid,NULL,InterfaceCmd,NULL);
#if 1
	//InsertXimalayaMusic((const char *)XIMALA_MUSIC,(const char *)"testmusic11");
#else
	DelXimalayaMusic((const char *)XIMALA_MUSIC,(const char *)"testmusic11");
	DelXimalayaMusic((const char *)XIMALA_MUSIC,(const char *)"testmusic12");
	DelXimalayaMusic((const char *)XIMALA_MUSIC,(const char *)"testmusic13");
	DelXimalayaMusic((const char *)XIMALA_MUSIC,(const char *)"testmusic14");
	DelXimalayaMusic((const char *)XIMALA_MUSIC,(const char *)"testmusic15");
	DelXimalayaMusic((const char *)XIMALA_MUSIC,(const char *)"testmusic16");
#endif
	while(1){
		sleep(1);
	}

	//test_NetMusic();
	//SaveSystemPlayNum();
	CleanMusicList();
	return 0;
}
#endif
