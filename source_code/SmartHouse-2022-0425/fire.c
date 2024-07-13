#include "contrlDevices.h"

int fireIfOrNotInit(int pinNum)		//��ʼ������
{
	 pinMode(pinNum,INPUT);					//��������Ϊ��������
	 digitalWrite(pinNum,HIGH);				//��������ߵ�ƽ����Ĭ��Ϊ�ر�״̬
}

int fireStatusRead(int pinNum)		//��ȡ���洫����״̬����
{
	return digitalRead(pinNum);				//��ȡ�ߵ͵�ƽ������0��1
}
	
struct Devices fireIfOrNot = {		//���洫�����豸����ڵ�
	.deviceName = "fireIfOrNot",
	.pinNum = 25,					//��ݮ������Ϊ25
	.deviceInit = fireIfOrNotInit,
	.readStatus = fireStatusRead

};

struct Devices* addFireToDeviceLink(struct Devices *phead)	//ͷ�巨���豸�ڵ�����豸����������
{
	if(phead == NULL){
		return &fireIfOrNot;
	}
	else{
		fireIfOrNot.next = phead;
		phead = &fireIfOrNot;
	}
}


