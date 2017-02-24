#include <stdlib.h>

typedef struct {
	volatile  unsigned long long head:48, tag:16;		//head is stack_elem. tag fixes aba problem 

}stack_top;

struct stack_elem_t{
	volatile struct stack_elem_t* next;

}

typedef struct{
	stack_top stacktop;

}stack_t;


static Push(stack_t* Stack,void* element){

	stack_top oldtop;
	stack_top newtop;
	
	do{
		oldtop.head = Stack->stacktop.head;
		oldtop.tag = Stack->stacktop.tag;
	
		((struct stack_elem_t *)element)->next = ((struct stack_elem_t *)oldtop.head);
		newtop.head = (unsigned long long)element;		//insert new element as the head	
		newtop.tag = oldtop.tag + 1;
	}while(!__sync_val_compare_and_swap((volatile unsigned long long*)&(Stack->stacktop),
					 *((unsigned long long*)&oldtop), *((unsigned long long*)&newtop)));

}

//
static void* Pop(stack_t* Stack){

	stack_top oldtop;
	stack_top newtop;
	
	do{
		oldtop.head = Stack->stacktop.head;
		oldtop.tag = Stack->stacktop.tag;
		
		//if empty stack
		if(oldtop.head == 0)	return NULL;

		newtop.head = ((struct stack_elem_t *)oldtop.head)->next;		//pop element at head	
		newtop.tag = oldtop.tag + 1;
	}while(!__sync_val_compare_and_swap((volatile unsigned long long*)&(Stack->stacktop),
					 *((unsigned long long*)&oldtop), *((unsigned long long*)&newtop)));

	return (void *)oldtop;

}
