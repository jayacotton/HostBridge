/* header for ring.c

*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define TRUE true;
#define FALSE false;

struct ring {
	unsigned char * buffer;
	int head;
	int tail;
	int size;
	int count;
};

extern int 	Ring_Size(struct ring *);		// return size of ring
extern int 	Ring_Space(struct ring *);		// return space left in ring
extern bool 	Ring_Full(struct ring *);		// true if ring full
extern bool 	Ring_Empty(struct ring *);		// true if ring empty
extern void 	Ring_Write(struct ring *,unsigned char); // write a byte to ring
extern unsigned char Ring_Read(struct ring *); 	// read a byte from ring
extern struct ring 	*Ring_Init(int );		// create a ring struct of size
extern void 	Ring_Reset(struct ring *);		// reset to empty state
extern void 	Ring_Delete(struct ring *);		// remove the ring struct 
