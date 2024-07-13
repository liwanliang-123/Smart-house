#include "contrlDevices.h"
#include "inputCommand.h"

int 			c_fd;
float			dis = 0;
extern int		cameraData;

typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef unsigned long uint32;
#define HIGH_TIME 32
int pinNumber =26;	//use gpio1 to read data
char strff_TMP[32],strdd_RH[32];



#define KEL 					0
#define GEL 					1
#define KCT 					2
#define GCT 					3
#define KWS 					4
#define GWS 					5
#define KYC 					6
#define GYC 					7
#define OPEN 					8
#define CLOSE 					9
#define KFS 					10
#define GFS 					11
#define ZT 			     		12
#define GX 				    	13


struct Devices * tmp  = NULL;
struct Devices * tmp1 = NULL;
struct Devices * tmp2 = NULL;
struct Devices * tmp3 = NULL;
struct Devices * tmp4 = NULL;
struct Devices * pdeviceHead = NULL;
struct InputCommander * pCommandHead = NULL;
struct InputCommander * socketHeadler = NULL;

struct Devices * findDeviceByname(char * name, struct Devices * phead)
{
	struct Devices * tmp = phead;

	if (tmp == NULL)
		{
		return NULL;
		}
	else 
		{
		while (tmp != NULL)
			{
			if (strcmp(tmp->devicesName, name) == 0)
				{
				return tmp;
				}

			tmp 				= tmp->next;
			}

		return NULL;
		}
}


struct InputCommander * findInputCommanderByname(char * name, struct InputCommander * phead)
{
	struct InputCommander * tmp = phead;

	if (phead == NULL)
		{
		return NULL;
		}
	else 
		{
		while (tmp != NULL)
			{
			if (strcmp(tmp->commandName, name) == 0)
				{
				return tmp;
				}

			tmp 				= tmp->next;
			}

		return NULL;
		}
} 

int get_cmd_type(char * cmd)
{
	if (strstr(cmd, "KEL"))
		return KEL; //strstr用来查找字符串

	if (strstr(cmd, "GEL"))
		return GEL;

	if (strstr(cmd, "KCT"))
		return KCT;

	if (strstr(cmd, "GCT"))
		return GCT;

	if (strstr(cmd, "KWS"))
		return KWS;

	if (strstr(cmd, "GWS"))
		return GWS;

	if (strstr(cmd, "KYC"))
		return KYC;

	if (strstr(cmd, "GYC"))
		return GYC;

	if (strstr(cmd, "openAll"))
		return OPEN;

	if (strstr(cmd, "closeAll"))
		return CLOSE;

	if (strstr(cmd, "KFS"))
		return KFS;

	if (strstr(cmd, "GFS"))
		return GFS;
	
	if (strstr(cmd, "GX"))
		return GX;

	if (strstr(cmd, "ZT"))
		return ZT;
	
	return -1;
}


