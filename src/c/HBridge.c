/* Host Bridge code 

No need to worry about what the host is, there is only one on the buss with
us.  Possible to multiple hosts, but we only address 1 at a time, selection
will have to happen upstairs.

*/

#include "../HostBridge.h"
#include "circular.h"

#define EOT 4 
#define NULL 0
extern int HBInited;
extern cbuf_handle_t HBBuf;
extern void MakeReady();

unsigned int PutData(int count, unsigned char *buffer)
{
	if(CheckREADY()&2){
		SendData(count, buffer);	
		return count;
	}
	return 0;
}

void GetData(int count, unsigned char *buffer)
{
unsigned char c;

	while(count--){
/* int off */
		circular_buf_get(HBBuf, &c);
/* int on */
		if(c == NULL) {
			MakeReady();
			return;
		}
		*buffer++ = c;
	}
	MakeReady();
}

extern unsigned char HBREADY;
extern unsigned char HBEOT;
extern unsigned char CheckReady();

void MakeReady()
{
/* int off */
	HBREADY++;
/* int on */
}

unsigned int CheckEOT()
{
	return HBEOT;
}

unsigned int CheckREADY()
{
	return HBREADY | CheckReady();
}

/* ISR code */

/* set up the ISR code for ReceiveData.

Note:  this is currently RomWBW specific code.
*/

/* this is the interface buffer, NO TOUCHY */
#define HB_BUF_SIZE  128		/* cpm sector size */
unsigned char HB_buf[HB_BUF_SIZE +10];
/* this is the buffer handle, use by send and receive */
cbuf_handle_t HBBuf;
unsigned char HBInited = 0;
unsigned char HBEOT = 0;
unsigned char HBREADY = 0;
unsigned char HBState = 0;
unsigned char OHBREADY = 0;
unsigned char HBStat[3] = {0,0,0};

void HBSetUpISR()
{/* set up the ISR code for receiving the data, sending is TBD */
}
void HBInit()
{
	circular_buf_init(HBBuf,HB_BUF_SIZE);
	HBSetUpISR();
	HBInited = 1;
	HBREADY = 1;
	HBEOT = 0;
	if(CheckReady() & 2) OHBREADY++;
}

/* This is not really an ISR routine, but it could
be, if needed */

void SendData(int count, unsigned char *buffer)
{
	if(!HBInited){
		HBInit();
	}else{
// Wait here for the other host to set the ready flag.
		while(OHBREADY == 0){}	/* TODO:  put in a timer here */

// send data bytes to the PIO port (Z80PIO in this case)

		while(count--) PIOPut(*buffer++);
		PIOPut(EOT);
	}
}

unsigned char CheckReady()
{
	if(!HBInited)
		HBInit();
	HBStat[1] = HBREADY;
	HBStat[2] = EOT;
	if(HBEOT) HBEOT = 0;	/* going to loose the current message */
	SendData(3,HBStat);
	while(!HBEOT){}		/*TODO: Add a timer here wait to receive the status from other host */
	return (OHBREADY << 1);
}

/* This is an ISR routine, it is highly RomWBW
specific code at this time.

Note:  you can't get here until HBInit is called, so you gotta do it sometime.
Also, SendData will init if needed. 

*/

void ReceiveData()
{unsigned char c;
/* int off */
	c = PIOGet();
	circular_buf_put(HBBuf,c);
/* int on */
	if(c == EOT){
		HBEOT++;	/* got EOT */
		HBREADY = 0;	/* not ready now */
		circular_buf_get(HBBuf,&c);	/* get a command */
		switch(c){
			case 0:		/* process a status report */
				HBState = 0;	
				circular_buf_get(HBBuf,&c);
				OHBREADY = c;
				circular_buf_get(HBBuf,&c);
				break;	
			case 1:
				HBState = 1;	
				circular_buf_get(HBBuf,&c);
				circular_buf_get(HBBuf,&c);
				break;
			case 2:
				HBState = 1;	
				circular_buf_get(HBBuf,&c);
				circular_buf_get(HBBuf,&c);
				break;
			default:
				break;
		}
	}
}
void PIOPut(unsigned char c)
{
}
unsigned char PIOGet()
{
	return NULL;
}
