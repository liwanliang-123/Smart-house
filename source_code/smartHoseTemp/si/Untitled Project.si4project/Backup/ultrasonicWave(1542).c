#include "contrlDevices.h"

#define Trig    4
#define Echo    5

//float dis;

void ultraInit(void)
{
    pinMode(4, INPUT);  //设置端口为输入
    pinMode(5, OUTPUT); //设置端口为输出
}

float ultrasonicStatusRead(void)
{
	struct timeval tv1;
    struct timeval tv2;
    long startTime, stopTime;
 
    digitalWrite(Trig, LOW);    //Trig这里延时是为了在高电平来的前一段时间保持低电平
    delayMicroseconds(5);
 
    digitalWrite(Trig, HIGH);  //Trig
    delayMicroseconds(10);      //发出超声波脉冲
    digitalWrite(Trig, LOW);   //Trig

    while(!(digitalRead(Echo) == 1));   //Echo等待高电平的到来
    if(gettimeofday(&tv1, NULL) == -1){ //获取当前时间 开始接收到返回信号的时候 也就是高电平开始时间
            printf("tv1时间获取错误\n");
            return -1;
    }
    while(!(digitalRead(Echo) == 0));   //Echo等待高电平的结束 低电平的到来
    if(gettimeofday(&tv2, NULL) == -1){ //获取当前时间  最后接收到返回信号的时候 也就是高电平结束时间
            printf("tv2时间获取错误\n");
            return -1;
    }
    startTime = tv1.tv_sec * 1000000 + tv1.tv_usec;   //tv1.tv_sec单位是秒，乘1000000表示微秒，再加上后面的微秒就是时间
    stopTime  = tv2.tv_sec * 1000000 + tv2.tv_usec;   //单位为微秒
 
    dis = (float)(stopTime - startTime) / 1000000 * 34000 / 2;  //计算时间差求出距离
                    /*结束时间-开始时间就是种时间，除以1000000单位就是秒，乘速度34000 cm/s*/

	return dis;
}

struct Devices ultrasonicIfOrNot = {

	.devicesName     = "ultraIfOrNot",
	
	.devicesUltInit  = ultraInit,
	.readUltStatus   = ultrasonicStatusRead
};

struct Devices* addultrasonicIfOrNotToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){    
		return &ultrasonicIfOrNot;         //fireIfOrNot作为头结点
	}else{
		ultrasonicIfOrNot.next = phead;
		phead = &ultrasonicIfOrNot;
	}
}








