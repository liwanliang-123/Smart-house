#include "contrlDevices.h"
#include "inputCommand.h"

int c_fd;
float dis = 0;

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

//语音线程
void *voice_thread(void *data)
{
	int n_read = 0;
	struct Devices* tmp;
	struct InputCommander* voicdHeadler;
	voicdHeadler = findInputCommanderByname("voice",pCommandHead);
	if(voicdHeadler == NULL){
		printf("find voicdHeadler error\n");
		pthread_exit(NULL);
	}else if(voicdHeadler->Init(voicdHeadler,NULL,NULL) < 0){
			printf("voice init error\n");
			pthread_exit(NULL);
	}else{
			printf("%s init success\n",voicdHeadler->commandName);
	}
	while(1){
		
		n_read = voicdHeadler->getCommand(voicdHeadler);                 //数据读到command里面
		if(n_read == 0){                          //没有数据
			//printf("no data form voice\n");
			printf("voice waiting....\n");
			sleep(3);
		}else {
			printf("do divece contrl:%s\n",voicdHeadler->command);
		}
		if(strstr(voicdHeadler->command,"KEL") != NULL){           //开二楼灯
				tmp = findDeviceByname("upstairLight",pdeviceHead);      //找二楼的链表节点
				tmp->devicesInit(tmp->pinNum);                     
				tmp->open(tmp->pinNum);
				sleep(2);
				
        }else if(strstr(voicdHeadler->command,"GEL") != NULL){           //关二楼灯
				tmp = findDeviceByname("upstairLight",pdeviceHead);      //找二楼的链表节点
				tmp->devicesInit(tmp->pinNum);                     
				tmp->close(tmp->pinNum);
				sleep(2);
        }
		//printf("do divece contrl:%s",voicdHeadler->command);
		memset(voicdHeadler->command,'\0',sizeof(voicdHeadler->command));		
		sleep(2);
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

//网络线程
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
		sleep(2);
	}
	
}

//摄像头线程
/*
void *camera_thread(void *data)
{
	int camera_data = 0;
	struct Devices*    read_camera = NULL;
	struct Devices*         camera = NULL;
	struct Devices*      find_lock = NULL;
	
	read_camera = findDeviceByname("camerRead",pdeviceHead);             //找摄像头控制引脚节点
	if(read_camera == NULL){
		printf("camera_thread 失败\n");
		pthread_exit(NULL);
	}else{		
		read_camera->devicesInit(read_camera->pinNum);                    //初始化引脚
	}
	while(1){
		camera_data = read_camera->readStatus(read_camera->pinNum);       //不断的读取引脚
		printf("camera_data = %d\n",camera_data);
		if(camera_data != 0){                          					  //低电平表示有人靠近
			printf("camera waiting....\n");
			sleep(3);
		}else{ 
			camera = findDeviceByname("camera",pdeviceHead);    		  //去找摄像头节点
			if(camera !=NULL){                                  		  //找到摄像头节点
				//tmp->cam(); 				                 			  //拍照
				//tmp->postUrl(); 			                              //进行比对
				printf("拍照成功\n");
				sleep(3);
				printf("camera_data else = %d\n",camera_data);

				find_lock = findDeviceByname("lockIfOrNot",pdeviceHead);  //找锁控制引脚节点
				if(find_lock != NULL){                                    //表示找到锁的控制节点
					find_lock->devicesInit(find_lock->pinNum);            //初始化引脚
					find_lock->open(find_lock->pinNum);                   //表示锁已经打开
					sleep(3);                                             //锁打开后延时3s
					find_lock->close(find_lock->pinNum);                  //关闭锁
				}
			}
		}
	}
}

*/
void *camera_thread(void *data)
{

	int camera_data = 0;
	
	struct Devices* 		camera = NULL;
	struct Devices* 	 find_lock = NULL;
		
	while(1){
		
		printf("camera_thread ult_data = %0.2f cm\n",dis);
		sleep(2);
		if(dis >= 25){											  //低电平表示有人靠近
			printf("camera waiting....\n");
			sleep(2);
		}else if(dis < 25){                                         //距离大于25
			camera = findDeviceByname("camera",pdeviceHead);
			if(camera !=NULL){											  //找到摄像头节点
				//tmp->cam();											  //拍照
				//tmp->postUrl();										  //进行比对
				printf("拍照成功\n");
				//sleep(3);
				//printf("camera_data else = %d\n",camera_data);
	
				find_lock = findDeviceByname("lockIfOrNot",pdeviceHead);  //找锁控制引脚节点
				printf("lock..............\n");
				if(find_lock != NULL){									  //表示找到锁的控制节点
					find_lock->devicesInit(find_lock->pinNum);			  //初始化引脚
					find_lock->open(find_lock->pinNum); 				  //表示锁已经打开
					sleep(3);											  //锁打开后延时3s
					find_lock->close(find_lock->pinNum);				  //关闭锁
					printf("lock..............\n");
				}else if(find_lock == NULL){
					printf("else lock>>>>>>>>>>>>\n");
				}
			}
		}
		sleep(1);
	}
}

	/*
		tmp = findDeviceByname("camera",pdeviceHead);
		if(tmp !=NULL){
			//tmp->devicesInit(tmp->pinNum);	//初始化引脚
			tmp->cam(); 				  //拍照
			tmp->postUrl(); 			  //进行比对
		}
	*/

