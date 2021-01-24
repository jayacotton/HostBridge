#include <stdlib.h>
#include <malloc.h>
#include "../HostBridge.h"

void main()
{
	HBInit();
	while(1){
		PutData(14,"This is a test");
	}
}
