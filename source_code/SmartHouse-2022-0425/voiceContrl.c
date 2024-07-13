#include<wiringPi.h>
#include<wiringSerial.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
#include"InputCommand.h"

int voiceGetCommand(struct InputCommander*voicer)		//获取指令函数
{
	int nread = 0;
	memset(voicer->command,'\0', sizeof(voicer->command));
	
	nread = read(voicer->fd,voicer->command,sizeof(voicer->command));  //将串口的指令读到voicer->command中，在语言线程中进行分析

	return nread;
}


int voiceInit(struct InputCommander *voicer, char *ipAdress, char *port)
{
	int fd;
	
	if((fd = serialOpen(voicer->deviceName,9600))==-1) 	//打开树莓派串口，波特率为9600
	{
		exit(-1);
	}
	voicer->fd = fd;	//打开串口文件成功，返回“文件描述符”到“语音控制”链表节点中

	return fd;
}


struct InputCommander voiceContrl = {	//语音控制链表节点
	.commandName = "voice",
	.deviceName = "/dev/ttyAMA0",		//串口
	.command = {'\0'},
	.Init = voiceInit,
	.getCommand = voiceGetCommand,
	.log = {'\0'},

	.next = NULL
	
};

struct InputCommander* addvoiceContrlToInputCommandLink(struct InputCommander *phead)//头插法将“语音控制”链表节点加入指令控制工厂链表函数
{
	if(phead == NULL){
		return &voiceContrl;
	}
	else{
		voiceContrl.next = phead;
		phead = &voiceContrl;
	}
};


