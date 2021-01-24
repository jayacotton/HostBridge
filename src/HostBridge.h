/* HostBridge header

This defines the data wrappers and entry points to get at the data buffers.


*/


extern unsigned int PutData(int, unsigned char *);	/*PutData(count,pointer) */
extern void GetData(int, unsigned char *);	/*GetData(count,pointer) */

extern void SendReady();			/*set up ready for transfer */

extern unsigned int CheckEOM();			/* check to see if an EOM has been received */
extern unsigned int CheckREADY();		/* check the ready status of both ends */
						/* 1 = local ready */
						/* 2 = remote ready */
						/* 3 = both ready */

/* system interface code, don't call this stuff */

extern void ReceiveData();			/* ISR to collect data for buffer */
extern void SendData();				/* send data to other host */
extern void HBInit();				/* init the ISR code */

extern unsigned char PIOGet();
extern void PIOPut(unsigned char);
