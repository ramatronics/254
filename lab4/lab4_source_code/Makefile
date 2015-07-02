CC=gcc
CFLAGS=-Wall -g
LD=gcc
LDFLAGS=-g -lrt


OBJS1=producer.o 
OBJS2=consumer.o 

all: producer.out consumer.out

consumer.out: $(OBJS1)
	$(LD) $(LDFLAGS) -o 