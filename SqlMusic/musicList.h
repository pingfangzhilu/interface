#ifndef _MUSICLIST_H
#define _MUSICLIST_H

typedef struct{
	char listname[24];
	int Nums;
	int DirTime;
	int playindex;
}List_t;

#define DOWN_URL_MUSIC

#define PLAY_NEXT		1		//������һ�׸���
#define PLAY_PREV		2 		//������һ�׸���
#define PLAY_NEXT_AUTO	3 		//������һ�׸���

#ifdef DOWN_URL_MUSIC
#define XIMALA_MUSIC	"ximalaya"	//ϲ�������ղصĸ���		

//��ϲ�����ŵĸ������ص�����
extern int InsertXimalayaMusic(const char *musicDir,const char *musicName);
extern int DelXimalayaMusic(const char *musicDir,const char *musicName);
#endif
#define MUSIC_LIST	4
extern int SysOnloadMusicList(const char *sdcard,const char *mp3Music,const char *story,const char *english,const char *guoxue);
extern int GetSdcardMusic(const char *sdcard,const char *musicDir,char *getMusicname,unsigned char Mode);
extern int InitMusicList(void);
extern void CleanMusicList(void);

#endif
