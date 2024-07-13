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

#define SWITCH1 21		//��յ�ƶ�Ӧ������
#define SWITCH2 22
#define SWITCH3 23
#define SWITCH4 24
#define SWITCH5 25		//���洫������Ӧ������

struct InputCommander *pCommandHead = NULL;		//ָ����ƹ�������ڵ�ͷ
struct Devices		  *pdeviceHead = NULL;		//�豸��������ͷ�ڵ�
struct InputCommander *socketHandler = NULL;	//��������߳�ִ�еĺ���ʹ�õ���ȫ�ֱ�������read_thread�̺߳�socket_thread��ʹ�õ���

int c_fd;


struct Devices* findDeviceByName(char *name,struct Devices *phead)		//��������Ѱ���豸�����������ں���������������
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

struct InputCommander* findCommandByName(char *name,struct InputCommander *phead)		//��������Ѱ��ָ����ƹ����������ں���������������
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

void *fireAlarmThread(void *data)				//�����ֱ������̡߳�ִ�еĺ���
{
	int status;
	struct Devices *firetmp = NULL;
	struct Devices *buztmp = NULL;

	firetmp = findDeviceByName("fireIfOrNot",pdeviceHead);		//Ѱ�ҡ����洫����������ڵ㣬���ظ�firetmp
	buztmp = findDeviceByName("beep",pdeviceHead);				//Ѱ�ҡ�������������ڵ㣬���ظ�buztmp

	while(1){
		status = firetmp->readStatus(firetmp->pinNum);			//��ȡ�����洫������״̬
	
		if(status == 0){						//��⵽�����ǿ��Դ
			buztmp->open(buztmp->pinNum);		//�򿪷�����
			delay(1000);						//��ʱ1000����=1��
		}

		if(status == 1){						//δ��⵽���桢ǿ��Դ��������
			buztmp->close(buztmp->pinNum);		//�رշ�����
		}
	}
}

void * voice_thread(void* datas)		//�����߳�
{
	struct InputCommander *voiceHandler;	
	struct Devices        *tmp = NULL;

	int nread;
	voiceHandler = findCommandByName("voice",pCommandHead);		//�ҵ������ڵ�

	if(voiceHandler == NULL){		//��ֹ�δ���
		printf("find voiceHandler error\n");
		pthread_exit(NULL);	//�߳��ﲻ��return

	}else{
		if(voiceHandler->Init(voiceHandler,NULL,NULL)<0){	//�ж�voice��fd�Ƿ���ȷ
			printf("voice init error\n");
			pthread_exit(NULL);
		}else{
			printf("%s init success\n",voiceHandler->commandName);
		}
		while(1){
			nread = voiceHandler->getCommand(voiceHandler);	//ָ�����voiceHandler->command����
			if(nread==0){
				printf("waiting....\n");
			}else{
				printf("do divece contrl:%s\n",voiceHandler->command);
				if(strstr(voiceHandler->command,"WZ") != NULL){		//һ��ָ�

					printf("shoudaoXP\n");	
				}else if(strstr(voiceHandler->command,"KYS") != NULL){		//�Դ����յ���ָ����з�������ִ�ж�Ӧ�Ĳ���
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

void *cameraThread_func(void* data)//���̵߳ĺ����и�ʽҪ��
{
	struct Devices *cameraTemp;

	cameraTemp = findDeviceByName("camera", pdeviceHead); 	//�豸��Ҫ�ӹ�������ȡ����

	if(cameraTemp == NULL){  //��ֹ�δ���ı����жϣ�����ָ�븳ֵ�ǣ�һ��Ҫ����NULL���������������������ǿ�̸
		printf("find camera error\n");
		pthread_exit(NULL); //���߳��в���return
	}

	cameraTemp->justDoOnce(); //����postUrl����
}


void * read_thread(void* datas)
{
	int n_read;
	while(1){
		memset(socketHandler->command,'\0', sizeof(socketHandler->command));
		n_read = read(c_fd,socketHandler->command,sizeof(socketHandler->command));	//���׽����ж�ȡָ�����socketHandler->command��
		if(n_read == -1){
			perror("read");
		}else if(n_read>0){
			printf("\nget: %d,%s\n",n_read,socketHandler->command);	//��ӡ��ָ���ֽ�����ָ��
			if(strstr(socketHandler->command,"KYS") != NULL){		//��socket�յ���ָ����з�������ִ�ж�Ӧ�Ĳ���
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
			if(0 == strcmp(socketHandler->command,"OPCA")){		//��ָ����OPCAʱ��������ͷ�̣߳�������գ�����ʶ��
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

	if(socketHandler == NULL){	//��ֹ�δ���
		printf("find socketHandler error\n");
		pthread_exit(NULL);		//���߳��в���return
	}else{
		printf("%s init success\n",socketHandler->commandName);
	}
	socketHandler->Init(socketHandler,NULL,NULL);		//�ҵ�֮��ִ��socketInit����

	while(1){
		c_fd = accept(socketHandler->sfd, (struct sockaddr *)&c_addr, &clen);	//��������connect�����ӵ��󴴽�readThread�߳�
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

	//1.ָ�����ʼ��
	pCommandHead = addvoiceContrlToInputCommandLink(pCommandHead);
	pCommandHead = addSocketContrlToInputCommandLink(pCommandHead);

	//2.�豸���ƹ�����ʼ��
	pdeviceHead = addBathroomLightToDeviceLink(pdeviceHead);
	pdeviceHead = addUpstairLightToDeviceLink(pdeviceHead);
	pdeviceHead = addRestaurantLightToDeviceLink(pdeviceHead);
	pdeviceHead = addLivingroomLightToDeviceLink(pdeviceHead);
	pdeviceHead = addcameraToDeviceLink(pdeviceHead);
	pdeviceHead = addFireToDeviceLink(pdeviceHead);
	pdeviceHead = addBeepToDeviceLink(pdeviceHead);

	struct Devices *tmpDeviceshead = pdeviceHead;
	while(tmpDeviceshead != NULL){						//�豸���������豸��ʼ��
		tmpDeviceshead->deviceInit(tmpDeviceshead->pinNum);
		tmpDeviceshead = tmpDeviceshead->next;
	}
	
	
	//3.�̳߳ؽ���
	//int pthread_create(pthread_t *restrict tidp, const pthread_attr_t *restrict attr, void *(*start_rtn)(void *), void *restrict arg);
	//3.1�����߳�
	pthread_create(&voiceThread,NULL,voice_thread,NULL);

	//3.2socket�߳�
	pthread_create(&socketThread,NULL,socket_thread,NULL);

	//3.3����ͷ�߳�

	//3.4�����߳�
	pthread_create(&fireAlarm_thread,NULL,fireAlarmThread,NULL);
	printf("camera init success\n");

	pthread_join(voiceThread,NULL);		//�ȴ������̵߳���pthread_exit����
	pthread_join(socketThread,NULL);	//�ȴ�socket�̵߳���pthread_exit����
	pthread_join(fireAlarm_thread, NULL);	//�ȴ������̵߳���pthread_exit����


	return 0;
}

