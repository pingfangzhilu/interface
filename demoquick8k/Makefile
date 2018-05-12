BUILD_PATH=/opt/buildroot-gcc463/usr/bin/
CROSS_COMPILE=$(BUILD_PATH)mipsel-linux-
CC=$(CROSS_COMPILE)gcc


CFLAGS = -Wall -fpic -shared 
LDFLAGS= -lopencore-amrnb
TAR = libdemoquick463.so

#CFLAGS = -Wall
#LDFLAGS= -L /usr/mylib/  -lopencore-amrnb
#TAR = demoquick
#all += test.o


all +=WavtoAmr/quickWavAmr.o
all +=WavtoAmr/wavreader.o

all +=AmrtoWav/amrnb-dec.o
all +=AmrtoWav/wavwriter.o


export CC
$(TAR): $(all)
	$(CC) $(CFLAGS) -o $(TAR) $(all) $(LDFLAGS)
	$(RM) -f *.gch *.bak $(all) 
	
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $< 

.PHONY: clean
clean:
	rm -f $(TAR) $(all) 
