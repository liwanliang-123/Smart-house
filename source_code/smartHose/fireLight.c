#include "contrlDevices.h"
#include <stdlib.h>

int fireIfOrNotInit(int pinNum)
{
	pinMode(pinNum,INPUT);
	digitalWrite(pinNum,HIGH);

}
int fireStatusRead(int pinNum)
{
	return digitalRead(pinNum);
}

struct Devices fireIfOrNot = {

	.pinNum = 25,
	.devicesName  = "fireIfOrNot",
	
	.devicesInit  = fireIfOrNotInit,
	.readStatus   = fireStatusRead
};

struct Devices* addfireIfOrNotToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){    
		return &fireIfOrNot;         //fireIfOrNot作为头结点
	}else{
		fireIfOrNot.next = phead;
		phead = &fireIfOrNot;
	}
}



