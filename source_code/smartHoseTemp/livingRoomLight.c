#include "contrlDevices.h"
#include <stdlib.h>

int livingRoomLightopen(int pinNum)
{
	digitalWrite(pinNum,LOW);

}
int livingRoomLightclose(int pinNum)
{
	digitalWrite(pinNum,HIGH);

}
int livingRoomLightInit(int PinNum)
{
	pinMode(PinNum,OUTPUT);
	digitalWrite(PinNum,HIGH);
}

int livingRoomLightStatus(int Ptatus)
{

}

struct Devices livingRoomLight = {

	.pinNum = 23,
	.devicesName  = "livingRoomLight",
	.open         = livingRoomLightopen,
	.close        = livingRoomLightclose,
	
	.devicesInit  = livingRoomLightInit,
	.changeStatus = livingRoomLightStatus
};

struct Devices* addlivingroomLightToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){    
		return &livingRoomLight;         //livingRoomLight作为头结点
	}else{
		livingRoomLight.next = phead;
		phead = &livingRoomLight;
	}
}



