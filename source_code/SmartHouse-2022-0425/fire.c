#include "contrlDevices.h"

int fireIfOrNotInit(int pinNum)		//初始化函数
{
	 pinMode(pinNum,INPUT);					//配置引脚为输入引脚
	 digitalWrite(pinNum,HIGH);				//引脚输出高电平，即默认为关闭状态
}

int fireStatusRead(int pinNum)		//读取火焰传感器状态函数
{
	return digitalRead(pinNum);				//读取高低电平，返回0或1
}
	
struct Devices fireIfOrNot = {		//火焰传感器设备链表节点
	.deviceName = "fireIfOrNot",
	.pinNum = 25,					//树莓派引脚为25
	.deviceInit = fireIfOrNotInit,
	.readStatus = fireStatusRead

};

struct Devices* addFireToDeviceLink(struct Devices *phead)	//头插法将设备节点加入设备工厂链表函数
{
	if(phead == NULL){
		return &fireIfOrNot;
	}
	else{
		fireIfOrNot.next = phead;
		phead = &fireIfOrNot;
	}
}


