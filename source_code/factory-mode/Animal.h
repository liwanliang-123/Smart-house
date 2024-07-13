#include <stdio.h>
#include <string.h>

struct Animal{     //类
	char name[128];
	int age;
	int sex;        //成员属性
	int others;
	void (*peat)(); //成员方法
	void (*pbeat)();
	void (*ptest)();

	struct Animal *next;
};

/*
struct Animal data;   //对象1
struct Animal date;   //对象2
*/

struct Animal* putPersonInLink(struct Animal *phead);
struct Animal* putDogInLink(struct Animal *phead);
struct Animal* putCatInLink(struct Animal *phead);


