#include "comshead.h"

unsigned char *base64_encode(unsigned char *str,int inSize)
{
    long len;
    long str_len;
    unsigned char *res;
    int i,j;
    //定义base64编码表
    unsigned char *base64_table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //计算经过base64编码后的字符串长度
    //str_len=strlen(str);
    str_len=inSize;
    if(str_len % 3 == 0)
    {   
        len=str_len/3*4;
    }   
    else{
        len=(str_len/3+1)*4;
     }   
    //printf("str_len = %d\n",str_len);    
    res=malloc(sizeof(unsigned char)*len+1);
    res[len]='\0';
    //printf("len = %d\n",len);   
    //以3个8位字符为一组进行编码
    for(i=0,j=0;i<len-2;j+=3,i+=4)
    {   
        res[i]=base64_table[str[j]>>2]; //取出第一个字符的前6位并找出对应的结果字符
        res[i+1]=base64_table[(str[j]&0x3)<<4 | (str[j+1]>>4)]; //将第一个字符的后位与第二个字符的前4位进行组合并找到对应的结果字符
        res[i+2]=base64_table[(str[j+1]&0xf)<<2 | (str[j+2]>>6)]; //将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符
        res[i+3]=base64_table[str[j+2]&0x3f]; //取出第三个字符的后6位并找出结果字符
    }   

    switch(str_len % 3)
    {   
        case 1:
            res[i-2]='=';
            res[i-1]='=';
            break;
        case 2:
            res[i-1]='=';
            break;
    }   

    return res;
}
unsigned char *base64_decode(unsigned char *code,int inSize)
{
    //根据base64表，以字符找到对应的十进制数据
    int table[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
    long len;
    long str_len;
    unsigned char *res;
    int i,j;

    //计算解码后的字符串长度
    len=inSize;
    //判断编码后的字符串后是否有=
    if(strstr((const char *)code,(const char *)"=="))
        str_len=len/4*3-2;
    else if(strstr((const char *)code,(const char *)"="))
        str_len=len/4*3-1;
    else
        str_len=len/4;

    res=malloc(sizeof(unsigned char)*str_len+1);
    res[str_len]='\0';

    //以4个字符为一位进行解码
    for(i=0,j=0;i < len-2;j+=3,i+=4)
    {   
        res[j]=((unsigned char)table[code[i]])<<2 | (((unsigned char)table[code[i+1]])>>4); //取出第一个字符对应base64表的十进制数的前6位与第二个字符对应base64表的十进制数的后2>位>进行组合
        res[j+1]=(((unsigned char)table[code[i+1]])<<4) | (((unsigned char)table[code[i+2]])>>2); //取出第二个字符对应base64表的十进制数的后4位与第三个字符对应bas464表的十进制数的后4位进行组合
        res[j+2]=(((unsigned char)table[code[i+2]])<<6) | ((unsigned char)table[code[i+3]]); //取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合
    }   

    return res;

}

static char    map64[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};
/*********************************** Code *************************************/
/*
 *    Decode a buffer from "string" and into "outbuf"
 */
int websDecode64(char *outbuf, char *string, int outlen)
{
    unsigned long    shiftbuf;
    char            *cp, *op;
    int                c, i, j, shift;
    op = outbuf;
    *op = '\0';
    cp = string;
    while (*cp && *cp != '=') {
        /*
         *        Map 4 (6bit) input bytes and store in a single long (shiftbuf)
         */
        shiftbuf = 0;
        shift = 18;
        for (i = 0; i < 4 && *cp && *cp != '='; i++, cp++) {
            c = map64[*cp & 0xff];
            if (c == -1) {
                printf("Bad string: %s at %c index %d \n", string,c, i);
                return -1;
            }
            shiftbuf = shiftbuf | (c << shift);
            shift -= 6;
        }
        /*
         *        Interpret as 3 normal 8 bit bytes (fill in reverse order).
         *        Check for potential buffer overflow before filling.
         */
        --i;
        if ((op + i) >= &outbuf[outlen]) {
            strcpy(outbuf, "String too big");
            return -1;
        }
        for (j = 0; j < i; j++) {
            *op++ = (char) ((shiftbuf >> (8 * (2 - j))) & 0xff);
        }
        *op = '\0';
    }
    return 0;
}

#if 0
int main(int argc,char **argv)
{
        if(argc<2){
                printf("intput base encode string \n");
                return -1;
        }
        char *string =argv[1];
        char outbuf[128]={0};
        int outlen =sizeof(outbuf);

        websEncode64(outbuf, string, outlen);
        printf("out: %s \n",outbuf);
        return 0;
}
#endif
