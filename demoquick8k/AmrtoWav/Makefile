#CROSS_COMPILE	=mipsel-linux-
CC=$(CROSS_COMPILE)gcc


#CFLAGS =-fpic -shared -Wall 
CFLAGS = -Wall

LDFLAGS= -L /usr/mylib/ -lopencore-amrnb

#TAR = libdemoAmr.so
TAR = maintest

all +=amrnb-dec.o
all +=wavwriter.o

export CC
$(TAR): $(all)
	$(CC) $(CFLAGS) -o $(TAR) $(all) $(LDFLAGS)
	$(RM) -f *.gch *.bak $(all) 
	
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $< 

.PHONY: clean
clean:
	rm -f $(TAR) $(all) 
