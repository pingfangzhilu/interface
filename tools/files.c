#include "comshead.h"
char *readFileBuf(const char *filename){
	FILE *fp = fopen(filename,"r");
    	if(fp==NULL){
		return NULL;
	}
    	fseek(fp,0,SEEK_END);
    	int len = ftell(fp);
    	fseek(fp,0,SEEK_SET);
	if(len==0){
		fclose(fp);
		return NULL;
	}
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


