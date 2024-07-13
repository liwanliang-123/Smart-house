#include<wiringPi.h>
#include<wiringSerial.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
#include"InputCommand.h"

int voiceGetCommand(struct InputCommander*voicer)		//��ȡָ���
{
	int nread = 0;
	memset(voicer->command,'\0', sizeof(voicer->command));
	
	nread = read(voicer->fd,voicer->command,sizeof(voicer->command));  //�����ڵ�ָ�����voicer->command�У��������߳��н��з���

	return nread;
}


int voiceInit(struct InputCommander *voicer, char *ipAdress, char *port)
{
	int fd;
	
	if((fd = serialOpen(voicer->deviceName,9600))==-1) 	//����ݮ�ɴ��ڣ�������Ϊ9600
	{
		exit(-1);
	}
	voicer->fd = fd;	//�򿪴����ļ��ɹ������ء��ļ��������������������ơ�����ڵ���

	return fd;
}


struct InputCommander voiceContrl = {	//������������ڵ�
	.commandName = "voice",
	.deviceName = "/dev/ttyAMA0",		//����
	.command = {'\0'},
	.Init = voiceInit,
	.getCommand = voiceGetCommand,
	.log = {'\0'},

	.next = NULL
	
};

struct InputCommander* addvoiceContrlToInputCommandLink(struct InputCommander *phead)//ͷ�巨�����������ơ�����ڵ����ָ����ƹ���������
{
	if(phead == NULL){
		return &voiceContrl;
	}
	else{
		voiceContrl.next = phead;
		phead = &voiceContrl;
	}
};


