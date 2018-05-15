TOOLS_PATH=/opt/buildroot-gcc463/usr/bin/

#CROSS_COMPILE	=$(TOOLS_PATH)mipsel-linux-

CC =$(CROSS_COMPILE)gcc
STRIP=$(CROSS_COMPILE)strip


TAR_LIB_PATH_MIPS=/opt/buildroot-gcc463/usr/mipsel-buildroot-linux-uclibc/lib/
TAR_LIB_PATH_X86=/usr/mylib/

#TAR_LIB_PATH=$(TAR_LIB_PATH_MIPS)
TAR_LIB_PATH=$(TAR_LIB_PATH_X86)

CFLAGS = -Wall  -fpic -shared
LDFLAGS=       

#TAR = test
TAR = libsystools.so

all +=sdcard.o
all +=check_pid.o
all +=base64.o
all +=log.o
all +=systools.o
all +=files.o
all +=aes.o


export CC
$(TAR): $(all)
	$(CC) $(CFLAGS) -o $(TAR) $(all) $(LDFLAGS)
	$(STRIP) $(TAR)
	$(RM) -f *.gch *.o *.bak $(all) 
	
%.o:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $< 

.PHONY: clean
clean:
	rm -f  $(all) 
