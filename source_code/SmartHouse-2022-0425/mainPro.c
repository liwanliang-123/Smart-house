#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <wiringSerial.h>

#include "contrlDevices.h"
#include "InputCommand.h"

#define SWITCH1 21		//四盏灯对应的引脚
#define SWITCH2 22
#define SWITCH3 23
#define SWITCH4 24
#define SWITCH5 25		//火焰传感器对应的引脚

struct InputCommander *pCommandHead = NULL;		//指令控制工厂链表节点头
struct Devices		  *pdeviceHead = NULL;		//设备工厂链表头节点
struct InputCommander *socketHandler = NULL;	//网络控制线程执行的函数使用到的全局变量，在read_thread线程和socket_thread都使用到了

int c_fd;


struct Devices* findDeviceByName(char *name,struct Devices *phead)		//根据名字寻找设备工厂链表链节函数，并返回链节
{
	struct Devices *tmp = phead;
	if(phead == NULL){
		return NULL;
	}else{
		while(tmp != NULL){
			if(strcmp(tmp->deviceName,name) == 0){
				return tmp;
			}
			tmp = tmp->next;
		}
		return NULL;
	}
}

struct InputCommander* findCommandByName(char *name,struct InputCommander *phead)		//根据名字寻找指令控制工厂链表链节函数，并返回链节
{
	struct InputCommander *tmp = phead;
	if(phead == NULL){
		return NULL;
	}else{
		while(tmp != NULL){
			if(strcmp(tmp->commandName,name) == 0){
				return tmp;
			}
			tmp = tmp->next;
		}
		return NULL;
	}
}

void *fireAlarmThread(void *data)				//“火灾报警器线程”执行的函数
{
	int status;
	struct Devices *firetmp = NULL;
	struct Devices *buztmp = NULL;

	firetmp = findDeviceByName("fireIfOrNot",pdeviceHead);		//寻找“火焰传感器”链表节点，返回给firetmp
	buztmp = findDeviceByName("beep",pdeviceHead);				//寻找“蜂鸣器”链表节点，返回给buztmp

	while(1){
		status = firetmp->readStatus(firetmp->pinNum);			//读取“火焰传感器”状态
	
		if(status == 0){						//检测到火焰或强光源
			buztmp->open(buztmp->pinNum);		//打开蜂鸣器
			delay(1000);						//延时1000毫秒=1秒
		}

		if(status == 1){						//未检测到火焰、强光源或解除警报
			buztmp->close(buztmp->pinNum);		//关闭蜂鸣器
		}
	}
}

void * voice_thread(void* datas)		//语音线程
{
	struct InputCommander *voiceHandler;	
	struct Devices        *tmp = NULL;

	int nread;
	voiceHandler = findCommandByName("voice",pCommandHead);		//找到语音节点

	if(voiceHandler == NULL){		//防止段错误
		printf("find voiceHandler error\n");
		pthread_exit(NULL);	//线程里不用return

	}else{
		if(voiceHandler->Init(voiceHandler,NULL,NULL)<0){	//判断voice的fd是否正确
			printf("voice init error\n");
			pthread_exit(NULL);
		}else{
			printf("%s init success\n",voiceHandler->commandName);
		}
		while(1){
			nread = voiceHandler->getCommand(voiceHandler);	//指令读在voiceHandler->command里面
			if(nread==0){
				printf("waiting....\n");
			}else{
				printf("do divece contrl:%s\n",voiceHandler->command);
				if(strstr(voiceHandler->command,"WZ") != NULL){		//一级指令，

					printf("shoudaoXP\n");	
				}else if(strstr(voiceHandler->command,"KYS") != NULL){		//对串口收到的指令进行分析，并执行对应的操作
					digitalWrite(SWITCH1,LOW);
				}else if(strstr(voiceHandler->command,"GYS") != NULL){
					digitalWrite(SWITCH1,HIGH);
				}else if(strstr(voiceHandler->command,"KCT") != NULL){
					digitalWrite(SWITCH2,LOW);
				}else if(strstr(voiceHandler->command,"GCT") != NULL){
					digitalWrite(SWITCH2,HIGH);
				}else if(strstr(voiceHandler->command,"KEL") != NULL){
					digitalWrite(SWITCH3,LOW);
				}else if(strstr(voiceHandler->command,"GEL") != NULL){
					digitalWrite(SWITCH3,HIGH);
				}else if(strstr(voiceHandler->command,"KWS") != NULL){
					digitalWrite(SWITCH4,LOW);
				}else if(strstr(voiceHandler->command,"GWS") != NULL){
					digitalWrite(SWITCH4,HIGH);
				}
			}
			memset(voiceHandler->command,'\0',sizeof(voiceHandler->command));
		}
	}

}

void *cameraThread_func(void* data)//起线程的函数有格式要求
{
	struct Devices *cameraTemp;

	cameraTemp = findDeviceByName("camera", pdeviceHead); 	//设备都要从工厂里面取出来

	if(cameraTemp == NULL){  //防止段错误的必需判断，当给指针赋值是，一定要考虑NULL的情况，否则后续操作都是空谈
		printf("find camera error\n");
		pthread_exit(NULL); //在线程中不用return
	}

	cameraTemp->justDoOnce(); //调用postUrl函数
}


