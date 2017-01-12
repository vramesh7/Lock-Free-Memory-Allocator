#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "LockFreeAllocator.h"

#define EIGHTBYTES	sizeof(char *)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static void UpdateActive(procheap* heap, descriptor* desc, unsigned long morecredits);
int CompareAndSwap(uintptr_t* add, int old_val, int new_val);
int CompareAndSwapWrapper(uintptr_t* add, int old_val, int new_val);
void* MallocFromActive(procheap *heap);

//Test functions for CAS
int CompareAndSwapWrapper(uintptr_t* add, int old_val, int new_val){
	int value;
	pthread_mutex_lock(&mutex);
	value = CompareAndSwap(uintptr_t* add, int old_val, int new_val);
	pthread_mutex_unlock(&mutex);
	return value;
}


int CompareAndSwap(uintptr_t* add, int old_val, int new_val)
{
	int value = *add
	if(value == old_val)
		*add = new_val;
	return value;

}

void *malloc(unsigned int sz){
	
	procheap* heap = find_heap(sz);

	if(!heap)
		//Allocate block from OS and return its address

	while(1){

		addr = MallocFromActive(heap);
		if(addr) 
			return addr;
		addr=  MallocFromPartial(heap);
		if(addr) 
			return addr;
		addr = MallocFromNewSB(heap);
		if(addr) 
			return addr;

	}

	
}


/*
 * MallocFromActive: 2 step operation:  1. reading active superblock from heap, updating credits field => reserving a block
 *					2. Lockfree Pop from a LIFO list of superblocks
 */

void* MallocFromActive(procheap *heap){

	active newactive,oldactive;
	void* addr;
	descriptor* desc;
	unsigned long morecredits = 0;
	unsigned int next = 0;
	anchor newanchor,oldanchor;
	
	//step 1: block reserve
	do{
		newactive=oldactive=heap->active;			//active sb
		//return if no active sb present
		if(!(*(ull *)(&oldactive))) return NULL;		//since a struct can never be null
		
		if(oldactive.credits == 0){				//No blocks present in sb. set active sb to null
			*((ull *)(&newactive)) = NULL;
		}
		else{
			newactive.credits--;
		}

	}while(!CompareAndSwapWrapper(&heap->active,oldactive,newactive));
	
	 
	//step 2: LF pop from list
	//init desc
	desc = (descriptor *)oldcredits.ptr;	//Extract the prtr field from oldcredits

	do{
		
		newanchor = oldanchor = desc->Anchor;
		addr = (void *)((unsigned long)desc->sb + oldanchor.avail * desc->sz);
		next = *(unsigned long *)addr;	
		newanchor.avail = next;
		newanchor.tag++;

	
		if(oldactive.credits == 0){
			if(oldanchor.count == 0)
				newanchor.state = FULL;
			else{
				morecredits = MIN(oldanchor.count,MAXCREDITS);
				newanchor.count -= morecredits;
			}
		}

	}while(!CompareAndSwapWrapper(&heap->active,oldactive,newactive));
	
	if((oldactive.credits == 0) && (oldanchor.count > 0))	//get morecredits since number of blocs != 0
		UpdateActive(heap,desc,morecredits);
	
	*(descriptor **)addr = desc;
	return (void *)((unsigned long)addr+EIGHTBYTES);
}


descriptor* mask_credits(active Active){

	return (descriptor *)(Active.ptr);
}


static void UpdateActive(procheap* heap, descriptor* desc, unsigned long morecredits){
	active newactive;
	anchor oldanchor,newanchor;

	newanchor.ptr = (unsigned long)desc;	//pointer to desc in newanchor's ptr field
	newactive.credits = morecredits-1;

	if(CompareAndSwapWrapper((volatile unsigned long*)&heap->active,(unsigned long)NULL,*(unsigned long *)&newactive))
		return ;	//return on success.
		
	//We are here=> couldnt install newactive as the active SB. There is another active SB in the heap. install this block as partial
	do{
	newanchor = oldanchor = desc->Anchor;
	newanchor.count += morecredits;
	newanchor.state = PARTIAL;
	}while(!CompareAndSwapWrapper((volatile unsigned long*)&desc->Anchor,*((unsigned long *)&oldanchor),*(unsigned long *)&newanchor));
	
	HeapPutPartial(desc);
		

}



static descriptor* ListGetPartial(sizeclass* sc){
	
}

static void ListPutPartial(descriptor* desc){
	 
}


