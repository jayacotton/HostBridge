CFLAGS = --list --c-code-in-asm -Wall -clib=new -m

all: test

test: test.c ring.c HBridge.c sysface.c
	zcc +cpm -create-app $(CFLAGS) -otest test.c ring.c ../asm/isr.o sysface.c

