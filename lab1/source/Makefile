
# ECE254/lab1/starter/ls/Makefile
# Y. Huang, 2015/05/01

CC=gcc
CFLAGS=-Wall -g
LD=gcc
LDFLAGS=-g


OBJS1=myls.o

all: myls.out

myls.out: $(OBJS1) 
	$(LD) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o *.out 
