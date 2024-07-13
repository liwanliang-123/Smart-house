#include "contrlDevices.h"

int restaurantLightOpen(int pinNum)				//�򿪲����ƺ���
{
	digitalWrite(pinNum,LOW);
}

int restaurantLightClose(int pinNum)			//�رղ����ƺ���
{
	digitalWrite(pinNum,HIGH);
}

int restaurantLightCloseInit(int pinNum)		//��ʼ������
{
	 pinMode(pinNum,OUTPUT);							//��������Ϊ�������
	 digitalWrite(pinNum,HIGH);							//��������ߵ�ƽ����Ĭ��Ϊ�ر�״̬
}	

int restaurantLightCloseStatus(int status)
{

}

struct Devices restaurantLight = {		//�������豸����ڵ�
	.deviceName = "restaurantLight",
	.pinNum = 23,						//��ݮ������Ϊ23
	.open = restaurantLightOpen,
	.close = restaurantLightClose,
	.deviceInit = restaurantLightCloseInit,
	.changeStatus = restaurantLightCloseStatus

};

struct Devices* addRestaurantLightToDeviceLink(struct Devices *phead)	//ͷ�巨���豸�ڵ�����豸����������
{
	if(phead == NULL){
		return &restaurantLight;
	}
	else{
		restaurantLight.next = phead;
		phead = &restaurantLight;
	}
}


