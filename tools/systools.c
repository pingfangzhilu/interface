#include "comshead.h"
/********************************************
功能：搜索字符串右边起的第一个匹配字符
********************************************/
char* Rstrchr(char* s, char x)
{
    int i = strlen(s);
    if(!(*s))
    {
        return 0;
    }
    while(s[i-1])
    {
        if(strchr(s+(i-1), x))
        {
            return (s+(i-1));
        }
        else
        {
            i--;
        }
    }
    return 0;
}
/********************************************
功能：把字符串转换为全小写
********************************************/
void   ToLowerCase(char* s)
{
    while(*s && *s!='\0' )
    {
        *s=tolower(*s++);
    }
    *s = '\0';
}
void parse_url(const char *url, char *domain, int *port, char *file_name){	 
	int j = 0;	  
	int start = 0;
	*port = 80;
	char *patterns[] = {"http://", "https://", NULL};
	int i;
	for(i = 0; patterns[i]; i++)
		if (strncmp(url, patterns[i], strlen(patterns[i])) == 0)
			start = strlen(patterns[i]);
	for( i = start; url[i] != '/' && url[i] != '\0'; i++, j++)
		domain[j] = url[i];
	domain[j] = '\0';
	char *pos = strstr(domain, ":");
    	if (pos)        
		sscanf(pos, ":%d", port);
	for( i = 0; i < (int)strlen(domain); i++)
    	{        
    		if (domain[i] == ':')        
		{            
			domain[i] = '\0';
			break;
		}
	}
    	j = 0;    
	for ( i = start; url[i] != '\0'; i++)
	{        
		if (url[i] == '/')        
		{            
			if (i !=  strlen(url) - 1)                
				j = 0;            
				continue;        
		}        
		else            
			file_name[j++] = url[i];    
	}    
	file_name[j] = '\0';
}

