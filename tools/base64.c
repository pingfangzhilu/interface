#include "comshead.h"

#if 0
/*
Linux�ṩ�������з�ʽ��base64����ͽ��롣
��ʽ��echo "str" | base64
���ַ���str+���� ����Ϊbase64�ַ��������(�ͳ������еĽ��һ��)
��ʽ��echo -n "str" | base64#include "comshead.h"
���ַ���str����Ϊbase64�ַ��������
*/


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
#if 0
static char    alphabet64[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/',
};
#endif
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
                //error(E_L, E_LOG, T("Bad string: %s at %c index %d"), string,
                //      c, i);
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
            //gstrcpy(outbuf, T("String too big"));
            return -1;
        }
        for (j = 0; j < i; j++) {
            *op++ = (char) ((shiftbuf >> (8 * (2 - j))) & 0xff);
        }
        *op = '\0';
    }
    return 0;
}
/******************************************************************************/
/*
 *    Encode a buffer from "string" into "outbuf"
 */
void websEncode64(char *outbuf, char *string, int outlen)
{
    unsigned long    shiftbuf;
    char            *cp, *op;
    int                 x,i, j, shift;
    //����base64�����
    unsigned char alphabet64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    op = outbuf;
    *op = '\0';
    cp = string;
    while (*cp) {
        /*
         *        Take three characters and create a 24 bit number in shiftbuf
         */
        shiftbuf = 0;
	// note : ����Ҫ����������м��ֶΰ���0 ,����ֺ��沿�ֱ��벻����
        for (j = 2; j >= 0 && *cp; j--, cp++) {
            shiftbuf |= ((*cp & 0xff) << (j * 8));
        }
        /*
         *        Now convert shiftbuf to 4 base64 letters. The i,j magic calculates
         *        how many letters need to be output.
         */
        shift = 18;
        for (i = ++j; i < 4 && op < &outbuf[outlen] ; i++) {
            x = (shiftbuf >> shift) & 0x3f;
            *op++ = alphabet64[(shiftbuf >> shift) & 0x3f];
            shift -= 6;
        }
        /*
         *        Pad at the end with '='
         */
        while (j-- > 0) {
            *op++ = '=';
        }
        *op = '\0';
    }
}
#endif
unsigned char *base64_encode(unsigned char *str,int inSize)
{
    long len;
    long str_len;
    unsigned char *res;
    int i,j;
    //����base64�����
    unsigned char *base64_table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //���㾭��base64�������ַ�������
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
    //��3��8λ�ַ�Ϊһ����б���
    for(i=0,j=0;i<len-2;j+=3,i+=4)
    {   
        res[i]=base64_table[str[j]>>2]; //ȡ����һ���ַ���ǰ6λ���ҳ���Ӧ�Ľ���ַ�
        res[i+1]=base64_table[(str[j]&0x3)<<4 | (str[j+1]>>4)]; //����һ���ַ��ĺ�λ��ڶ����ַ���ǰ4λ������ϲ��ҵ���Ӧ�Ľ���ַ�
        res[i+2]=base64_table[(str[j+1]&0xf)<<2 | (str[j+2]>>6)]; //���ڶ����ַ��ĺ�4λ��������ַ���ǰ2λ��ϲ��ҳ���Ӧ�Ľ���ַ�
        res[i+3]=base64_table[str[j+2]&0x3f]; //ȡ���������ַ��ĺ�6λ���ҳ�����ַ�
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
    //����base64�����ַ��ҵ���Ӧ��ʮ��������
    int table[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
    long len;
    long str_len;
    unsigned char *res;
    int i,j;

    //����������ַ�������
    len=inSize;
    //�жϱ������ַ������Ƿ���=
    if(strstr((const char *)code,(const char *)"=="))
        str_len=len/4*3-2;
    else if(strstr((const char *)code,(const char *)"="))
        str_len=len/4*3-1;
    else
        str_len=len/4;

    res=malloc(sizeof(unsigned char)*str_len+1);
    res[str_len]='\0';

    //��4���ַ�Ϊһλ���н���
    for(i=0,j=0;i < len-2;j+=3,i+=4)
    {   
        res[j]=((unsigned char)table[code[i]])<<2 | (((unsigned char)table[code[i+1]])>>4); //ȡ����һ���ַ���Ӧbase64���ʮ��������ǰ6λ��ڶ����ַ���Ӧbase64���ʮ�������ĺ�2>λ>�������
        res[j+1]=(((unsigned char)table[code[i+1]])<<4) | (((unsigned char)table[code[i+2]])>>2); //ȡ���ڶ����ַ���Ӧbase64���ʮ�������ĺ�4λ��������ַ���Ӧbas464���ʮ�������ĺ�4λ�������
        res[j+2]=(((unsigned char)table[code[i+2]])<<6) | ((unsigned char)table[code[i+3]]); //ȡ���������ַ���Ӧbase64���ʮ�������ĺ�2λ���4���ַ��������
    }   

    return res;

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
