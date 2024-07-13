#include "contrlDevices.h"
#include <stdlib.h>

int bathRoomLightopen(int pinNum)
{
	digitalWrite(pinNum,LOW);

}
int bathRoomLightclose(int pinNum)
{
	digitalWrite(pinNum,HIGH);

}
int bathRoomLightInit(int PinNum)
{
	pinMode(PinNum,OUTPUT);
	digitalWrite(PinNum,HIGH);
}

int bathRoomLightStatus(int Ptatus)
{

}

struct Devices bathRoomLight = {

	.pinNum = 22,
	.devicesName  = "bathRoomLight",
	.open         = bathRoomLightopen,
	.close        = bathRoomLightclose,
	
	.devicesInit  = bathRoomLightInit,
	.changeStatus = bathRoomLightStatus
};

struct Devices* addBathroomLightToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){    
		return &bathRoomLight;         //bathRoomLight作为头结点
	}else{
		bathRoomLight.next = phead;
		phead = &bathRoomLight;
	}
}


