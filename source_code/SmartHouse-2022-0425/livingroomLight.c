#include "contrlDevices.h"

int livingroomLightOpen(int pinNum)			//�����Һ���
{
	digitalWrite(pinNum,LOW);
}

int livingroomLightClose(int pinNum)		//�ر����Һ���
{
	digitalWrite(pinNum,HIGH);
}

int livingroomLightCloseInit(int pinNum)		//��ʼ������
{
	 pinMode(pinNum,OUTPUT);							//��������Ϊ�������
	 digitalWrite(pinNum,HIGH);							//��������ߵ�ƽ����Ĭ��Ϊ�ر�״̬
}

int livingroomLightCloseStatus(int status)
{

}

struct Devices livingroomLight = {			//���ҵ��豸����ڵ�
	.deviceName = "livingroomLight",
	.pinNum = 24,							//��ݮ�����ź�
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


