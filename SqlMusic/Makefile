
#CROSS_COMPILE= /opt/buildroot-gcc463/usr/bin/mipsel-linux-

CC = $(CROSS_COMPILE)gcc
OPENSRC_DIR=/home/openSrc/src
LDFLAGS=-L $(OPENSRC_DIR)/sqlite3/sqlite-3.6.17/output/x86/lib/ -lsqlite3

CFLAGS= -lpthread   -Wall -I $(OPENSRC_DIR)/sqlite3/sqlite-3.6.17/output/x86/include/

all +=sqlite.o
all +=MusicListDb.o

TAR=sqlite

sqlite:$(all)
	$(CC)  $(all)  -o $(TAR) $(CFLAGS) $(LDFLAGS)
	rm *.o
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $< 
clean:
	rm sqlite *.o
