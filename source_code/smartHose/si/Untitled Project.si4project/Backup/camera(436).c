#include "contrlDevices.h"
#include <stdlib.h>

int cameraopen(int pinNum)
{
	digitalWrite(pinNum,LOW);

}
int cameraclose(int pinNum)
{
	digitalWrite(pinNum,HIGH);

}
int cameraInit(int PinNum)
{
	pinMode(PinNum,OUTPUT);
	digitalWrite(PinNum,HIGH);
}

int cameraRead(int pinNum)
{
	return digitalRead(pinNum);
}

struct Devices camera = {

	.pinNum = 22,
	.devicesName  = "camera",
	.open         = cameraopen,
	.close        = cameraclose,
	
	.devicesInit  = cameraInit,
	.readStatus   = cameraRead
};

struct Devices* addcameraToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){    
		return &camera;         //camera作为头结点
	}else{
		camera.next = phead;
		phead = &camera;
	}
}
