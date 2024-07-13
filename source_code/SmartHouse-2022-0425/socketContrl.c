#include<wiringPi.h>
#include<wiringSerial.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<stdlib.h>
#include <string.h>

#include"InputCommand.h"


/*int socketGetCommand(struct InputCommander*socketMes)		
{
	int c_fd;
	int n_read = 0;
	struct sockaddr_in c_addr;
	memset(&c_addr, 0, sizeof(struct sockaddr_in));
	int clen = sizeof(struct sockaddr_in);
	
	c_fd = accept(socketMes->sfd, (struct sockaddr *)&c_addr, &clen);
	
	n_read = read(c_fd,socketMes->command,sizeof(socketMes->command));		//��ȡָ������socketMes->command��
	if(n_read == -1){
		perror("read");
	}else if(n_read>0){
		printf("\nget: %d\n",n_read);  //��ȡ����ӡ����ȡ�����ֽ���
	}else{
		printf("client quit\n");
	}

	return n_read;
}
*/

int socketInit(struct InputCommander *socketMes, char *ipAdress, char *port)	//�׽��ֳ�ʼ������ʼ����listen��ֱ����connect��
{
	int s_fd;		//�׽���������
	
	struct sockaddr_in s_addr;

	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	
	//1.socket
	s_fd = socket(AF_INET, SOCK_STREAM, 0);		//�����׽���
	if(s_fd == -1)		//�ж��׽����Ƿ񴴽��ɹ�
	{
		perror("socket error\n");
		exit(-1);
	}
	
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(socketMes->port));
	inet_aton(socketMes->ipAddress, &s_addr.sin_addr);
	
	//2.bind
	bind(s_fd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr_in));		//�׽�����˿ںŰ�

	//3.listen		
	listen(s_fd, 10);		//����
	printf("socket Server listening...\n");
	socketMes->sfd = s_fd;
	return s_fd;		//�׽������������ص������������ڵ�
	
}


struct InputCommander socketContrl = {
	.commandName = "sockerServer",	
	.command = {'\0'},
	.port = "8089",						//��ݮ�ɴ򿪴�������ӵĶ˿ں�
	.ipAddress = "172.20.10.2",			//��ݮ����������ʱ��IP��ַ
	.Init = socketInit,
//	.getCommand = socketGetCommand,
	.log = {'\0'},

	.next = NULL
	
};

struct InputCommander* addSocketContrlToInputCommandLink(struct InputCommander *phead)	//ͷ�巨���豸�ڵ�����豸����������
{
	if(phead == NULL){
		return &socketContrl;
	}
	else{
		socketContrl.next = phead;
		phead = &socketContrl;
	}
};