//火灾报警线程

void *fire_thread(void *data)
{	
	int fire_data;
	struct Devices* fire = NULL;
	struct Devices* Buzz = NULL;
	fire = findDeviceByname("fireIfOrNot",pdeviceHead);           //找火灾控制引脚节点
	if(fire != NULL){
		fire->devicesInit(fire->pinNum);                       	 //初始化引脚
		while(1){                                                //不断的读取引脚
		    printf("fire_thread ult_data = %0.2f cm\n",dis);
			fire_data = fire->readStatus(fire->pinNum);
			printf("fire_data = %d\n",fire_data);
			if(fire_data != 0){             							//低电平表示火灾传感器输入
				printf("fire waiting ......\n");
				sleep(2);
			}else{
				Buzz = findDeviceByname("buzz",pdeviceHead);      //找蜂鸣器节点
				if(Buzz !=NULL){                                  //找到节点
					Buzz->devicesInit(Buzz->pinNum);               //初始化蜂鸣器引脚
					Buzz->open(Buzz->pinNum);                      //打开蜂鸣器
					sleep(2);
					Buzz->close(Buzz->pinNum);                     //关闭蜂鸣器
					sleep(3);
				}
			}
			sleep(2);
		}
	}
}

void *ult_thread(void *data)
{
	struct Devices* camera = NULL;

    struct timeval tv1;
    struct timeval tv2;
    long startTime, stopTime;
	pinMode(5, INPUT);  //设置端口为输入
    pinMode(4, OUTPUT); //设置端口为输出

 	while(1){
	    digitalWrite(Trig, LOW);    //这里延时是为了在高电平来的前一段时间保持低电平
	    delayMicroseconds(5);
	 
	    digitalWrite(Trig, HIGH);
	    delayMicroseconds(10);      //发出超声波脉冲
	    digitalWrite(Trig, LOW);
	 
	    while(!(digitalRead(Echo) == 1));   //等待高电平的到来
	    if(gettimeofday(&tv1, NULL) == -1){ //获取当前时间 开始接收到返回信号的时候 也就是高电平开始时间
	            printf("tv1时间获取错误\n");
	            //return -1;
	    }
	    while(!(digitalRead(Echo) == 0));   //等待高电平的结束 低电平的到来
	    if(gettimeofday(&tv2, NULL) == -1){ //获取当前时间  最后接收到返回信号的时候 也就是高电平结束时间
	            printf("tv2时间获取错误\n");
	            //return -1;
	    }
	    startTime = tv1.tv_sec * 1000000 + tv1.tv_usec;   //tv1.tv_sec单位是秒，乘1000000表示微秒，再加上后面的微秒就是时间
	    stopTime  = tv2.tv_sec * 1000000 + tv2.tv_usec;   //单位为微秒
	 
	    dis = (float)(stopTime - startTime) / 1000000 * 34000 / 2;  //计算时间差求出距离
	                    /*结束时间-开始时间就是种时间，除以1000000单位就是秒，乘速度34000 cm/s*/
	    printf("ult %0.2f cm\n",dis);
		
		sleep(3);
 	}
}


