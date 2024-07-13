#include "contrlDevices.h"
#include <stdlib.h>

int RestaurantLightopen(int pinNum)
{
	digitalWrite(pinNum,LOW);

}
int RestaurantLightclose(int pinNum)
{
	digitalWrite(pinNum,HIGH);

}
int RestaurantLightInit(int PinNum)
{
	pinMode(PinNum,OUTPUT);
	digitalWrite(PinNum,HIGH);
}

int RestaurantLightStatus(int Ptatus)
{

}

struct Devices RestaurantLight = {

	.pinNum = 24,
	.devicesName  = "RestaurantLight",
	.open         = RestaurantLightopen,
	.close        = RestaurantLightclose,
	
	.devicesInit  = RestaurantLightInit,
	.changeStatus = RestaurantLightStatus
};

struct Devices* addRestaurantLightToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){    
		return &RestaurantLight;         //RestaurantLight作为头结点
	}else{
		RestaurantLight.next = phead;
		phead = &RestaurantLight;
	}
}



