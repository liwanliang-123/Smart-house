#include <wiringPi.h>
#include <stdlib.h>


struct InputCommander		//指令控制工厂链表节点定义
{
	char commandName[128];
	char deviceName[128];
	char command[32];			//存放指令
	int (*Init)(struct InputCommander *voicer, char *ipAdress, char *port);
	int (*getCommand)(struct InputCommander*voicer);
	char log[1024];
	int fd;
	char port[12];
	char ipAddress[32];
	int sfd;	
	
	struct InputCommander *next;

};

struct InputCommander* addvoiceContrlToInputCommandLink(struct InputCommander *phead);
struct InputCommander* addSocketContrlToInputCommandLink(struct InputCommander *phead);





