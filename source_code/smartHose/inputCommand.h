#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

struct InputCommander{

	char commandName[128];
	char command[128];
	int (*Init)(struct InputCommander *voice,char *ipAddress,char *port);
	int (*getCommand)(struct InputCommander *voice);

	char log[1024];
	int baudRate;
	int fd;
	int sfd;
	char DevicesName[128];
	char port[12];
	char ipAddress[64];
	struct InputCommander *next;
};

struct InputCommander* addvoiceContrlToInputCommandLink(struct InputCommander *phead);
struct InputCommander* addsocketContrlToInputCommandLink(struct InputCommander *phead);


