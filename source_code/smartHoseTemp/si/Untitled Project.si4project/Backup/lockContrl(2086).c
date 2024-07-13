#include "contrlDevices.h"
#include <stdlib.h>

int lockIfOrNotInit(int pinNum)
{
	pinMode(pinNum,INPUT);
	digitalWrite(pinNum,HIGH);

}
int lockStatusRead(int pinNum)
{
	return digitalRead(pinNum);
}

struct Devices lockIfOrNot = {

	.pinNum = 28,
	.devicesName  = "lockIfOrNot",
	
	.devicesInit  = lockIfOrNotInit,
	.readStatus   = lockStatusRead
};

struct Devices* addlockIfOrNotToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){    
		return &lockIfOrNot;         //lockIfOrNot作为头结点
	}else{
		lockIfOrNot.next = phead;
		phead = &lockIfOrNot;
	}
}



