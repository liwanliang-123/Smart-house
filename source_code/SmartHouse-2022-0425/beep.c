#include "contrlDevices.h"


int beepInit(int pinNum);					
int beepOpen(int pinNum);
int beepClose(int pinNum);
struct Devices *addBeepToDeviceLink(struct Devices *phead);



struct Devices beep = {			//蜂鸣器设备链表节点
	.deviceName = "beep",
	.pinNum = 29,					//树莓派gpio引脚29
	.deviceInit = beepInit,
	.open = beepOpen,
	.close = beepClose
};


int beepInit(int pinNum)					//初始化函数
{
	pinMode(pinNum,OUTPUT);					//配置引脚为输出引脚
	digitalWrite(pinNum,HIGH);				//引脚输出高电平，即默认为关闭状态
}

int beepOpen(int pinNum)					//打开蜂鸣器函数
{
	digitalWrite(pinNum,LOW);
}

int beepClose(int pinNum)					//关闭蜂鸣器函数
{
	digitalWrite(pinNum,HIGH);
}


struct Devices* addBeepToDeviceLink(struct Devices *phead)			//头插法将设备节点加入设备工厂链表函数
{
	if(phead == NULL){
		return &beep;
	}else{
		beep.next = phead;
		phead = &beep;
		return phead;
	}
}

