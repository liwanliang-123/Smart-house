#include "contrlDevices.h"

int livingroomLightOpen(int pinNum)			//打开卧室函数
{
	digitalWrite(pinNum,LOW);
}

int livingroomLightClose(int pinNum)		//关闭卧室函数
{
	digitalWrite(pinNum,HIGH);
}

int livingroomLightCloseInit(int pinNum)		//初始化函数
{
	 pinMode(pinNum,OUTPUT);							//配置引脚为输出引脚
	 digitalWrite(pinNum,HIGH);							//引脚输出高电平，即默认为关闭状态
}

int livingroomLightCloseStatus(int status)
{

}

struct Devices livingroomLight = {			//卧室灯设备链表节点
	.deviceName = "livingroomLight",
	.pinNum = 24,							//树莓派引脚号
	.open = livingroomLightOpen,
	.close = livingroomLightClose,
	.deviceInit = livingroomLightCloseInit,
	.changeStatus = livingroomLightCloseStatus

};

struct Devices* addLivingroomLightToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){
		return &livingroomLight;
	}
	else{
		livingroomLight.next = phead;
		phead = &livingroomLight;
	}
}


