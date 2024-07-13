#include "contrlDevices.h"

int restaurantLightOpen(int pinNum)				//打开餐厅灯函数
{
	digitalWrite(pinNum,LOW);
}

int restaurantLightClose(int pinNum)			//关闭餐厅灯函数
{
	digitalWrite(pinNum,HIGH);
}

int restaurantLightCloseInit(int pinNum)		//初始化函数
{
	 pinMode(pinNum,OUTPUT);							//配置引脚为输出引脚
	 digitalWrite(pinNum,HIGH);							//引脚输出高电平，即默认为关闭状态
}	

int restaurantLightCloseStatus(int status)
{

}

struct Devices restaurantLight = {		//餐厅灯设备链表节点
	.deviceName = "restaurantLight",
	.pinNum = 23,						//树莓派引脚为23
	.open = restaurantLightOpen,
	.close = restaurantLightClose,
	.deviceInit = restaurantLightCloseInit,
	.changeStatus = restaurantLightCloseStatus

};

struct Devices* addRestaurantLightToDeviceLink(struct Devices *phead)	//头插法将设备节点加入设备工厂链表函数
{
	if(phead == NULL){
		return &restaurantLight;
	}
	else{
		restaurantLight.next = phead;
		phead = &restaurantLight;
	}
}


