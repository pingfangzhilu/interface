#include <stdio.h>

#include "WavAmrCon.h"

int main(int argc, char *argv[]) 
{
	if (argc < 3) {
		fprintf(stderr, "%s in.amr out.wav\n", argv[0]);
		return 1;
	}
	AmrToWav8k(argv[1],argv[2]);
	return 0;
}

