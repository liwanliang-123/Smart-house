#include "contrlDevices.h"
#include <stdlib.h>

char camBuf[1024] = {'\0'};
//struct Devices* cameraN = NULL;

int cameraData = 0;

int cameraopen(int pinNum)
{
	digitalWrite(pinNum,LOW);

}
int cameraclose(int pinNum)
{
	digitalWrite(pinNum,HIGH);

}
int cameraInit(int PinNum)
{
	pinMode(PinNum,OUTPUT);
	digitalWrite(PinNum,HIGH);
}

int cameraRead(int pinNum)
{
	return digitalRead(pinNum);
}

char *CamerGetPicBase64FromFile(char *filePath)
{
	char *bufPic;
	char cmd[128] = {'\0'};
	sprintf(cmd,"base64 %s > tmpFile",filePath);
	system(cmd);

	int fd = open("./tmpFile",O_RDWR);
	int filelen = lseek(fd,0,SEEK_END);
	bufPic = (char *)malloc(filelen + 2);
	lseek(fd,0,SEEK_SET);
	memset(bufPic,0,filelen+2);
	read(fd,bufPic,filelen);
	close(fd);

	system("rm -f tmpFile");

	return bufPic;
}

size_t readData(void *ptr, size_t size, size_t nmemb,void *stream)
{
	strncpy(camBuf,ptr,1024);
	printf("===========================get data=====================\n");
	printf("%s\n",camBuf);
}


bool camerPostUrl()
{
		int fd;
		int filelen;

		CURL *curl;
		CURLcode res;
		char *postString;
		char img1[24];
		char img2[24];
		char *key = "4LgQdJpPjyd3g9JyQoRxKa";
		char *secret = "cbbe3ba272e44f10b2b2e51ff761d2c0";
		int typeId = 21;
		char *format = "xml";
		
		char *bufPic1 = CamerGetPicBase64FromFile("./test.jpg");
		char *bufPic2 = CamerGetPicBase64FromFile("./image.jpg");
	
		int len = strlen(key) + strlen(secret) + strlen(bufPic1)+strlen(bufPic2) + 1024;
		postString = (char *)malloc(strlen(key) + strlen(secret) + strlen(bufPic1)+strlen(bufPic2) + 1024);
		memset(postString,'\0',sizeof(postString));
		sprintf(postString,"&img1=%s&img2=%s&key=%s&secret=%s&typeId=%d&format=%s",bufPic1,bufPic2,key,secret,21,format);
	
		curl = curl_easy_init();
		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt");				// 指定cookie文件
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postString); 					// 指定post内容
			curl_easy_setopt(curl, CURLOPT_URL, "https://netocr.com/api/faceliu.do");	// 指定url
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readData);					//将返回的http头输出到readData
			res = curl_easy_perform(curl);
	
			printf("OK:%d\n",res);
			if(strstr(camBuf,"是") != NULL){
				printf("the same Person\n");
				cameraData = 1;
				//cameraN->cameraNum = 1;
				sleep(3);
			}else{
				//cameraN->cameraNum = 0;
				cameraData = 0;
				printf("diff Person\n");
			}
			curl_easy_cleanup(curl);
		}
		return true;
}


void camer(void)
{
	system("raspistill -t 1000 -o image.jpg -q 5");
}

struct Devices camera = {

	.pinNum 	  = 6,
	.devicesName  = "camera",
	.open         = cameraopen,
	.close        = cameraclose,
	.devicesInit  = cameraInit,
	
	.readStatus   = cameraRead,

	.postUrl 	  = camerPostUrl,
	.cam          = camer
};

struct Devices* addcameraToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){    
		return &camera;         //camera作为头结点
	}else{
		camera.next = phead;
		phead = &camera;
	}
}