//语音线程
void * voice_thread(void * data)
{
	int 			datas;
	char			Strs;
	int 			n_read = 0;
	pinMode(0, OUTPUT);
	pinMode(2, OUTPUT);
	digitalWrite(0, LOW);
	digitalWrite(2, LOW);

	struct InputCommander * voicdHeadler;
	voicdHeadler		= findInputCommanderByname("voice", pCommandHead);

	if (voicdHeadler == NULL)
		{
		printf("find voicdHeadler error\n");
		pthread_exit(NULL);
		}
	else if (voicdHeadler->Init(voicdHeadler, NULL, NULL) < 0)
		{
		printf("voice init error\n");
		pthread_exit(NULL);
		}
	else 
		{
		printf("%s init success\n", voicdHeadler->commandName);
		}

	while (1)
		{
		n_read	= voicdHeadler->getCommand(voicdHeadler); //数据读到command里面

		if (n_read == 0){ //没有数据
			printf("voice waiting....\n");
			sleep(1);
		}
		datas = get_cmd_type(voicdHeadler->command);
		if (datas == -1){
			printf("get_cmd_type error\n");
			memset(voicdHeadler->command,'\0',sizeof(voicdHeadler->command));
		}
		else{
			switch (datas){
				case KFS:
					printf("KFS>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
					if (strstr(voicdHeadler->command, "KFS") != NULL) //开二楼灯
						digitalWrite(1, LOW);
						digitalWrite(2, HIGH);
					break;

				case GFS:
					printf("GFS>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
					if (strstr(voicdHeadler->command, "GFS") != NULL) //关二楼
						digitalWrite(1, LOW);
						digitalWrite(2, LOW);
					break;
					
				case KEL:
					if (strstr(voicdHeadler->command, "KEL") != NULL) //开二楼灯
					tmp = findDeviceByname("upstairLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->open(tmp->pinNum);
					break;

				case GEL:
					if (strstr(voicdHeadler->command, "GEL") != NULL) //关二楼
						tmp = findDeviceByname("upstairLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->close(tmp->pinNum);
					break;

				case KCT:
					if (strstr(voicdHeadler->command, "KCT") != NULL) //开餐厅灯
						tmp = findDeviceByname("RestaurantLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->open(tmp->pinNum);
					break;

				case GCT:
					if (strstr(voicdHeadler->command, "GCT") != NULL) //关二楼灯
						tmp = findDeviceByname("RestaurantLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->close(tmp->pinNum);
					break;

				case KWS:
					if (strstr(voicdHeadler->command, "KWS") != NULL) //开卧室灯
						tmp = findDeviceByname("bathRoomLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->open(tmp->pinNum);
					break;

				case GWS:
					if (strstr(voicdHeadler->command, "GWS") != NULL) //关二楼灯
						tmp = findDeviceByname("bathRoomLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->close(tmp->pinNum);
					break;

				case KYC:
					if (strstr(voicdHeadler->command, "KYC") != NULL) //开二楼灯
						tmp = findDeviceByname("livingRoomLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->open(tmp->pinNum);
					break;

				case GYC:
					if (strstr(voicdHeadler->command, "GYC") != NULL) //关二楼灯
						tmp = findDeviceByname("livingRoomLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->close(tmp->pinNum);
					break;

				case OPEN:
					if (strstr(voicdHeadler->command, "openAll") != NULL) //开所有
						tmp1 = findDeviceByname("livingRoomLight", pdeviceHead); 
						tmp2 = findDeviceByname("upstairLight", pdeviceHead); 
						tmp3 = findDeviceByname("bathRoomLight", pdeviceHead);
						tmp4 = findDeviceByname("RestaurantLight", pdeviceHead);
						tmp1->devicesInit(tmp1->pinNum);
						tmp2->devicesInit(tmp2->pinNum);
						tmp3->devicesInit(tmp3->pinNum);
						tmp4->devicesInit(tmp4->pinNum);
						tmp1->open(tmp1->pinNum);
						tmp2->open(tmp2->pinNum);
						tmp3->open(tmp3->pinNum);
						tmp4->open(tmp4->pinNum);
					break;

				case CLOSE:
					if (strstr(voicdHeadler->command, "closeAll") != NULL) //开所有
						tmp1 = findDeviceByname("livingRoomLight", pdeviceHead); 
						tmp2 = findDeviceByname("upstairLight", pdeviceHead); 
						tmp3 = findDeviceByname("bathRoomLight", pdeviceHead);
						tmp4 = findDeviceByname("RestaurantLight", pdeviceHead);
						tmp1->devicesInit(tmp1->pinNum);
						tmp2->devicesInit(tmp2->pinNum);
						tmp3->devicesInit(tmp3->pinNum);
						tmp4->devicesInit(tmp4->pinNum);
						tmp1->close(tmp1->pinNum);
						tmp2->close(tmp2->pinNum);
						tmp3->close(tmp3->pinNum);
						tmp4->close(tmp4->pinNum);
					break;

				default:
					printf("Voice error\n");
				}
			}

		memset(voicdHeadler->command, '\0', sizeof(voicdHeadler->command));
		}

}


void * read_thread(void * data)
{
	int n_read;
	int datas;
	n_read	= read(c_fd, socketHeadler->command, sizeof(socketHeadler->command)); //将套接字描述符c_fd中的数据读1024个到readBuf中

	if (n_read == -1)
		{
		perror("read"); 							//打印错误信息
		}
	else if (n_read > 0)
		{
		//printf("get message:%s\n", socketHeadler->command); //将从客户端读到的数据打印
		datas = get_cmd_type(socketHeadler->command);
		if (datas == -1){
			printf("sockrt_get_cmd_type error\n");
			memset(socketHeadler->command,'\0',sizeof(socketHeadler->command));
		}
		else{
			switch(datas){
				case KFS:
					printf("KFS>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
					if (strstr(socketHeadler->command, "KFS") != NULL) //开二楼灯
						digitalWrite(1, LOW);
						digitalWrite(2, HIGH);
					break;

				case GFS:
					printf("GFS>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
					if (strstr(socketHeadler->command, "GFS") != NULL) //关二楼
						digitalWrite(1, LOW);
						digitalWrite(2, LOW);
					break;
					
				case KEL:
					if (strstr(socketHeadler->command, "KEL") != NULL) //开二楼灯
					tmp = findDeviceByname("upstairLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->open(tmp->pinNum);
					break;

				case GEL:
					if (strstr(socketHeadler->command, "GEL") != NULL) //关二楼
						tmp = findDeviceByname("upstairLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->close(tmp->pinNum);
					break;

				case KCT:
					if (strstr(socketHeadler->command, "KCT") != NULL) //开餐厅灯
						tmp = findDeviceByname("RestaurantLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->open(tmp->pinNum);
					break;

				case GCT:
					if (strstr(socketHeadler->command, "GCT") != NULL) //关二楼灯
						tmp = findDeviceByname("RestaurantLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->close(tmp->pinNum);
					break;

				case KWS:
					if (strstr(socketHeadler->command, "KWS") != NULL) //开卧室灯
						tmp = findDeviceByname("bathRoomLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->open(tmp->pinNum);
					break;

				case GWS:
					if (strstr(socketHeadler->command, "GWS") != NULL) //关二楼灯
						tmp = findDeviceByname("bathRoomLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->close(tmp->pinNum);
					break;

				case KYC:
					if (strstr(socketHeadler->command, "KYC") != NULL) //开二楼灯
						tmp = findDeviceByname("livingRoomLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->open(tmp->pinNum);
					break;

				case GYC:
					if (strstr(socketHeadler->command, "GYC") != NULL) //关二楼灯
						tmp = findDeviceByname("livingRoomLight", pdeviceHead); //找二楼的链表节点

					tmp->devicesInit(tmp->pinNum);
					tmp->close(tmp->pinNum);
					break;

				case OPEN:
					if (strstr(socketHeadler->command, "openAll") != NULL) //开所有
						tmp1 = findDeviceByname("livingRoomLight", pdeviceHead); 
						tmp2 = findDeviceByname("upstairLight", pdeviceHead); 
						tmp3 = findDeviceByname("bathRoomLight", pdeviceHead);
						tmp4 = findDeviceByname("RestaurantLight", pdeviceHead);
						tmp1->devicesInit(tmp1->pinNum);
						tmp2->devicesInit(tmp2->pinNum);
						tmp3->devicesInit(tmp3->pinNum);
						tmp4->devicesInit(tmp4->pinNum);
						tmp1->open(tmp1->pinNum);
						tmp2->open(tmp2->pinNum);
						tmp3->open(tmp3->pinNum);
						tmp4->open(tmp4->pinNum);
					break;

				case CLOSE:
					if (strstr(socketHeadler->command, "closeAll") != NULL) //开所有
						tmp1 = findDeviceByname("livingRoomLight", pdeviceHead); 
						tmp2 = findDeviceByname("upstairLight", pdeviceHead); 
						tmp3 = findDeviceByname("bathRoomLight", pdeviceHead);
						tmp4 = findDeviceByname("RestaurantLight", pdeviceHead);
						tmp1->devicesInit(tmp1->pinNum);
						tmp2->devicesInit(tmp2->pinNum);
						tmp3->devicesInit(tmp3->pinNum);
						tmp4->devicesInit(tmp4->pinNum);
						tmp1->close(tmp1->pinNum);
						tmp2->close(tmp2->pinNum);
						tmp3->close(tmp3->pinNum);
						tmp4->close(tmp4->pinNum);
					break;
				default:
					printf("Voice error\n");
				}
			   printf("get message:%s\n", socketHeadler->command); //将从客户端读到的数据打印
		}
	}
	else {
		printf("client quit\n");
	}
}


void * put_thread(void * data)
{
	//将温湿度写入Socket
	printf("???????????????????????????????????????\n");
	write(c_fd,strff_TMP,6);
	sleep(1);
	write(c_fd,strdd_RH,6);
	sleep(1);
}

//网络线程
void * socket_thread(void * data)
{
	int 			clen;
	int 			n_read = 0;
	pthread_t		readThread;
	pthread_t       putThread;
	
	struct sockaddr_in c_addr; //用来返回已经连接的客户端的协议地址
	memset(&c_addr, 0, sizeof(struct sockaddr_in));
	clen				= sizeof(struct sockaddr_in); //结构体长度

	socketHeadler		= findInputCommanderByname("socketServer", pCommandHead);

	if (socketHeadler == NULL)
		{
		printf("find socketHeadler error\n");
		pthread_exit(NULL);
		}
	else 
		{
		printf("%s init sucessful....\n", socketHeadler->commandName);
		}

	socketHeadler->Init(socketHeadler, NULL, NULL);

	while (1)
		{
		c_fd				= accept(socketHeadler->sfd, (struct sockaddr *) &c_addr, &clen); //连接客户端

		if (c_fd == 0)
			{
			printf("error\n");
			perror("accept");
			}

		//printf("???????????????????????????????????????\n");
		//write(c_fd,"hello",5);
		//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>TMP:%s\n",strff_TMP);
		
		int datas = get_cmd_type(socketHeadler->command);
		if(datas != 12 || datas != 13){
			pthread_create(&readThread, NULL, read_thread, NULL);
		}
		memset(socketHeadler->command,'\0',sizeof(socketHeadler->command));
		int i = 0;
		if(datas == 13){
			for(i = 0;i < 5;i++){
				pthread_create(&putThread,NULL,put_thread,NULL);
				sleep(1);
			}
			datas = get_cmd_type(socketHeadler->command);
		}
		if(datas == 12){
			printf(">>>>>>>>>>>>>>quit");
			//pthread_exit(NULL);			
			memset(socketHeadler->command,'\0',sizeof(socketHeadler->command));
		}
		sleep(2);
	}

}

//摄像头线程
void * camera_thread(void * data)
{
	sleep(1);
	int 			camera_data = 25;

	struct Devices * camera = NULL;
	struct Devices * find_lock = NULL;

	while (1)
		{

		//printf("camera_thread ult_data = %0.2f cm\n",dis);
		sleep(1);

		if (dis >= camera_data)
			{ //低电平表示有人靠近
			printf("camera waiting....\n");
			sleep(1);
			}
		else if (dis < camera_data)
			{ //距离大于25
			camera				= findDeviceByname("camera", pdeviceHead);

			if (camera != NULL)
				{ //找到摄像头节点

				camera->cam();											  //拍照
				camera->postUrl();										  //进行比对
				if (cameraData == 1)
					{
					printf("识别成功\n");
					find_lock			= findDeviceByname("lockIfOrNot", pdeviceHead); //找锁控制引脚节点

					if (find_lock != NULL)
						{ //表示找到锁的控制节点

						find_lock->devicesInit(find_lock->pinNum); //初始化引脚
						find_lock->open(find_lock->pinNum); //表示锁已经打开
						sleep(3);					//锁打开后延时3s
						find_lock->close(find_lock->pinNum); //关闭锁
						sleep(3);

						}
					else 
						{
						printf("find lockIfOrNot error\n");
						pthread_exit(NULL);
						}
					}
				else 
					{
					printf("cameraData : %d\n", cameraData);
					printf("识别失败\n");
					}
				}
			}

		sleep(2);
		}
}

//火灾报警线程
void * fire_thread(void * data)
{
	int 			fire_data;

	struct Devices * fire = NULL;
	struct Devices * Buzz = NULL;
	fire				= findDeviceByname("fireIfOrNot", pdeviceHead); //找火灾控制引脚节点

	if (fire != NULL)
		{
		fire->devicesInit(fire->pinNum);			//初始化引脚

		while (1)
			{ //不断的读取引脚

			//printf("fire_thread ult_data = %0.2f cm\n",dis);
			fire_data			= fire->readStatus(fire->pinNum);
			printf("fire_data = %d\n", fire_data);

			if (fire_data != 0){ //低电平表示火灾传感器输入
				printf("fire waiting ......\n");
				sleep(1);
			}
			else{
				printf("BUZZ success!\n");
				Buzz = findDeviceByname("buzz", pdeviceHead); //找蜂鸣器节点

				if (Buzz != NULL){ //找到节点
					Buzz->devicesInit(Buzz->pinNum); //初始化蜂鸣器引脚
					Buzz->open(Buzz->pinNum);		//打开蜂鸣器
					sleep(1);
					Buzz->close(Buzz->pinNum);		//关闭蜂鸣器
					sleep(1);
					Buzz->open(Buzz->pinNum);		//打开蜂鸣器
					sleep(1);
					Buzz->close(Buzz->pinNum);
				}
			}
			sleep(2);
		}
	}
}

void * ult_thread(void * data)
{
	struct timeval tv1;
	struct timeval tv2;
	long			startTime, stopTime;

	pinMode(5, INPUT);								//设置端口为输入
	pinMode(4, OUTPUT); 							//设置端口为输出

	while (1)
		{
		digitalWrite(Trig, LOW);					//这里延时是为了在高电平来的前一段时间保持低电平
		delayMicroseconds(5);

		digitalWrite(Trig, HIGH);
		delayMicroseconds(10);						//发出超声波脉冲
		digitalWrite(Trig, LOW);

		while (! (digitalRead(Echo) == 1))
			; //等待高电平的到来

		if (gettimeofday(&tv1, NULL) == -1)
			{ //获取当前时间 开始接收到返回信号的时候 也就是高电平开始时间
			printf("tv1时间获取错误\n");

			//return -1;
			}

		while (! (digitalRead(Echo) == 0))
			; //等待高电平的结束 低电平的到来

		if (gettimeofday(&tv2, NULL) == -1)
			{ //获取当前时间	最后接收到返回信号的时候 也就是高电平结束时间
			printf("tv2时间获取错误\n");

			//return -1;
			}

		startTime			= tv1.tv_sec * 1000000 + tv1.tv_usec; //tv1.tv_sec单位是秒，乘1000000表示微秒，再加上后面的微秒就是时间
		stopTime			= tv2.tv_sec * 1000000 + tv2.tv_usec; //单位为微秒

		dis 				= (float) (stopTime - startTime) / 1000000 * 34000 / 2; //计算时间差求出距离

		/*结束时间-开始时间就是种时间，除以1000000单位就是秒，乘速度34000 cm/s*/
		printf("ult %0.2f cm\n", dis);

		sleep(2);
		}
}


void * tem_thread(void *data)
{	 	 
	uint32 databuf;
	 
	uint8 readSensorData(void)
	{
		uint8 crc; 
		uint8 i;
	 
		pinMode(pinNumber,OUTPUT); // set mode to output
		digitalWrite(pinNumber, 0); // output a high level 
		delay(25);
		digitalWrite(pinNumber, 1); // output a low level 
		pinMode(pinNumber, INPUT); // set mode to input
		pullUpDnControl(pinNumber,PUD_UP);
	 
		delayMicroseconds(27);
		if(digitalRead(pinNumber)==0){ //SENSOR ANS
		
			 while(!digitalRead(pinNumber)); //wait to high
	 
		  for(i=0;i<32;i++){
		  	
		   while(digitalRead(pinNumber)); //data clock start
		   while(!digitalRead(pinNumber)); //data start
			  delayMicroseconds(HIGH_TIME);
			  databuf*=2;
			   if(digitalRead(pinNumber)==1){ //1
					databuf++;
			   }
		 }
	 
		  for(i=0;i<8;i++){
		   while(digitalRead(pinNumber)); //data clock start
		   while(!digitalRead(pinNumber)); //data start
			  delayMicroseconds(HIGH_TIME);
			  crc*=2;  
			  if(digitalRead(pinNumber)==1){ //1
					crc++;
			   }
		 }
		 return 1;
	   }
	   else{
	   		return 0;
	  }
   }
	 
	printf("Enter OS-------\n");
	while(1) {
		pinMode(pinNumber,OUTPUT); // set mode to output
		digitalWrite(pinNumber, 26); // output a high level 
		delay(1000);
		if(readSensorData()){
		   printf("Congratulations ! Sensor data read ok!\n");
		   //printf("RH:%d.%d\n",(databuf>>24)&0xff,(databuf>>16)&0xff); 
		   //printf("TMP:%d.%d\n",(databuf>>8)&0xff,databuf&0xff);

		   double Sum = (databuf>>24)&0xff;
		   double  RH  = (databuf>>16)&0xff;
		   double Sum_RH = Sum + RH;
		   sprintf(strdd_RH,"%.1lf",Sum_RH);  //数字转化为字符串
		   strcat(strdd_RH,"s");
		   printf("RH:%s\n",strdd_RH);
		   
		   float Sum1  = (databuf>>8)&0xff;
		   sprintf(strff_TMP,"%f",Sum1);
		   float tmp  = databuf&0xff;
		   tmp = tmp/10;                 //缩小10倍
		   sprintf(strff_TMP,"%f",tmp);
		   float Sum_tmp = Sum1 + tmp;   //相加
		   sprintf(strff_TMP,"%.1f",Sum_tmp);
		   strcat(strff_TMP,"w");
		   printf("TMP:%s\n",strff_TMP);
		   
		   //memset(strff_TMP,0,sizeof(strff_TMP)); memset(strdd_RH,0,sizeof(strdd_RH));
		   databuf = 0;
		   Sum_tmp = 0;
		   Sum_RH = 0;

	   }
		else{
		   printf("Sorry! Sensor dosent ans!\n");
		   databuf=0;
	   }
	   sleep(3);
   }
}

int main(void)
{
	if (wiringPiSetup() == -1){
		printf("wiringPi Init error\n");
		return - 1;
	}

	pinMode(pinNumber, OUTPUT); // set mode to output
	digitalWrite(pinNumber, 26); // output a high level 

	char name[128] ={0};
	struct Devices * tmp = NULL;

	pthread_t		voicethread;
	pthread_t		socketthread;
	pthread_t		camerathread;
	pthread_t		firethread;
	pthread_t		ultsthread;
	pthread_t       temthread;

	//1、指令工厂初始化
	pCommandHead		= addvoiceContrlToInputCommandLink(pCommandHead);
	pCommandHead		= addsocketContrlToInputCommandLink(pCommandHead);

	//2、设备控制工厂初适化
	pdeviceHead 		= addBathroomLightToDeviceLink(pdeviceHead); //将bathRoomLight加入到链表中去
	pdeviceHead 		= addupstairLightToDeviceLink(pdeviceHead);
	pdeviceHead 		= addlivingroomLightToDeviceLink(pdeviceHead);
	pdeviceHead 		= addRestaurantLightToDeviceLink(pdeviceHead);
	pdeviceHead 		= addfireIfOrNotToDeviceLink(pdeviceHead); //火灾节点		
	pdeviceHead 		= addcameraToDeviceLink(pdeviceHead); //摄像头节点
	pdeviceHead 		= addcamerReadToDeviceLink(pdeviceHead); //读摄像头引脚
	pdeviceHead 		= addBuzzToDeviceLink(pdeviceHead); //蜂鸣器节点添加到工厂
	pdeviceHead 		= addlockIfOrNotToDeviceLink(pdeviceHead); //锁节点添加到工厂

	//3、线程池的建立
	//int pthread_create(pthread_t *restrict tidp, const pthread_attr_t *restrict attr, void *(*start_rtn)(void *), void *restrict arg);
	//3.1 、超声波线程
	int 			ultRet = pthread_create(&ultsthread, NULL, ult_thread, NULL);

	if (ultRet == 0)
		{
		printf("ultRet create success\n");
		}

	//	3.2、语音线程
	int 			voiceRet = pthread_create(&voicethread, NULL, voice_thread, NULL);

	if (voiceRet == 0)
		{
		printf("voiceRet create success\n");
		}

	//	3.3、socket线程
	int 			socketRet = pthread_create(&socketthread, NULL, socket_thread, NULL);

	if (socketRet == 0)
		{
		printf("socketRet create success\n");
		}

	//	3.4、摄像头线程
	int 			cameraRet = pthread_create(&camerathread, NULL, camera_thread, NULL);

	if (cameraRet == 0)
		{
		printf("cameraRet create success\n");
		}

	//	3.5、火灾线程
	int 			fireRet = pthread_create(&firethread, NULL, fire_thread, NULL);

	if (fireRet == 0)
		{
		printf("fireRet create success\n");
		}

	//温湿度线程
	int             temRet     = pthread_create(&temthread,NULL,tem_thread,NULL);
	if(temRet == 0){

		printf("temRet creat success!\n");
	}

	//等待线程的退出
	pthread_join(voicethread, NULL);
	pthread_join(socketthread, NULL);
	pthread_join(camerathread, NULL);
	pthread_join(firethread, NULL);
	pthread_join(ultsthread, NULL);
	pthread_join(temthread,NULL);

	return 0;

}


