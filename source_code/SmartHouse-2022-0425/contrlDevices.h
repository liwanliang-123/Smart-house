#include <wiringPi.h>
#include <stdlib.h>

struct Devices
{
	char deviceName[128];
	int status;
	int pinNum;
	
	int (*open)(int pinNum);
	int (*close)(int pinNum);
	int (*deviceInit)(int pinNum);
	void (*justDoOnce)(); //用于摄像头
	char* (*getFace)(); //用于摄像头
	char* (*getPicFromOCRBase64)(); //用于摄像头
	size_t (*readData)(); //用于摄像头

	int (*readStatus)(int pinNum);
	int (*changeStatus)(int status);
	
	struct Devices *next;


};

struct Devices* addBathroomLightToDeviceLink(struct Devices *phead);
struct Devices* addUpstairLightToDeviceLink(struct Devices *phead);
struct Devices* addRestaurantLightToDeviceLink(struct Devices *phead);
struct Devices* addLivingroomLightToDeviceLink(struct Devices *phead);
struct Devices* addcameraToDeviceLink(struct Devices *phead);

struct Devices* addFireToDeviceLink(struct Devices *phead);
struct Devices* addBeepToDeviceLink(struct Devices *phead);











