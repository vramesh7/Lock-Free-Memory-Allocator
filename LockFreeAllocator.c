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


static volatile descriptor* DescAvail;

//min request we can satisfy on malloc is 8 bytes and max = 2kB. 8 bytes overhead
sizeclass sizeclasslist[2048 / EIGHTBYTES] = 	{
				{{0,0}, 16, SBSIZE},
				{{0,0}, 24, SBSIZE}, {{0,0}, 32, SBSIZE},
				{{0,0}, 40, SBSIZE}, {{0,0}, 48, SBSIZE},
				{{0,0}, 56, SBSIZE}, {{0,0}, 64, SBSIZE},
				{{0,0}, 72, SBSIZE}, {{0,0}, 80, SBSIZE},
				{{0,0}, 88, SBSIZE}, {{0,0}, 96, SBSIZE},
				{{0,0}, 104, SBSIZE}, {{0,0}, 112, SBSIZE},
				{{0,0}, 120, SBSIZE}, {{0,0}, 128, SBSIZE},
				{{0,0}, 136, SBSIZE}, {{0,0}, 144, SBSIZE},
				{{0,0}, 152, SBSIZE}, {{0,0}, 160, SBSIZE},
				{{0,0}, 168, SBSIZE}, {{0,0}, 176, SBSIZE},
				{{0,0}, 184, SBSIZE}, {{0,0}, 192, SBSIZE},
				{{0,0}, 200, SBSIZE}, {{0,0}, 208, SBSIZE},
				{{0,0}, 216, SBSIZE}, {{0,0}, 224, SBSIZE},
				{{0,0}, 232, SBSIZE}, {{0,0}, 240, SBSIZE},
				{{0,0}, 248, SBSIZE}, {{0,0}, 256, SBSIZE},
				{{0,0}, 264, SBSIZE}, {{0,0}, 272, SBSIZE},
				{{0,0}, 280, SBSIZE}, {{0,0}, 288, SBSIZE},
				{{0,0}, 296, SBSIZE}, {{0,0}, 304, SBSIZE},
				{{0,0}, 312, SBSIZE}, {{0,0}, 320, SBSIZE},
				{{0,0}, 328, SBSIZE}, {{0,0}, 336, SBSIZE},
				{{0,0}, 344, SBSIZE}, {{0,0}, 352, SBSIZE},
				{{0,0}, 360, SBSIZE}, {{0,0}, 368, SBSIZE},
				{{0,0}, 376, SBSIZE}, {{0,0}, 384, SBSIZE},
				{{0,0}, 392, SBSIZE}, {{0,0}, 400, SBSIZE},
				{{0,0}, 408, SBSIZE}, {{0,0}, 416, SBSIZE},
				{{0,0}, 424, SBSIZE}, {{0,0}, 432, SBSIZE},
				{{0,0}, 440, SBSIZE}, {{0,0}, 448, SBSIZE},
				{{0,0}, 456, SBSIZE}, {{0,0}, 464, SBSIZE},
				{{0,0}, 472, SBSIZE}, {{0,0}, 480, SBSIZE},
				{{0,0}, 488, SBSIZE}, {{0,0}, 496, SBSIZE},
				{{0,0}, 504, SBSIZE}, {{0,0}, 512, SBSIZE},
				{{0,0}, 520, SBSIZE}, {{0,0}, 528, SBSIZE},
				{{0,0}, 536, SBSIZE}, {{0,0}, 544, SBSIZE},
				{{0,0}, 552, SBSIZE}, {{0,0}, 560, SBSIZE},
				{{0,0}, 568, SBSIZE}, {{0,0}, 576, SBSIZE},
				{{0,0}, 584, SBSIZE}, {{0,0}, 592, SBSIZE},
				{{0,0}, 600, SBSIZE}, {{0,0}, 608, SBSIZE},
				{{0,0}, 616, SBSIZE}, {{0,0}, 624, SBSIZE},
				{{0,0}, 632, SBSIZE}, {{0,0}, 640, SBSIZE},
				{{0,0}, 648, SBSIZE}, {{0,0}, 656, SBSIZE},
				{{0,0}, 664, SBSIZE}, {{0,0}, 672, SBSIZE},
				{{0,0}, 680, SBSIZE}, {{0,0}, 688, SBSIZE},
				{{0,0}, 696, SBSIZE}, {{0,0}, 704, SBSIZE},
				{{0,0}, 712, SBSIZE}, {{0,0}, 720, SBSIZE},
				{{0,0}, 728, SBSIZE}, {{0,0}, 736, SBSIZE},
				{{0,0}, 744, SBSIZE}, {{0,0}, 752, SBSIZE},
				{{0,0}, 760, SBSIZE}, {{0,0}, 768, SBSIZE},
				{{0,0}, 776, SBSIZE}, {{0,0}, 784, SBSIZE},
				{{0,0}, 792, SBSIZE}, {{0,0}, 800, SBSIZE},
				{{0,0}, 808, SBSIZE}, {{0,0}, 816, SBSIZE},
				{{0,0}, 824, SBSIZE}, {{0,0}, 832, SBSIZE},
				{{0,0}, 840, SBSIZE}, {{0,0}, 848, SBSIZE},
				{{0,0}, 856, SBSIZE}, {{0,0}, 864, SBSIZE},
				{{0,0}, 872, SBSIZE}, {{0,0}, 880, SBSIZE},
				{{0,0}, 888, SBSIZE}, {{0,0}, 896, SBSIZE},
				{{0,0}, 904, SBSIZE}, {{0,0}, 912, SBSIZE},
				{{0,0}, 920, SBSIZE}, {{0,0}, 928, SBSIZE},
				{{0,0}, 936, SBSIZE}, {{0,0}, 944, SBSIZE},
				{{0,0}, 952, SBSIZE}, {{0,0}, 960, SBSIZE},
				{{0,0}, 968, SBSIZE}, {{0,0}, 976, SBSIZE},
				{{0,0}, 984, SBSIZE}, {{0,0}, 992, SBSIZE},
				{{0,0}, 1000, SBSIZE}, {{0,0}, 1008, SBSIZE},
				{{0,0}, 1016, SBSIZE}, {{0,0}, 1024, SBSIZE},
				{{0,0}, 1032, SBSIZE}, {{0,0}, 1040, SBSIZE},
				{{0,0}, 1048, SBSIZE}, {{0,0}, 1056, SBSIZE},
				{{0,0}, 1064, SBSIZE}, {{0,0}, 1072, SBSIZE},
				{{0,0}, 1080, SBSIZE}, {{0,0}, 1088, SBSIZE},
				{{0,0}, 1096, SBSIZE}, {{0,0}, 1104, SBSIZE},
				{{0,0}, 1112, SBSIZE}, {{0,0}, 1120, SBSIZE},
				{{0,0}, 1128, SBSIZE}, {{0,0}, 1136, SBSIZE},
				{{0,0}, 1144, SBSIZE}, {{0,0}, 1152, SBSIZE},
				{{0,0}, 1160, SBSIZE}, {{0,0}, 1168, SBSIZE},
				{{0,0}, 1176, SBSIZE}, {{0,0}, 1184, SBSIZE},
				{{0,0}, 1192, SBSIZE}, {{0,0}, 1200, SBSIZE},
				{{0,0}, 1208, SBSIZE}, {{0,0}, 1216, SBSIZE},
				{{0,0}, 1224, SBSIZE}, {{0,0}, 1232, SBSIZE},
				{{0,0}, 1240, SBSIZE}, {{0,0}, 1248, SBSIZE},
				{{0,0}, 1256, SBSIZE}, {{0,0}, 1264, SBSIZE},
				{{0,0}, 1272, SBSIZE}, {{0,0}, 1280, SBSIZE},
				{{0,0}, 1288, SBSIZE}, {{0,0}, 1296, SBSIZE},
				{{0,0}, 1304, SBSIZE}, {{0,0}, 1312, SBSIZE},
				{{0,0}, 1320, SBSIZE}, {{0,0}, 1328, SBSIZE},
				{{0,0}, 1336, SBSIZE}, {{0,0}, 1344, SBSIZE},
				{{0,0}, 1352, SBSIZE}, {{0,0}, 1360, SBSIZE},
				{{0,0}, 1368, SBSIZE}, {{0,0}, 1376, SBSIZE},
				{{0,0}, 1384, SBSIZE}, {{0,0}, 1392, SBSIZE},
				{{0,0}, 1400, SBSIZE}, {{0,0}, 1408, SBSIZE},
				{{0,0}, 1416, SBSIZE}, {{0,0}, 1424, SBSIZE},
				{{0,0}, 1432, SBSIZE}, {{0,0}, 1440, SBSIZE},
				{{0,0}, 1448, SBSIZE}, {{0,0}, 1456, SBSIZE},
				{{0,0}, 1464, SBSIZE}, {{0,0}, 1472, SBSIZE},
				{{0,0}, 1480, SBSIZE}, {{0,0}, 1488, SBSIZE},
				{{0,0}, 1496, SBSIZE}, {{0,0}, 1504, SBSIZE},
				{{0,0}, 1512, SBSIZE}, {{0,0}, 1520, SBSIZE},
				{{0,0}, 1528, SBSIZE}, {{0,0}, 1536, SBSIZE},
				{{0,0}, 1544, SBSIZE}, {{0,0}, 1552, SBSIZE},
				{{0,0}, 1560, SBSIZE}, {{0,0}, 1568, SBSIZE},
				{{0,0}, 1576, SBSIZE}, {{0,0}, 1584, SBSIZE},
				{{0,0}, 1592, SBSIZE}, {{0,0}, 1600, SBSIZE},
				{{0,0}, 1608, SBSIZE}, {{0,0}, 1616, SBSIZE},
				{{0,0}, 1624, SBSIZE}, {{0,0}, 1632, SBSIZE},
				{{0,0}, 1640, SBSIZE}, {{0,0}, 1648, SBSIZE},
				{{0,0}, 1656, SBSIZE}, {{0,0}, 1664, SBSIZE},
				{{0,0}, 1672, SBSIZE}, {{0,0}, 1680, SBSIZE},
				{{0,0}, 1688, SBSIZE}, {{0,0}, 1696, SBSIZE},
				{{0,0}, 1704, SBSIZE}, {{0,0}, 1712, SBSIZE},
				{{0,0}, 1720, SBSIZE}, {{0,0}, 1728, SBSIZE},
				{{0,0}, 1736, SBSIZE}, {{0,0}, 1744, SBSIZE},
				{{0,0}, 1752, SBSIZE}, {{0,0}, 1760, SBSIZE},
				{{0,0}, 1768, SBSIZE}, {{0,0}, 1776, SBSIZE},
				{{0,0}, 1784, SBSIZE}, {{0,0}, 1792, SBSIZE},
				{{0,0}, 1800, SBSIZE}, {{0,0}, 1808, SBSIZE},
				{{0,0}, 1816, SBSIZE}, {{0,0}, 1824, SBSIZE},
				{{0,0}, 1832, SBSIZE}, {{0,0}, 1840, SBSIZE},
				{{0,0}, 1848, SBSIZE}, {{0,0}, 1856, SBSIZE},
				{{0,0}, 1864, SBSIZE}, {{0,0}, 1872, SBSIZE},
				{{0,0}, 1880, SBSIZE}, {{0,0}, 1888, SBSIZE},
				{{0,0}, 1896, SBSIZE}, {{0,0}, 1904, SBSIZE},
				{{0,0}, 1912, SBSIZE}, {{0,0}, 1920, SBSIZE},
				{{0,0}, 1928, SBSIZE}, {{0,0}, 1936, SBSIZE},
				{{0,0}, 1944, SBSIZE}, {{0,0}, 1952, SBSIZE},
				{{0,0}, 1960, SBSIZE}, {{0,0}, 1968, SBSIZE},
				{{0,0}, 1976, SBSIZE}, {{0,0}, 1984, SBSIZE},
				{{0,0}, 1992, SBSIZE}, {{0,0}, 2000, SBSIZE},
				{{0,0}, 2008, SBSIZE}, {{0,0}, 2016, SBSIZE},
				{{0,0}, 2024, SBSIZE}, {{0,0}, 2032, SBSIZE},
				{{0,0}, 2040, SBSIZE}, {{0,0}, 2048, SBSIZE},
				{{0,0}, 2056, SBSIZE}
				};
