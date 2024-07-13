#include "contrlDevices.h"


int bathroomLightOpen(int pinNum)	//打开浴室灯函数
{
	digitalWrite(pinNum,LOW);
}

int bathroomLightClose(int pinNum)	//关闭浴室灯函数
{
	digitalWrite(pinNum,HIGH);
}

int bathroomLightCloseInit(int pinNum)	//初始化函数
{	
	 pinMode(pinNum,OUTPUT);		//配置引脚为输出引脚
	 digitalWrite(pinNum,HIGH);		//引脚输出高电平，即默认为关闭状态
}

int bathroomLightCloseStatus(int status)
{

}

struct Devices bathroomLight = {	//浴室灯设备链表节点
	.deviceName = "bathroomLight",
	.pinNum = 22,
	.open = bathroomLightOpen,
	.close = bathroomLightClose,
	.deviceInit = bathroomLightCloseInit,
	.changeStatus = bathroomLightCloseStatus

};

struct Devices* addBathroomLightToDeviceLink(struct Devices *phead)		//头插法将设备节点加入设备工厂链表函数
{
	if(phead == NULL){
		return &bathroomLight;
	}
	else{
		bathroomLight.next = phead;
		phead = &bathroomLight;
	}
}

