#include <stdlib.h>
#include <fcntl.h>
#include "contrlDevices.h"

int upstairLightOpen(int pinNum)		//�򿪶�¥����
{
	digitalWrite(pinNum,LOW);
}

int upstairLightClose(int pinNum)	//�رն�¥����
{
	digitalWrite(pinNum,HIGH);
}

int upstairLightCloseInit(int pinNum)	//��ʼ������
{
	 pinMode(pinNum,OUTPUT);		//��������Ϊ�������
	 digitalWrite(pinNum,HIGH);		//��������ߵ�ƽ����Ĭ��Ϊ�ر�״̬
}

int upstairLightCloseStatus(int status)
{

}

struct Devices upstairLight = {		//��¥���豸����ڵ�
	.deviceName = "upstairLight",
	.pinNum = 21,
	.open = upstairLightOpen,
	.close = upstairLightClose,
	.deviceInit = upstairLightCloseInit,
	.changeStatus = upstairLightCloseStatus

};

struct Devices* addUpstairLightToDeviceLink(struct Devices *phead)	//ͷ�巨���豸�ڵ�����豸����������
{
	if(phead == NULL){
		return &upstairLight;
	}
	else{
		upstairLight.next = phead;
		phead = &upstairLight;
	}
}


