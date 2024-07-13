#include "Animal.h"

struct Animal* findUtilByName(char *str,struct Animal* phead)
{
	struct Animal* tmp = phead;
	if(phead == NULL){
		printf("NULL\n");
		return NULL;
	}else{
		while(tmp != NULL){
			if(strcmp(tmp->name,str) == 0){
				return tmp;
			}
			tmp = tmp->next;
		}
		return NULL;
	}
}

int main()
{
	char buf[128] = {'\0'};
	struct Animal *ptmp;
	struct Animal *phead = NULL;        //链表头节点
	phead = putCatInLink(phead);        //当前phead为cat
	phead = putDogInLink(phead);        //当前phead为dog,dog的下一个为cat
	phead = putPersonInLink(phead);     //当前phead为person,person的下一个为dog
										//当前头节点为person
	while(1){
		printf("please Input Tom or dog or person\n");
		scanf("%s",buf);

		ptmp = findUtilByName(buf,phead);  //去找和输入一样的字段
		if(ptmp != NULL){
			ptmp->pbeat();                
			ptmp->peat();
		}
		memset(buf,'\0',sizeof(buf));
	}
	return 0;
}

