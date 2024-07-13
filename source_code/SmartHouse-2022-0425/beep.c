#include "contrlDevices.h"


int beepInit(int pinNum);					
int beepOpen(int pinNum);
int beepClose(int pinNum);
struct Devices *addBeepToDeviceLink(struct Devices *phead);



struct Devices beep = {			//�������豸����ڵ�
	.deviceName = "beep",
	.pinNum = 29,					//��ݮ��gpio����29
	.deviceInit = beepInit,
	.open = beepOpen,
	.close = beepClose
};


int beepInit(int pinNum)					//��ʼ������
{
	pinMode(pinNum,OUTPUT);					//��������Ϊ�������
	digitalWrite(pinNum,HIGH);				//��������ߵ�ƽ����Ĭ��Ϊ�ر�״̬
}

int beepOpen(int pinNum)					//�򿪷���������
{
	digitalWrite(pinNum,LOW);
}

int beepClose(int pinNum)					//�رշ���������
{
	digitalWrite(pinNum,HIGH);
}


struct Devices* addBeepToDeviceLink(struct Devices *phead)			//ͷ�巨���豸�ڵ�����豸����������
{
	if(phead == NULL){
		return &beep;
	}else{
		beep.next = phead;
		phead = &beep;
		return phead;
	}
}

