/* ------------------------------------------------------------------
 * Copyright (C) 2009 Martin Storsjo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include "wavreader.h"
#include "interf_enc.h"

/*******************************************************
函数功能:将wav格式文件编码成amr格式数据
参数:	wavsrc wav格式数据内容 Outdata amr格式数据内容 OutSize数据大小
返回值: -1 编码失败 0 编码成功
********************************************************/
int WavToAmr8k(const char *wavsrc,char *Outdata,int *OutSize){
	int mode = 7;
	int  dtx = 0;
	void *wav, *amr;
	int format, sampleRate, channels, bitsPerSample;
	int inputSize=1*2*160;
	uint8_t *inputBuf;

	wav = check_wavdata((const char *)wavsrc);
	if (!wav) {
		return -1;
	}
	if (!wav_get_header_inter(wav, &format, &channels, &sampleRate, &bitsPerSample, NULL)){
		fprintf(stderr, "Bad wav file \n");
		return -1;
	}
	if (format != 1) {
		fprintf(stderr, "Unsupported WAV format %d\n", format);
		return -1;
	}
	if (bitsPerSample != 16) {
		fprintf(stderr, "Unsupported WAV sample depth %d\n", bitsPerSample);
		return -1;
	}
	if (channels != 1)
		fprintf(stderr, "Warning, only compressing one audio channel\n");
	if (sampleRate != 8000)
		fprintf(stderr, "Warning, AMR-NB uses 8000 Hz sample rate (WAV file has %d Hz)\n", sampleRate);

	amr = Encoder_Interface_init(dtx);

	int pos =6;
	memcpy(Outdata, "#!AMR\n", pos);
	short buf[160];
	char outbuf[160];
	int read, i, n;
	while (1) {
		
		read = wav_read_data_inter(wav,wavsrc, &inputBuf, inputSize);
		read /= channels;
		read /= 2;
		if (read < 160)
			break;
		for (i = 0; i < 160; i++) {
			const uint8_t* in = &inputBuf[2*channels*i];
			buf[i] = in[0] | (in[1] << 8);
		}
		n = Encoder_Interface_Encode(amr, mode, buf, outbuf, 0);
		memcpy(Outdata+pos, outbuf, n);
		pos	+=n;
	}
	Encoder_Interface_exit(amr);
	wav_read_close_inter(wav);
	*OutSize = pos;
	return 0;
}
/*******************************************************
函数功能:将wav格式文件编码成amr格式文件
参数:	infile 输入文件路径 outfile 输出文件路径
返回值: -1 编码失败 0 编码成功
********************************************************/
int WavToAmr8kFile(const char *infile,const char *outfile){
	int mode = 7;
	int  dtx = 0;
	FILE *out;
	void *wav, *amr;
	int format, sampleRate, channels, bitsPerSample;
	int inputSize=1*2*160;
	uint8_t inputBuf[inputSize];

	wav = check_file((const char *)infile);
	if (!wav) {
		return -1;
	}
	if (!wav_get_header_inter(wav, &format, &channels, &sampleRate, &bitsPerSample, NULL)){
		fprintf(stderr, "Bad wav file %s\n", infile);
		return -1;
	}
	if (format != 1) {
		fprintf(stderr, "Unsupported WAV format %d\n", format);
		return -1;
	}
	if (bitsPerSample != 16) {
		fprintf(stderr, "Unsupported WAV sample depth %d\n", bitsPerSample);
		return -1;
	}
	if (channels != 1)
		fprintf(stderr, "Warning, only compressing one audio channel\n");
	if (sampleRate != 8000)
		fprintf(stderr, "Warning, AMR-NB uses 8000 Hz sample rate (WAV file has %d Hz)\n", sampleRate);

	amr = Encoder_Interface_init(dtx);
	out = fopen(outfile, "wb");
	if (!out) {
		perror(outfile);
		return -1;
	}

	fwrite("#!AMR\n", 1, 6, out);
	while (1) {
		short buf[160];
		uint8_t outbuf[500];
		int read, i, n;
		read = wav_read_file_inter(wav, inputBuf, inputSize);
		read /= channels;
		read /= 2;
		if (read < 160)
			break;
		for (i = 0; i < 160; i++) {
			const uint8_t* in = &inputBuf[2*channels*i];
			buf[i] = in[0] | (in[1] << 8);
		}
		n = Encoder_Interface_Encode(amr, mode, buf, outbuf, 0);
		fwrite(outbuf, 1, n, out);
	}
	fclose(out);
	Encoder_Interface_exit(amr);
	wav_read_close_inter(wav);
	return 0;
}

#define MAIN_TEST
#ifdef MAIN_TEST
static int get_file_len(FILE *fp)
{
	int fsta,fend,cur_pos;
    cur_pos = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    fsta = ftell(fp);
    fseek(fp, 0, SEEK_END);
    fend = ftell(fp);

    fseek(fp, cur_pos, SEEK_SET);
   	printf("fend=%d  fsta=%d \n",fend,fsta);
    return  fend - fsta;
}
int main(int argc, char *argv[]) {
	if(argc<2)
	{
		printf("input enc src wav file \n");
		return -1;
	}
#if 0
       	FILE *fp = fopen(argv[1],"r");
        int size = get_file_len(fp);
        char *wavsrc = (char *)malloc(size);
        char *amrsrc=NULL;
        int a_size;
        fread(wavsrc,1,size,fp);
		amrsrc = (char *)malloc(20480);	//20k
	
        WavToAmr8k(wavsrc,amrsrc,&a_size);
        printf("amrsrc  addr %p  size = %d\n",amrsrc,a_size);
        fclose(fp);
		FILE *out = fopen("test.amr","w+");
		if(out==NULL)
		{
			goto exit1;		
		}
		fwrite(amrsrc,a_size,1,out);
		fclose(out);
		printf("wav to amr ok ...\n");
exit1:
        free(wavsrc);
        if(amrsrc)
             free(amrsrc);
	return 0;
#else
	WavToAmr8kFile((const char *)argv[1],(const char *)argv[2]);	
#endif
}
#endif
