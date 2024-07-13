#include "Animal.h"

void dogEat()
{
	printf("dog eat si\n");
}

void dogBeat()
{
	printf("dog do\n");
}

struct Animal dog = {
	.name = "dog",
	.peat = dogEat,
	.pbeat = dogBeat
};

struct Animal* putDogInLink(struct Animal *phead)
{
	if (phead == NULL){
		phead = &dog;
			
	}else{
		dog.next = phead;
		phead = &dog;
		return phead;
	}


}



