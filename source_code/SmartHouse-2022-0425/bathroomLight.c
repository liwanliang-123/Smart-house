#include "contrlDevices.h"


int bathroomLightOpen(int pinNum)	//��ԡ�ҵƺ���
{
	digitalWrite(pinNum,LOW);
}

int bathroomLightClose(int pinNum)	//�ر�ԡ�ҵƺ���
{
	digitalWrite(pinNum,HIGH);
}

int bathroomLightCloseInit(int pinNum)	//��ʼ������
{	
	 pinMode(pinNum,OUTPUT);		//��������Ϊ�������
	 digitalWrite(pinNum,HIGH);		//��������ߵ�ƽ����Ĭ��Ϊ�ر�״̬
}

int bathroomLightCloseStatus(int status)
{

}

struct Devices bathroomLight = {	//ԡ�ҵ��豸����ڵ�
	.deviceName = "bathroomLight",
	.pinNum = 22,
	.open = bathroomLightOpen,
	.close = bathroomLightClose,
	.deviceInit = bathroomLightCloseInit,
	.changeStatus = bathroomLightCloseStatus

};

struct Devices* addBathroomLightToDeviceLink(struct Devices *phead)		//ͷ�巨���豸�ڵ�����豸����������
{
	if(phead == NULL){
		return &bathroomLight;
	}
	else{
		bathroomLight.next = phead;
		phead = &bathroomLight;
	}
}

