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
	
	n_read = read(c_fd,socketMes->command,sizeof(socketMes->command));		//获取指令，存放在socketMes->command中
	if(n_read == -1){
		perror("read");
	}else if(n_read>0){
		printf("\nget: %d\n",n_read);  //获取到打印出获取到的字节数
	}else{
		printf("client quit\n");
	}

	return n_read;
}
*/

int socketInit(struct InputCommander *socketMes, char *ipAdress, char *port)	//套接字初始化，初始化到listen，直到有connect来
{
	int s_fd;		//套接字描述符
	
	struct sockaddr_in s_addr;

	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	
	//1.socket
	s_fd = socket(AF_INET, SOCK_STREAM, 0);		//创建套接字
	if(s_fd == -1)		//判断套接字是否创建成功
	{
		perror("socket error\n");
		exit(-1);
	}
	
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(atoi(socketMes->port));
	inet_aton(socketMes->ipAddress, &s_addr.sin_addr);
	
	//2.bind
	bind(s_fd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr_in));		//套接字与端口号绑定

	//3.listen		
	listen(s_fd, 10);		//监听
	printf("socket Server listening...\n");
	socketMes->sfd = s_fd;
	return s_fd;		//套接字描述符返回到网络控制链表节点
	
}


struct InputCommander socketContrl = {
	.commandName = "sockerServer",	
	.command = {'\0'},
	.port = "8089",						//树莓派打开待外界连接的端口号
	.ipAddress = "172.20.10.2",			//树莓派连接网络时的IP地址
	.Init = socketInit,
//	.getCommand = socketGetCommand,
	.log = {'\0'},

	.next = NULL
	
};

struct InputCommander* addSocketContrlToInputCommandLink(struct InputCommander *phead)	//头插法将设备节点加入设备工厂链表函数
{
	if(phead == NULL){
		return &socketContrl;
	}
	else{
		socketContrl.next = phead;
		phead = &socketContrl;
	}
};



