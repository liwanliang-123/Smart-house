#include "contrlDevices.h"
#include <stdlib.h>

int buzzOpen(int pinNum)
{
	digitalWrite(pinNum,LOW);

}
int buzzClose(int pinNum)
{
	digitalWrite(pinNum,HIGH);

}
int buzzInit(int PinNum)
{
	pinMode(PinNum,OUTPUT);
	digitalWrite(PinNum,HIGH);
}

int buzzStatus(int Ptatus)
{

}

struct Devices buzz = {

	.pinNum = 29,
	.devicesName  = "buzz",
	.open         = buzzOpen,
	.close        = buzzClose,
	
	.devicesInit  = buzzInit,
	.changeStatus = buzzStatus
};

struct Devices* addBuzzToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){    
		return &buzz;         //buzz作为头结点
	}else{
		buzz.next = phead;
		phead = &buzz;
	}
}


