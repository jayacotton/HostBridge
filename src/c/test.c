#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "ring.h"

struct ring * cbuf;

extern void msg_write(unsigned char *);
extern void msg_read (unsigned char *);
extern int wb;
extern int link;
extern struct ring * r;
extern char c;
void main()
{
unsigned char i,j;
unsigned char buff[32];

	cbuf = Ring_Init(32);
	r = cbuf; // salt the ring pointer for ISR
	SetInt(wb,4);	
	SetPIO();
	if(r == 0){
		printf("Error at Ring_Init\n");
		return;
	}
	i = 0;
	for(i=0;i<255;i++){
		msg_read(buff);
		printf("%s\n",buff);
	}		
	ClearInt();
}
void msg_read(unsigned char *bu)
{
	while( *bu = Ring_Read(cbuf))
		bu++;
	
}
