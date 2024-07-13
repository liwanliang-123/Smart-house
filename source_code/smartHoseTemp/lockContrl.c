#include "contrlDevices.h"
#include <stdlib.h>

int lockOpen(int pinNum)
{
	digitalWrite(pinNum,LOW);

}

int lockClose(int pinNum)
{
	digitalWrite(pinNum,HIGH);

}

int lockIfOrNotInit(int pinNum)
{
	pinMode(pinNum,OUTPUT);
	digitalWrite(pinNum,LOW);

}

struct Devices lockIfOrNot = {

	.pinNum = 28,
	.devicesName  = "lockIfOrNot",
	.devicesInit  = lockIfOrNotInit,
	.open         = lockOpen,
	.close        = lockClose
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


