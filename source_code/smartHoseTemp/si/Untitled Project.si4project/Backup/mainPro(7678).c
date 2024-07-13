#include "contrlDevices.h"
#include "inputCommand.h"
#include <pthread.h>

int c_fd;
struct Devices		 * pdeviceHead	  = NULL;
struct InputCommander* pCommandHead   = NULL;
struct InputCommander* socketHeadler  = NULL;

struct Devices* findDeviceByname(char *name,struct Devices* phead)
{
	struct Devices* tmp = phead;
	if(tmp == NULL){
		return NULL;
	}else{
		while(tmp != NULL){
			if(strcmp(tmp->devicesName,name) == 0){
				return tmp;
			}
			tmp = tmp ->next;
		}
		return NULL;
	}
}

struct InputCommander* findInputCommanderByname(char *name,struct InputCommander* phead)
{
	struct InputCommander* tmp = phead;
	if(phead == NULL){
		return NULL;
	}else{
		while(tmp != NULL){
			if(strcmp(tmp->commandName,name) == 0){
				return tmp;
			}
			tmp = tmp ->next;
		}
		return NULL;
	}
}

void *voice_thread(void *data)
{
	int n_read = 0;
	struct Devices* tmp;
	struct InputCommander* voicdHeadler;
	voicdHeadler = findInputCommanderByname("voice",pCommandHead);
	if(voicdHeadler == NULL){
		printf("find voicdHeadler error\n");
		pthread_exit(NULL);
	}else{
		if(voicdHeadler->Init(voicdHeadler,NULL,NULL) < 0){
			printf("voice init error\n");
			pthread_exit(NULL);
		}else{
			printf("%s init success\n",voicdHeadler->commandName);
		}
		/*
		  while(1){
                n_read = read(fd,cmd,sizeof(cmd));
                if(strlen(cmd) == 0){
                        printf("失败\n");
                        continue;
                }
                if(strstr(cmd,"open") != NULL){
                        printf("open\n");
                        digitalWrite(25, LOW);
                }
                if(strstr(cmd,"close") != NULL){
                        printf("close\n");
                        digitalWrite(25,HIGH);
                }
                memset(cmd,'\0',sizeof(cmd)/sizeof(char));
        }

		*/
		while(1){
			n_read = voicdHeadler->getCommand(voicdHeadler);
			if(strlen(voicdHeadler->command) == 0){
				printf("no data form voice\n");
			}else{
				tmp = findDeviceByname("bathRoomLight",pdeviceHead);
				if(strstr(voicdHeadler->command,"YES") != NULL){
					tmp->devicesInit(tmp->pinNum);
					tmp->open(tmp->pinNum);
					sleep(1);
					tmp->close(tmp->pinNum);
					sleep(1);
                }
				printf("do divece contrl:%s",voicdHeadler->command);
			}
		}
	}
}

void *read_thread(void *data)
{
	int n_read;
	n_read = read(c_fd,socketHeadler->command,sizeof(socketHeadler->command));//将套接字描述符c_fd中的数据读1024个到readBuf中
	if(n_read == -1){
	   perror("read"); //打印错误信息
	}else if(n_read > 0){
	   printf("get message:%s\n",socketHeadler->command); //将从客户端读到的数据打印
	}else{
		printf("client quit\n");
	}
}

void *socket_thread(void *data)
{
	int clen;
	int n_read = 0;
	pthread_t readThread;
		
	struct sockaddr_in c_addr; //用来返回已经连接的客户端的协议地址
	memset(&c_addr,0,sizeof(struct sockaddr_in));
	clen = sizeof(struct sockaddr_in); //结构体长度
	
	socketHeadler = findInputCommanderByname("socketServer",pCommandHead);
	if(socketHeadler == NULL){
		printf("find socketHeadler error\n");
		pthread_exit(NULL);
	}
	else{
		printf("%s init sucessful....\n",socketHeadler->commandName);
	}
	socketHeadler->Init(socketHeadler,NULL,NULL);
	while(1){
		c_fd = accept(socketHeadler->sfd,(struct sockaddr *)&c_addr,&clen); //连接客户端
		if(c_fd == 0){
			printf("error\n");
			perror("accept");
		}
		
		pthread_create(&readThread,NULL,read_thread,NULL);
	}
	
}

int main()
{
	if(wiringPiSetup() == -1){

		return -1;
	}
//	char *name = "bathRoomLight";
//	char *name = "upstairLight";
//	char *name = "livingRoomLight";
//	char *name = "RestaurantLight";
	char name[128] = {0};
	struct Devices* tmp;
	pthread_t voicethread;
	pthread_t socketthread;
	
//	struct Devices       * pCommandHead = NULL;
//	struct InputCommander* pdeviceHead  = NULL;


	//1、指令工厂初始化
	pCommandHead = addvoiceContrlToInputCommandLink(pCommandHead);
	pCommandHead = addsocketContrlToInputCommandLink(pCommandHead);


	//2、设备控制工厂初适化
	pdeviceHead = addBathroomLightToDeviceLink(pdeviceHead);     //将bathRoomLight加入到链表中去
	pdeviceHead = addupstairLightToDeviceLink(pdeviceHead);
	pdeviceHead = addlivingroomLightToDeviceLink(pdeviceHead);
	pdeviceHead = addRestaurantLightToDeviceLink(pdeviceHead);
	pdeviceHead = addfireIfOrNotToDeviceLink(pdeviceHead);

	
	//3、线程池的建立
//int pthread_create(pthread_t *restrict tidp, const pthread_attr_t *restrict attr, void *(*start_rtn)(void *), void *restrict arg);

	//	3.1、语音线程

	int voiceRet = pthread_create(&voicethread,NULL,voice_thread,NULL);
	if(voiceRet == 0){
        //printf("voiceRet create success\n");
    }

	//  3.2、socket线程

	int socketRet = pthread_create(&socketthread,NULL,socket_thread,NULL);
	if(socketRet == 0){
        //printf("socketRet create success\n");
    }

	//  3.3、摄像头线程
	//  3.4、火灾线程

	pthread_join(voicethread,NULL);
	pthread_join(socketthread,NULL);

/*
	while(1){
		printf("please input:\n");
		scanf("%s",name);

		tmp = findDeviceByname(name,pdeviceHead);
		if(tmp !=NULL){
			tmp->devicesInit(tmp->pinNum);
			tmp->open(tmp->pinNum);
			sleep(1);
			tmp->close(tmp->pinNum);
			sleep(1);
		}
	}*/

	return 0;
	
}

