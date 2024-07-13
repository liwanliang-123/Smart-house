#include "Animal.h"

void personEat()
{
	printf("person eat mi\n");
}

void personBeat()
{
	printf("person do\n");
}

struct Animal person = {
	.name = "person",
	.peat = personEat,
	.pbeat = personBeat
};

struct Animal* putPersonInLink(struct Animal *phead)
{
	if (phead == NULL){
		phead = &person;
			
	}else{
		person.next = phead;
		phead = &person;
		return phead;
	}
}



