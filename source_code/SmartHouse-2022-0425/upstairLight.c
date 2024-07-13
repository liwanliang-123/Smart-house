#include <stdlib.h>
#include <fcntl.h>
#include "contrlDevices.h"

int upstairLightOpen(int pinNum)		//打开二楼函数
{
	digitalWrite(pinNum,LOW);
}

int upstairLightClose(int pinNum)	//关闭二楼函数
{
	digitalWrite(pinNum,HIGH);
}

int upstairLightCloseInit(int pinNum)	//初始化函数
{
	 pinMode(pinNum,OUTPUT);		//配置引脚为输出引脚
	 digitalWrite(pinNum,HIGH);		//引脚输出高电平，即默认为关闭状态
}

int upstairLightCloseStatus(int status)
{

}

struct Devices upstairLight = {		//二楼灯设备链表节点
	.deviceName = "upstairLight",
	.pinNum = 21,
	.open = upstairLightOpen,
	.close = upstairLightClose,
	.deviceInit = upstairLightCloseInit,
	.changeStatus = upstairLightCloseStatus

};

struct Devices* addUpstairLightToDeviceLink(struct Devices *phead)	//头插法将设备节点加入设备工厂链表函数
{
	if(phead == NULL){
		return &upstairLight;
	}
	else{
		upstairLight.next = phead;
		phead = &upstairLight;
	}
}


