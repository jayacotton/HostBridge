#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "ring.h"

struct ring * cbuf;

extern void msg_write(unsigned char *);
extern void msg_read (unsigned char *);

void main()
{
unsigned char i,j;
unsigned char buff[32];

	cbuf = Ring_Init(32);
	i = 0;
	while(1){
		msg_write("this is a test");
		msg_read(buff);
		printf("%s\n",buff);
	}		
}
void msg_write(unsigned char *t)
{
	while(*t){
		Ring_Write(cbuf,*t);
		t++;
	}
	Ring_Write(cbuf,'\0');
}
void msg_read(unsigned char *bu)
{
	while( *bu = Ring_Read(cbuf))
		bu++;
	
}