static void HeapPutPartial(descriptor* desc){

	descriptor* prev;
	do{
		prev = desc->heap->Partial;	
	}while(!CompareAndSwapWrapper(&desc->heap->Partial,prev,desc));

	if(prev)	
		ListPutPartial(prev);		
}

static descriptor *HeapGetPartial(procheap *heap){
	descriptor* desc;
	do{
		desc = heap->Partial;
		if(desc == NULL)
			return ListGetPartial(heap->sc);	//head of linked list. If no partial SB
	}while(!CompareAndSwapWrapper(&heap->Partial,desc,NULL));

	return desc;

} 

static void *MallocFromPartial(procheap *heap){

	descriptor* desc;
	unsigned long morecredits = 0;
	anchor oldanchor,newanchor;
	void* addr;

	retry:
	desc = HeapGetPartial(heap);
	if(!desc) return NULL;
	desc->heap = heap;
	do{
		newanchor = oldanchor = desc->Anchor;
		if(oldanchor.state == EMPTY){		//Retire the desc if SB is empty. Someother thread makes SB empty!
			DescRetire(desc);
			goto retry;
		}
		//oldanchor state must be partial
		//oldanchor count must be > 0
		//reserve a block
		morecredits = MIN(oldanchor.count-1,MAXCREDITS);
		newanchor.count -= morecredits+1;
		newanchor.state = (morecredits > 0)?ACTIVE:FULL;
	
	}while(!CompareAndSwapWrapper((volatile unsigned long*)&desc->Anchor,
					*((unsigned long *)&oldanchor),*(unsigned long *)&newanchor));	

	do{	//pop reserved block
		newanchor =oldanchor = desc->Anchor;
		addr = (void *)((unsigned long)desc->sb + oldanchor.avail * desc->sz);
		newanchor.avail = *(unsigned long *)addr;
		newanchor.tag++;

	}while(!CompareAndSwapWrapper((volatile unsigned long*)&desc->Anchor,
					*((unsigned long *)&oldanchor),*(unsigned long *)&newanchor));	
	if(morecredits > 0)
		UpdateActive(heap,desc,morecredits);

	*((descriptor **)addr) = desc;		//pointer to a descriptor

	return addr+EIGHTBYTES;
}

static void *AllocNewSB(unsigned int size){

	void* addr;
	if((addr = mmap(NULL,size,PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED){		
		//null => kernel chooses address for new mapping 
		printf("Memory error:Alloc Fail: %e",errno);
		exit(1);
	}
	
	return addr;
}

static void MakeDescList(void* sb, unsigned long maxcount, unsigned long sz){

	unsigned int i;
	unsigned int currptr,nxtptr;
	currptr = (unsigned long)sb;
	for(i=0;i<maxcount-1;i++){
		currptr += i*sz;
		nextptr += (i+1)*sz;
		((descriptor *)currptr)->Next = (descriptor *)nextptr;
	}
	currptr = nextptr;
	((descriptor *)currptr)->Next = NULL;
}	

static void MakeList(void* sb, unsigned long maxcount, unsigned long sz){

	unsigned int i;
	unsigned int currptr;
	currptr = (unsigned long)sb;
	for(i=0;i<maxcount-1;i++){
		currptr += i*sz;
		*((unsigned long *)currptr) = i+1;		//block number
	}
	
}

static void *MallocFromNewSB(procheap *heap){

	descriptor* desc;

	active newactive;
	void* addr;
	
	desc = DescAlloc();
	desc->sb = AllocNewSB(heap->sc->sbsize);
	//Organize blocks in a linked list starting with index 0.
	MakeList(desc->sb,desc->maxcount,desc->sz);
	desc->heap = heap;
	desc->Anchor.avail =1;
	desc->sz=heap->sc->sz;
	desc->maxcount = heap->sc->sbsize/desc->sz;
	
	*((ull*)&newactive) = 0;		//wipe out newactive to 0.
	newactive.ptr = (unsigned long)desc;
	newactive.credits = MIN(desc->maxcount-1,MAXCREDITS)-1;
	desc->Anchor.count = (desc->maxcount-1)-(newactive.credits+1);
	desc->Anchor.state = ACTIVE;
	
	//memory fence.Volatile!
	if(!CompareAndSwapWrapper((volatile unsigned long*)&heap->Active,NULL,*(unsigned long *)&newactive)){
		addr = desc->sb;
		*(descriptor **)addr = desc;
		return addr + EIGHTBYTES;
	}
	else{
		//free the SB desc->sb
		munmap(desc->sb,desc->heap->sc->sbsize);
		DescRetire(desc);
		return NULL;
	}

}

