TOOLS_PATH=/opt/buildroot-gcc463/usr/bin/

#CROSS_COMPILE	=$(TOOLS_PATH)mipsel-linux-

CC =$(CROSS_COMPILE)gcc
STRIP=$(CROSS_COMPILE)strip

OPENSRC_DIR=/home/openSrc/src

OPEN_INC=-I $(OPENSRC_DIR)/libcurl/curl-7.50.1/output/mips/include/
OPEN_LIB = -L $(OPENSRC_DIR)/libcurl/curl-7.50.1/output/x86/lib/ -lcurl	
#OPEN_LIB=  -lz -lcurl

CFLAGS = -Wall -I ./include $(OPEN_INC) 
#CFLAGS = -Wall -I ./include $(OPEN_INC) -fpic -shared
LDFLAGS=  $(OPEN_LIB) -lpthread     

TAR = test
#TAR = libdownFile463.so
#TAR = libdownFile.so

all +=curldown.o

export CC
$(TAR): $(all)
	$(CC) $(CFLAGS) -o $(TAR) $(all) $(LDFLAGS)
	$(STRIP) $(TAR)
	$(RM) -f *.gch *.o *.bak $(all) 
	
%.o:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $< 

.PHONY: clean
clean:
	rm -f $(TAR) $(all) 
