/* create a circular buffer that does not use a divide for each 
byte write/read.

01/26/2021 JAC

I realize this code looks clunky.  It was devised to avoid
using a modulus function to calculate the ring wrap condition
testing.

The goal is to put the Ring_Write function into an ISR routine
and so, excution time must be reduced 

FIFO mode
write with head pointer
read with tail pointer

we can know
size of ring
space left on the ring.
ring full
ring empty

we can
init ring
reset ring
delete ring

write byte
read byte

*/

#include "ring.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int Ring_Size(struct ring *r)
{
	return r->size;
}
int Ring_Space(struct ring *r)
{
	return (r->size-r->count);
}
bool Ring_Full(struct ring *r)
{
	if(Ring_Space(r) == 0)
		return TRUE;
	return FALSE;
}
bool Ring_Empty(struct ring *r)
{
	if(Ring_Space(r) == r->size)
		return TRUE;
	return FALSE;
}
void Ring_Write(struct ring *r,unsigned char c)
{
	r->buffer[r->head] = c;
	if (r->head >= r->size)
		r->head = 0;	
	else
		r->head++;
	r->count++;
}
unsigned char Ring_Read(struct ring *r)
{
unsigned char c;
	c = r->buffer[r->tail];
	if(r->tail >= r->size)
		r->tail = 0;
	else
		r->tail++;
	r->count--;
	return c;
}
struct ring *Ring_Init(int sz)
{
struct ring *t;
	t = calloc(1,sizeof(struct ring));
	t->buffer = calloc(1,sz);
	t->size = sz;
	Ring_Reset(t);
	return t;
}
void Ring_Reset(struct ring *r)
{
	r->head = r->tail = r->count = 0;
	memset(r->buffer,0,r->size);	
}
void Ring_Delete(struct ring *r)
{
	free(r->buffer);
	free(r);
}
