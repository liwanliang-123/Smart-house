#include "inputCommand.h"

int socketInit(struct InputCommander *socketMes,char *ipAddress,char *port)
{
	int s_fd;	 
	s_fd = socket(AF_INET,SOCK_STREAM,0);//创建套接字
	if(s_fd == -1){
		perror("socket"); //打印错误信息
		exit(-1);
	}
	
	struct sockaddr_in s_addr;
	s_addr.sin_family = AF_INET;//IPv4协议
	s_addr.sin_port = htons(atoi(socketMes->port));//配置端口号，
									//htons函数为网络字节序和主机字节序的转化
	inet_aton(socketMes->ipAddress,&s_addr.sin_addr);//将IP地址配置到s_addr.sin_addr		   
	bind(s_fd,(struct sockaddr *)&s_addr,sizeof(struct sockaddr_in));//绑定

	listen(s_fd,5);
	printf("sockt Server listening .........\n");
	socketMes->sfd = s_fd;
	
	return s_fd;
}

int socketGetCommand(struct InputCommander *socketMes)
{
	int c_fd;
	int clen;
	int n_read = 0;
	struct sockaddr_in c_addr; //用来返回已经连接的客户端的协议地址
	clen = sizeof(struct sockaddr_in); //结构体长度
	c_fd = accept(socketMes->sfd,(struct sockaddr *)&c_addr,&clen); //连接客户端
	if(c_fd == -1){
		perror("accept"); //打印错误信息
	}
	
	n_read = read(c_fd,socketMes->command,sizeof(socketMes->command));//将套接字描述符c_fd中的数据读1024个到readBuf中
	if(n_read == -1){
	   perror("read"); //打印错误信息
	}else if(n_read > 0){
	   printf("get message:%s\n",socketMes->command); //将从客户端读到的数据打印
	}else{
		printf("client quit\n");
	}

	return n_read;
}

struct InputCommander socketContrl = {
	.commandName = "socketServer",
	.command     = {'\0'},
	.Init        = socketInit,
	.getCommand  = socketGetCommand,
	.log         = {'\0'},
	.sfd         = 0,
	.port        = "8801",
	.ipAddress   = "172.20.10.2", 
	.next        = NULL
};

struct InputCommander* addsocketContrlToInputCommandLink(struct InputCommander *phead)
{
	if(phead == NULL){    
		return &socketContrl;         //voiceContrl作为头结点
	}else{
		socketContrl.next = phead;
		phead = &socketContrl;
	}
}



