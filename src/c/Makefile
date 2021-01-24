CFLAGS = --list --c-code-in-asm -Wall -clib=new

all: test

test: test.c circular.c HBridge.c
	zcc +cpm -create-app $(CFLAGS) -otest test.c circular.c HBridge.c
