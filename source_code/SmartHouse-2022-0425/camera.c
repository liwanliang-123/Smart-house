#include "contrlDevices.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


void postUrl();
size_t readData1(void *ptr, size_t size, size_t nmemb, void *stream);
char* getFace1();
char* getPicFromOCRBase641(char *Filepath);
struct Devices* addcameraToDeviceLink(struct Devices *phead);





char ocrRetBuf[1024] = {'\0'};//ȫ�ֱ������������մ�OCR��̨���ص�����

size_t readData1(void *ptr, size_t size, size_t nmemb, void *stream)
//�ص��������ѴӺ�̨�����ݿ�����ocrRetBuf
{
        strncpy(ocrRetBuf,ptr,1024);
}



char* getFace1()
{
	printf("pai zhao zhong\n");
	system("raspistill -q 5 -t 1 -o image.jpg");
	
	while(access("./image.jpg",F_OK) != 0); //�ж��Ƿ��������
	
	printf("paizhao wanbi\n");
	
	char* base64BufFaceRec = getPicFromOCRBase641("./image.jpg");
	system("rm image.jpg");
	
	return base64BufFaceRec;   //���ظղ����յ�base64
}

char *getPicFromOCRBase641(char *Filepath)
{
        int fd;
        int filelen;
        char cmd[128]={'\0'};

        sprintf(cmd,"base64 %s > tmpFile",Filepath);
        system(cmd);
        fd=open("./tmpFile",O_RDWR);
        filelen=lseek(fd,0,SEEK_END);
        lseek(fd,0,SEEK_SET);
        char *bufpic=(char *)malloc(filelen+2);
        memset(bufpic,'\0',filelen+2);
        read(fd,bufpic,filelen+128);
        system("rm -rf tmpFile");
        close(fd);


        return bufpic;

}


void postUrl()
{
        CURL *curl;
        CURLcode res;

        //�ֿ����壬Ȼ���ַ���ƴ��
        char* key    = "xxx";	//����ƽ̨��������ʶ����key
        char* secret = "xxx";	//����ƽ̨��������ʶ����secret
        int   typeId = 21;	
        char* format = "xml";

        char* base64BufPic1 = getFace1();

        char* base64BufPic2 = getPicFromOCRBase641("yourself.jpg");

        int len = strlen(key)+strlen(secret)+strlen(base64BufPic1)+strlen(base64BufPic2)+128;//����ռ䲻���ᵼ��ջ���
        char* postString = (char* )malloc(len);
        memset(postString,'\0',len);//��ΪpostString��һ��ָ�룬������sizeof��������ָ��Ĵ�С

        sprintf(postString,"img1=%s&img2=%s&key=%s&secret=%s&typeId=%d&format=%s",base64BufPic1,base64BufPic2,key,secret,typeId,format);//����ƽ̨�Ĵ��θ�ʽ��д

        curl = curl_easy_init();

        if(curl){
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postString);  //ָ��post���ݣ��������  
                curl_easy_setopt(curl, CURLOPT_URL, "https://netocr.com/api/faceliu.do");// ָ��url
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,readData1);  //�ص�����readDate��ȡ����ֵ
                res = curl_easy_perform(curl);          //������״̬��
                printf("OK:%d\n",res);

                if(strstr(ocrRetBuf,"��") != NULL){    //�ж����ƺ�̨���ص��ַ�������û�С��ǡ�
                        printf("the same person\n");
                }
                else{
                        printf("different person\n");
                }
                curl_easy_cleanup(curl);
        }

}

int cameraInit(int pinNum)//�ú�������
{
	pinMode(pinNum,OUTPUT);				
	digitalWrite(pinNum,HIGH);
}


struct Devices camera = {

	.deviceName = "camera",
	.deviceInit = cameraInit,
	.pinNum = 6,             //�����ź�����
	.justDoOnce = postUrl,
	.getFace = getFace1,
	.getPicFromOCRBase64 = getPicFromOCRBase641,
	.readData = readData1

};

struct Devices* addcameraToDeviceLink(struct Devices *phead)
{
	if(phead == NULL){
		return &camera;
	}
	else{
		camera.next = phead;
		phead = &camera;
	}
}


