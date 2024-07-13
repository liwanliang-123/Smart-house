#include "contrlDevices.h"
#include <stdlib.h>

int upstairLightopen(int pinNum)
{
	digitalWrite(pinNum,LOW);

}
int upstairLightclose(int pinNum)
{
	digitalWrite(pinNum,HIGH);

}
int upstairLightInit(int PinNum)
{
	pinMode(PinNum,OUTPUT);
	digitalWrite(PinNum,HIGH);
}

int upstairLightStatus(int Ptatus)
{

}

struct Devices upstairLight = {

	.pinNum = 21,
	.devicesName  = "upstairLight",
	.open         = upstairLightopen,
	.close        = upstairLightclose,
	
	.devicesInit  = upstairLightInit,
	.changeStatus = upstairLightStatus
};

struct Devices* addupstairLightToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){    
		return &upstairLight;         //upstairLight作为头结点
	}else{
		upstairLight.next = phead;
		phead = &upstairLight;
	}
}


