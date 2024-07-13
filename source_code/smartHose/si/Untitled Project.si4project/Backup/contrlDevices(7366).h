#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <wiringPi.h>


#define true  1
#define false 0

#define Trig    4
#define Echo    5


typedef unsigned int bool;


struct Devices* addBathroomLightToDeviceLink(struct Devices *phead);
struct Devices* addlivingroomLightToDeviceLink(struct Devices *phead);
struct Devices* addupstairLightToDeviceLink(struct Devices *phead);
struct Devices* addRestaurantLightToDeviceLink(struct Devices *phead);
struct Devices* addfireIfOrNotToDeviceLink(struct Devices *phead);
struct Devices* addcameraToDeviceLink(struct Devices *phead);
struct Devices* addcamerReadToDeviceLink(struct Devices *phead);
struct Devices* addBuzzToDeviceLink(struct Devices *phead);
struct Devices* addlockIfOrNotToDeviceLink(struct Devices *phead);
struct Devices* addultrasonicIfOrNotToDeviceLink(struct Devices *phead);



char *CamerGetPicBase64FromFile(char *filePath);


struct Devices{

	char devicesName[128];
	int status;
	int pinNum;
	int pinNum4;
	int pinNum5;

	int (*open)(int PinNum);
	int (*close)(int PinNum);
	int (*devicesInit)(int PinNum);


	int (*devicesUltInit)(void);  //超声波初始化函数
	int (*readUltStatus)(void);   //超声波读取函数


	int (*readStatus)(int pinNum);
	int (*changeStatus)(int status);

	int (*getPicBase64)(char *filePath);
    bool (*postUrl)(void);
	void (*cam)(void);

	struct Devices* next;
};
