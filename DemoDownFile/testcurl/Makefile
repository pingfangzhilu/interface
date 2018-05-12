BUILD_PATH=/opt/buildroot-gcc463/usr/bin/
CROSS_COMPILE=$(BUILD_PATH)mipsel-linux-
CC=$(CROSS_COMPILE)gcc


XML_INC = -I /home/yue/work0615/demolib/libxml/libxml2-2.9.4/output_lib/mini_x86/include/libxml2/
XML_LIB_X86= -L /home/yue/work0615/demolib/libxml/libxml2-2.9.4/output_lib/x86/lib/ -lxml2

OPEN_SSL_INC=-I /home/yue/work0615/demolib/openssl_x86/openssl-1.0.1t/output/x86/include/
CURL_INC=-I /home/yue/work0615/demolib/libcurl/curl-7.50.1/output/x86/include/

MY_LIB_X86 = -L /usr/mylib/ -ldownFile  -DX86
MY_LIB_MIPS = -ldownFile463  -DMIPS_7688

SDK_INC=-I $(SDK_PATH)/lib/libnvram/ 

CFLAGS = -Wall -I ./include/ $(OPEN_SSL_INC) $(CURL_INC)  $(XML_INC) $(SDK_INC) -I $(KERNEL_PATH)/include/
LDFLAGS_X86= -lrt  -L /home/yue/work0615/demolib/libcurl/curl-7.50.1/output/x86/lib/ -lcurl  -L /home/yue/work0615/demolib/openssl_x86/openssl-1.0.1t/output/x86/lib/ -lssl -lcrypto $(XML_LIB_X86) $(MY_LIB_X86)     
LDFLAGS_MIPS= -ldl -lrt -lz -lcurl -lssl -lcrypto -lxml2  $(MY_LIB_MIPS) 

LDFLAGS=$(LDFLAGS_MIPS)
#LDFLAGS=$(LDFLAGS_X86)

TAR = down

all +=test.o

export CC
$(TAR): $(all)
	$(CC) $(CFLAGS) -o $(TAR) $(all) -lpthread $(LDFLAGS)
#	cp $(TAR) $(SDK_PATH)/romfs/bin/
#	cp $(TAR) /nfs/tang/
	$(RM) -f *.gch *.bak $(all) 
	
%.o:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $< 

.PHONY: clean
clean:
	rm -f $(TAR) $(all) 
