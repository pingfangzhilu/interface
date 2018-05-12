#CROSS_COMPILE	=mipsel-linux-
CC=$(CROSS_COMPILE)gcc

CFLAGS = -Wall  
LDFLAGS= -L /usr/mylib/  -lopencore-amrnb

TAR = demoquick

all +=quickWavAmr.o
all +=wavreader.o


export CC
$(TAR): $(all)
	$(CC) $(CFLAGS) -o $(TAR) $(all) $(LDFLAGS)
	$(RM) -f *.gch *.bak $(all) 
	
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $< 

.PHONY: clean
clean:
	rm -f $(TAR) $(all) 
