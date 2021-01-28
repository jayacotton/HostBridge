#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "ring.h"

struct ring * cbuf;

extern void msg_write(unsigned char *);
extern void msg_read (unsigned char *);
extern void wb();
extern struct ring * r;
extern char c;
void main()
{
unsigned char i,j;
unsigned char buff[32];

	cbuf = Ring_Init(32);
	r = cbuf; // salt the ring pointer for ISR
	if(r == 0){
		printf("Error at Ring_Init\n");
		return;
	}
	i = 0;
//	while(1){
	for(i=0;i<255;i++){
		msg_write("this is a test");
		msg_read(buff);
		printf("%s\n",buff);
	}		
	printf("Space left on ring %d\n",Ring_Space(cbuf));
}
void msg_write(unsigned char *t)
{
	while(*t){
		c = *t;	// read a byte (from PIO)
		wb();	// test asm version of Ring_Write
		t++;
	}
	c = '\0';
	wb();
}
void msg_read(unsigned char *bu)
{
	while( *bu = Ring_Read(cbuf))
		bu++;
	
}
