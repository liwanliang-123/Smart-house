#include <wiringPi.h>

struct Devices* addBathroomLightToDeviceLink(struct Devices *phead);
struct Devices* addlivingroomLightToDeviceLink(struct Devices *phead);
struct Devices* addupstairLightToDeviceLink(struct Devices *phead);
struct Devices* addRestaurantLightToDeviceLink(struct Devices *phead);
struct Devices* addfireIfOrNotToDeviceLink(struct Devices *phead);
struct Devices* addcameraToDeviceLink(struct Devices *phead);


struct Devices{

	char devicesName[128];
	int status;
	int pinNum;

	int (*open)(int PinNum);
	int (*close)(int PinNum);
	int (*devicesInit)(int PinNum);

	int (*readStatus)(int pinNum);
	int (*changeStatus)(int status);

	struct Devices* next;
};
