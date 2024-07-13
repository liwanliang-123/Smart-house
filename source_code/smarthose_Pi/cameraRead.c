#include "contrlDevices.h"
#include <stdlib.h>

int camerReadInit(int pinNum)
{
	pinMode(pinNum,INPUT);
	digitalWrite(pinNum,HIGH);

}
int camerReadStatus(int pinNum)
{
	return digitalRead(pinNum);
}

struct Devices camerIfNotRead = {

	.pinNum = 27,
	.devicesName  = "camerRead",
	
	.devicesInit  = camerReadInit,
	.readStatus   = camerReadStatus
};

struct Devices* addcamerReadToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){    
		return &camerIfNotRead;         //camerIfNotRead作为头结点
	}else{
		camerIfNotRead.next = phead;
		phead = &camerIfNotRead;
	}
}