void * read_thread(void* datas)
{
	int n_read;
	while(1){
		memset(socketHandler->command,'\0', sizeof(socketHandler->command));
		n_read = read(c_fd,socketHandler->command,sizeof(socketHandler->command));	//从套接字中读取指令放在socketHandler->command中
		if(n_read == -1){
			perror("read");
		}else if(n_read>0){
			printf("\nget: %d,%s\n",n_read,socketHandler->command);	//打印出指令字节数和指令
			if(strstr(socketHandler->command,"KYS") != NULL){		//对socket收到的指令进行分析，并执行对应的操作
				digitalWrite(SWITCH1,LOW);
			}
			if(strstr(socketHandler->command,"GYS") != NULL){
				digitalWrite(SWITCH1,HIGH);
			}
			if(strstr(socketHandler->command,"KCT") != NULL){
				digitalWrite(SWITCH2,LOW);
			}
			if(strstr(socketHandler->command,"GCT") != NULL){
				digitalWrite(SWITCH2,HIGH);
			}
			if(strstr(socketHandler->command,"KEL") != NULL){
				digitalWrite(SWITCH3,LOW);
			}
			if(strstr(socketHandler->command,"GEL") != NULL){
				digitalWrite(SWITCH3,HIGH);
			}
			if(strstr(socketHandler->command,"KWS") != NULL){
				digitalWrite(SWITCH4,LOW);
			}
			if(strstr(socketHandler->command,"GWS") != NULL){
				digitalWrite(SWITCH4,HIGH);
			}
			if(0 == strcmp(socketHandler->command,"OPCA")){		//当指令是OPCA时创建摄像头线程，完成拍照，人脸识别
				pthread_t cameraThread;
				pthread_create(&cameraThread,NULL,cameraThread_func,NULL);
				//pthread_join(cameraThread, NULL);
			}
		}
	}
}

void * socket_thread(void* datas)
{
	int n_read = 0;
	pthread_t readThread;
	struct sockaddr_in c_addr;
	memset(&c_addr, 0, sizeof(struct sockaddr_in));
	int clen = sizeof(struct sockaddr_in);

	socketHandler = findCommandByName("sockerServer",pCommandHead);

	if(socketHandler == NULL){	//防止段错误
		printf("find socketHandler error\n");
		pthread_exit(NULL);		//在线程中不用return
	}else{
		printf("%s init success\n",socketHandler->commandName);
	}
	socketHandler->Init(socketHandler,NULL,NULL);		//找到之后执行socketInit函数

	while(1){
		c_fd = accept(socketHandler->sfd, (struct sockaddr *)&c_addr, &clen);	//不断连接connect，连接到后创建readThread线程
		pthread_create(&readThread,NULL,read_thread,NULL);
	}
}

int main()
{
	char name[128];
	struct Devices *tmp = NULL;
	pthread_t voiceThread;
	pthread_t socketThread;
	pthread_t fireAlarm_thread;

	if(wiringPiSetup() == -1){
		return -1;
	}

	//1.指令工厂初始化
	pCommandHead = addvoiceContrlToInputCommandLink(pCommandHead);
	pCommandHead = addSocketContrlToInputCommandLink(pCommandHead);

	//2.设备控制工厂初始化
	pdeviceHead = addBathroomLightToDeviceLink(pdeviceHead);
	pdeviceHead = addUpstairLightToDeviceLink(pdeviceHead);
	pdeviceHead = addRestaurantLightToDeviceLink(pdeviceHead);
	pdeviceHead = addLivingroomLightToDeviceLink(pdeviceHead);
	pdeviceHead = addcameraToDeviceLink(pdeviceHead);
	pdeviceHead = addFireToDeviceLink(pdeviceHead);
	pdeviceHead = addBeepToDeviceLink(pdeviceHead);

	struct Devices *tmpDeviceshead = pdeviceHead;
	while(tmpDeviceshead != NULL){						//设备工厂所有设备初始化
		tmpDeviceshead->deviceInit(tmpDeviceshead->pinNum);
		tmpDeviceshead = tmpDeviceshead->next;
	}
	
	
	//3.线程池建立
	//int pthread_create(pthread_t *restrict tidp, const pthread_attr_t *restrict attr, void *(*start_rtn)(void *), void *restrict arg);
	//3.1语音线程
	pthread_create(&voiceThread,NULL,voice_thread,NULL);

	//3.2socket线程
	pthread_create(&socketThread,NULL,socket_thread,NULL);

	//3.3摄像头线程

	//3.4火灾线程
	pthread_create(&fireAlarm_thread,NULL,fireAlarmThread,NULL);
	printf("camera init success\n");

	pthread_join(voiceThread,NULL);		//等待语音线程调用pthread_exit函数
	pthread_join(socketThread,NULL);	//等待socket线程调用pthread_exit函数
	pthread_join(fireAlarm_thread, NULL);	//等待火灾线程调用pthread_exit函数


	return 0;
}