int main()
{
	if(wiringPiSetup() == -1){

		return -1;
	}
	char name[128] = {0};
	struct Devices* tmp;
	pthread_t voicethread;
	pthread_t socketthread;
	pthread_t camerathread;
	pthread_t firethread;
	pthread_t ultsthread;
	
	//1、指令工厂初始化
	pCommandHead = addvoiceContrlToInputCommandLink(pCommandHead);
	pCommandHead = addsocketContrlToInputCommandLink(pCommandHead);

	//2、设备控制工厂初适化
	pdeviceHead = addBathroomLightToDeviceLink(pdeviceHead);        //将bathRoomLight加入到链表中去
	pdeviceHead = addupstairLightToDeviceLink(pdeviceHead);
	pdeviceHead = addlivingroomLightToDeviceLink(pdeviceHead);
	pdeviceHead = addRestaurantLightToDeviceLink(pdeviceHead);
	pdeviceHead = addfireIfOrNotToDeviceLink(pdeviceHead);          //火灾节点       
	pdeviceHead = addcameraToDeviceLink(pdeviceHead);               //摄像头节点
	pdeviceHead = addcamerReadToDeviceLink(pdeviceHead);            //读摄像头引脚
	pdeviceHead = addBuzzToDeviceLink(pdeviceHead);                 //蜂鸣器节点添加到工厂
	pdeviceHead = addlockIfOrNotToDeviceLink(pdeviceHead);          //锁节点添加到工厂


	
	//3、线程池的建立
//int pthread_create(pthread_t *restrict tidp, const pthread_attr_t *restrict attr, void *(*start_rtn)(void *), void *restrict arg);

	int ultRet = pthread_create(&ultsthread,NULL,ult_thread,NULL);
	if(ultRet == 0){
        printf("ultRet create success\n");
    }

	//	3.1、语音线程

	int voiceRet = pthread_create(&voicethread,NULL,voice_thread,NULL);
	if(voiceRet == 0){
        printf("voiceRet create success\n");
    }

	//  3.2、socket线程

	int socketRet = pthread_create(&socketthread,NULL,socket_thread,NULL);
	if(socketRet == 0){
        printf("socketRet create success\n");
    }

	//  3.3、摄像头线程
	int cameraRet = pthread_create(&camerathread,NULL,camera_thread,NULL);
	if(cameraRet == 0){
        printf("cameraRet create success\n");
    }

	//  3.4、火灾线程

	int fireRet = pthread_create(&firethread,NULL,fire_thread,NULL);
	if(fireRet == 0){
        printf("fireRet create success\n");
    }


	//等待线程的退出

	  pthread_join(voicethread,NULL);
	  pthread_join(socketthread,NULL);
	  pthread_join(camerathread,NULL);
	  pthread_join(firethread,NULL);
	  pthread_join(ultsthread,NULL);

/*
		printf("please input:\n");
		scanf("%s",name);

		tmp = findDeviceByname(name,pdeviceHead);
		if(tmp !=NULL){
			//tmp->devicesInit(tmp->pinNum);    //初始化引脚
			tmp->cam();                   //拍照
			tmp->postUrl();                   //进行比对
		}
*/

	return 0;
	
}
