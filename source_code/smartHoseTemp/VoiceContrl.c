#include "inputCommand.h"


int voiceInit(struct InputCommander *voice,char *ipAddress,char *port)
{
	int fd;
	if((fd = serialOpen(voice->DevicesName,voice->baudRate)) == -1){

		exit(-1);
	}
	voice->fd = fd;
	
	return fd;
}

int voiceGetCommand(struct InputCommander *voice)
{
	int nread = 0;
	memset(voice->command,'\0',sizeof(voice->command));
	nread = read(voice->fd,voice->command,sizeof(voice->command));
/*	
	if(nread == 0){
		printf("usart for voice read over time\n");
	}else{    
		return nread;
	}*/
	return nread;	
}

struct InputCommander voiceContrl = {
	.commandName = "voice",
	.command     = {'\0'},
	.Init        = voiceInit,
	.getCommand  = voiceGetCommand,
	.log         = {'\0'},
	.DevicesName = "/dev/ttyAMA0",
	.baudRate    = 9600,
	.fd          = 0,
	.next        = NULL
};

struct InputCommander* addvoiceContrlToInputCommandLink(struct InputCommander *phead)
{
	if(phead == NULL){    
		return &voiceContrl;         //voiceContrl作为头结点
	}else{
		voiceContrl.next = phead;
		phead = &voiceContrl;
	}


}