__thread procheap* heaps[2048 / EIGHTBYTES] = { };		//local to every thread

//Test functions for CAS:LOCK BASED

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

static void *AllocNewSB(unsigned int size){

	void* addr;
	if((addr = mmap(NULL,size,PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED){		
		//null => kernel chooses address for new mapping 
		printf("Memory error:Alloc New SB Fail: %e",errno);
		exit(1);
	}
	if(addr == NULL){
		printf("Memory Fail");
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
		((descriptor *)currptr)->Next = (descriptor *)nextptr;		//Connect the next pointers. Make list
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

//Note: ABA problem for desc?

static descriptor* DescAlloc(){
	descriptor* desc,next;
	while(1){
		desc = DescAvail;

		if(desc){
			next = desc->Next;
			if(CompareAndSwapWrapper(*(volatile unsigned long *)&DescAvail,*(unsigned long *)&desc,*(unsigned long *)&next))
				break;
		}

		else{

			desc = AllocNewSB(DESCSBSIZE);
			MakeDescList((void *)desc, DESCSBSIZE / sizeof(descriptor), sizeof(descriptor));

			if(CompareAndSwapWrapper(*(volatile unsigned long *)&DescAvail,*(unsigned long *)&desc,*(unsigned long *)&next))
				break;

			munmap((void*)desc, DESCSBSIZE); 
		}
		
	}
	
	return desc;
}

static void DescRetire(descriptor* desc){
	descriptor* oldhead;	
	//Insert desc at the head of the list
	do{
		oldhead = DescAvail;
		desc->Next = oldhead;

	}while(!CompareAndSwapWrapper(*(volatile unsigned long *)&DescAvail,*(unsigned long *)&oldhead,*(unsigned long *)&desc));

}


static descriptor* ListGetPartial(sizeclass* sc){
	return (descriptor *)Pop(sc->Partial);
}

static void ListPutPartial(descriptor* desc){
	 Push(&desc->heap->sc->Partial, (void*)desc);
}


static void HeapPutPartial(descriptor* desc){

	descriptor* prev;
	do{
		prev = (descriptor*)desc->heap->Partial;	
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


static void UpdateActive(procheap* heap, descriptor* desc, unsigned long morecredits){
	active newactive;
	anchor oldanchor,newanchor;

	newactive.ptr = (unsigned long)desc;	//pointer to desc in newanchor's ptr field
	newactive.credits = morecredits-1;

	if(CompareAndSwapWrapper((volatile unsigned long*)&heap->Active,(unsigned long)NULL,*(unsigned long *)&newactive))
		return ;	//return on success.
		
	//We are here=> couldnt install newactive as the active SB. There is another active SB in the heap. install this block as partial
	do{
		newanchor = oldanchor = desc->Anchor;
		newanchor.count += morecredits;
		newanchor.state = PARTIAL;
	}while(!CompareAndSwapWrapper((volatile unsigned long*)&desc->Anchor,*((unsigned long *)&oldanchor),*(unsigned long *)&newanchor));
	
	HeapPutPartial(desc);
		
}


descriptor* mask_credits(active Active){

	return (descriptor *)(Active.ptr);
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
		newactive=oldactive=heap->Active;			//active sb
		//return if no active sb present
		if(!(*(ull *)(&oldactive))) return NULL;		//Check if we have an active block
		
		if(oldactive.credits == 0){				//No blocks present in sb. set active sb to null
			*((ull *)(&newactive)) = 0;
		}
		else{
			newactive.credits--;
		}

	}while(!CompareAndSwapWrapper((volatile unsigned long*)&heap->Active,*((unsigned long*)&oldactive,*((unsigned long*)&newactive));
	
	 
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

	}while(!CompareAndSwapWrapper((volatile unsigned long*)&heap->Active,*((unsigned long*)&oldactive,*((unsigned long*)&newactive));
	
	if((oldactive.credits == 0) && (oldanchor.count > 0))	//get morecredits since number of blocs != 0
		UpdateActive(heap,desc,morecredits);
	
	*(descriptor **)addr = desc;
	*(descriptor **)addr |= 0x1;	//encode. this is a small block.
	return (void *)((unsigned long)addr+EIGHTBYTES);
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
	*(descriptor **)addr |= 0x1;		//encode. this is a small block.
	return addr+EIGHTBYTES;
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
		*(descriptor **)addr |= 0x1;	//encode. this is a small block.
		return addr + EIGHTBYTES;
	}
	else{
		//free the SB desc->sb
		munmap(desc->sb,desc->heap->sc->sbsize);
		DescRetire(desc);
		return NULL;
	}

}

void* MallocLargeBlock(unsigned int sz){

	void* addr;
	if((addr = mmap(NULL,sz + EIGHTBYTES,PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED){		
		//null => kernel chooses address for new mapping 
		printf("Memory error:Alloc Large Block Fail: %e",errno);
		exit(1);
	}
	*(unsigned long *)addr = sz + EIGHTBYTES;		//Insert size in the header
	*(unsigned long *)addr |= 0x0;		//encode. this is a large block.
	return (void *)((unsigned long)addr + EIGHTBYTES);
	
}


static procheap* find_heap(unsigned int sz){

	procheap* heap;
  
	sz += EIGHTBYTES;
	if (sz > 2056) {
		return NULL;
	}

	heap = heaps[(sz/EIGHTBYTES)-1];

	if(heap == NULL){		//First time access to heap
		if((heap = mmap(NULL,sz + EIGHTBYTES,PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) ==MAP_FAILED) 		{		
			//null => kernel chooses address for new mapping 
			printf("Memory error:heap Fail: %e",errno);
			exit(1);
		}

		*((ull*)&(heap->Active)) = 0;
		heap->Partial = NULL;
		heap->sc = &sizeclasses[(sz /EIGHTBYES)-1];
		heaps[(sz / EIGHTBYTES)-1] = heap;

	}

	return heap;

}

void *malloc(unsigned int sz){
	
	
	procheap* heap = find_heap(sz);
	printf("Malloc request:size:%d::heap:%d",sz,(unsigned int)heap);
	if(!heap){
		//Allocate block from OS and return its address
		addr = MallocLargeBlock(sz);
		return addr;
	}

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


void free(void* ptr){
	descriptor* desc;
	procheap* heap = NULL;
	anchor newanchor,oldanchor;
	if(!ptr) return;	//if NULL

	ptr = (void*)((unsigned long)ptr - HEADER_SIZE);;		//get prefix
	desc =  BLOCKDESC(*(descriptor **)ptr);	// descriptor of the block

	if(BLOCKSTAT(*(unsigned long*)ptr) == 0){		//large block -desc holds sz+1
		munmap(ptr, BLOCKDESC(*((unsigned long *)(ptr))));
	}
	//small block
	void* sb = desc->sb;
	do{
		newanchor = oldanchor = desc->Anchor;
		*((unsigned long *)(ptr)) = oldanchor.avail;
		newanchor.avail = ((unsigned long)ptr - (unsigned long)sb) / desc->sz;
		if(oldanchor.state == FULL){
			newanchor.state = PARTIAL;
		}
	
		if(oldanchor.count == desc->maxcount-1){
			heap = desc->heap;
			newanchor.state = EMPTY;

		}
		else{
			newanchor.count++;
		}

	}while(!CompareAndSwapWrapper((volatile unsigned long*)&desc->Anchor,
					*((unsigned long *)&oldanchor),*(unsigned long *)&newanchor));	

	if(newanchor.state == EMPTY){	//SB free to be returned to OS
		munmap(sb,heap->sc->sbsize);
		if (CompareAndSwapWrapper(&heap->Partial, desc, NULL)) {
			DescRetire(desc);
		}	
	}

	}else if(oldanchor.state == FULL){
		
		HeapPutPartial(desc);
	}

}


