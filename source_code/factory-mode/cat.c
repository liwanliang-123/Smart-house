#include "Animal.h"

void catEat()
{
	printf("cat eat filash\n");
}

void catBeat()
{
	printf("cat do\n");
}

struct Animal cat = {
	.name = "Tom",        //实例化对象
	.peat = catEat,
	.pbeat = catBeat
};

struct Animal* putCatInLink(struct Animal *phead)
{
	if (phead == NULL){     //如果头为空
		phead = &cat;       //将cat作为链表头
			
	}else{
		cat.next = phead;   //将当前头节点放到cat—>next
		phead = &cat;       //让头节点等于cat
		return phead;       //返回头节点
	}

}



