
#include "LockFreeAllocator.h"

//Static initialization of superblocks of various size classes
sizeclass sizeclasses[2048 / GRANULARITY] =
				{
				{LF_QUEUE_INIT, 8, SBSIZE}, {LF_QUEUE_INIT, 16, SBSIZE},
				{LF_QUEUE_INIT, 24, SBSIZE}, {LF_QUEUE_INIT, 32, SBSIZE},
				{LF_QUEUE_INIT, 40, SBSIZE}, {LF_QUEUE_INIT, 48, SBSIZE},
				{LF_QUEUE_INIT, 56, SBSIZE}, {LF_QUEUE_INIT, 64, SBSIZE},
				{LF_QUEUE_INIT, 72, SBSIZE}, {LF_QUEUE_INIT, 80, SBSIZE},
				{LF_QUEUE_INIT, 88, SBSIZE}, {LF_QUEUE_INIT, 96, SBSIZE},
				{LF_QUEUE_INIT, 104, SBSIZE}, {LF_QUEUE_INIT, 112, SBSIZE},
				{LF_QUEUE_INIT, 120, SBSIZE}, {LF_QUEUE_INIT, 128, SBSIZE},
				{LF_QUEUE_INIT, 136, SBSIZE}, {LF_QUEUE_INIT, 144, SBSIZE},
				{LF_QUEUE_INIT, 152, SBSIZE}, {LF_QUEUE_INIT, 160, SBSIZE},
				{LF_QUEUE_INIT, 168, SBSIZE}, {LF_QUEUE_INIT, 176, SBSIZE},
				{LF_QUEUE_INIT, 184, SBSIZE}, {LF_QUEUE_INIT, 192, SBSIZE},
				{LF_QUEUE_INIT, 200, SBSIZE}, {LF_QUEUE_INIT, 208, SBSIZE},
				{LF_QUEUE_INIT, 216, SBSIZE}, {LF_QUEUE_INIT, 224, SBSIZE},
				{LF_QUEUE_INIT, 232, SBSIZE}, {LF_QUEUE_INIT, 240, SBSIZE},
				{LF_QUEUE_INIT, 248, SBSIZE}, {LF_QUEUE_INIT, 256, SBSIZE},
				{LF_QUEUE_INIT, 264, SBSIZE}, {LF_QUEUE_INIT, 272, SBSIZE},
				{LF_QUEUE_INIT, 280, SBSIZE}, {LF_QUEUE_INIT, 288, SBSIZE},
				{LF_QUEUE_INIT, 296, SBSIZE}, {LF_QUEUE_INIT, 304, SBSIZE},
				{LF_QUEUE_INIT, 312, SBSIZE}, {LF_QUEUE_INIT, 320, SBSIZE},
				{LF_QUEUE_INIT, 328, SBSIZE}, {LF_QUEUE_INIT, 336, SBSIZE},
				{LF_QUEUE_INIT, 344, SBSIZE}, {LF_QUEUE_INIT, 352, SBSIZE},
				{LF_QUEUE_INIT, 360, SBSIZE}, {LF_QUEUE_INIT, 368, SBSIZE},
				{LF_QUEUE_INIT, 376, SBSIZE}, {LF_QUEUE_INIT, 384, SBSIZE},
				{LF_QUEUE_INIT, 392, SBSIZE}, {LF_QUEUE_INIT, 400, SBSIZE},
				{LF_QUEUE_INIT, 408, SBSIZE}, {LF_QUEUE_INIT, 416, SBSIZE},
				{LF_QUEUE_INIT, 424, SBSIZE}, {LF_QUEUE_INIT, 432, SBSIZE},
				{LF_QUEUE_INIT, 440, SBSIZE}, {LF_QUEUE_INIT, 448, SBSIZE},
				{LF_QUEUE_INIT, 456, SBSIZE}, {LF_QUEUE_INIT, 464, SBSIZE},
				{LF_QUEUE_INIT, 472, SBSIZE}, {LF_QUEUE_INIT, 480, SBSIZE},
				{LF_QUEUE_INIT, 488, SBSIZE}, {LF_QUEUE_INIT, 496, SBSIZE},
				{LF_QUEUE_INIT, 504, SBSIZE}, {LF_QUEUE_INIT, 512, SBSIZE},
				{LF_QUEUE_INIT, 520, SBSIZE}, {LF_QUEUE_INIT, 528, SBSIZE},
				{LF_QUEUE_INIT, 536, SBSIZE}, {LF_QUEUE_INIT, 544, SBSIZE},
				{LF_QUEUE_INIT, 552, SBSIZE}, {LF_QUEUE_INIT, 560, SBSIZE},
				{LF_QUEUE_INIT, 568, SBSIZE}, {LF_QUEUE_INIT, 576, SBSIZE},
				{LF_QUEUE_INIT, 584, SBSIZE}, {LF_QUEUE_INIT, 592, SBSIZE},
				{LF_QUEUE_INIT, 600, SBSIZE}, {LF_QUEUE_INIT, 608, SBSIZE},
				{LF_QUEUE_INIT, 616, SBSIZE}, {LF_QUEUE_INIT, 624, SBSIZE},
				{LF_QUEUE_INIT, 632, SBSIZE}, {LF_QUEUE_INIT, 640, SBSIZE},
				{LF_QUEUE_INIT, 648, SBSIZE}, {LF_QUEUE_INIT, 656, SBSIZE},
				{LF_QUEUE_INIT, 664, SBSIZE}, {LF_QUEUE_INIT, 672, SBSIZE},
				{LF_QUEUE_INIT, 680, SBSIZE}, {LF_QUEUE_INIT, 688, SBSIZE},
				{LF_QUEUE_INIT, 696, SBSIZE}, {LF_QUEUE_INIT, 704, SBSIZE},
				{LF_QUEUE_INIT, 712, SBSIZE}, {LF_QUEUE_INIT, 720, SBSIZE},
				{LF_QUEUE_INIT, 728, SBSIZE}, {LF_QUEUE_INIT, 736, SBSIZE},
				{LF_QUEUE_INIT, 744, SBSIZE}, {LF_QUEUE_INIT, 752, SBSIZE},
				{LF_QUEUE_INIT, 760, SBSIZE}, {LF_QUEUE_INIT, 768, SBSIZE},
				{LF_QUEUE_INIT, 776, SBSIZE}, {LF_QUEUE_INIT, 784, SBSIZE},
				{LF_QUEUE_INIT, 792, SBSIZE}, {LF_QUEUE_INIT, 800, SBSIZE},
				{LF_QUEUE_INIT, 808, SBSIZE}, {LF_QUEUE_INIT, 816, SBSIZE},
				{LF_QUEUE_INIT, 824, SBSIZE}, {LF_QUEUE_INIT, 832, SBSIZE},
				{LF_QUEUE_INIT, 840, SBSIZE}, {LF_QUEUE_INIT, 848, SBSIZE},
				{LF_QUEUE_INIT, 856, SBSIZE}, {LF_QUEUE_INIT, 864, SBSIZE},
				{LF_QUEUE_INIT, 872, SBSIZE}, {LF_QUEUE_INIT, 880, SBSIZE},
				{LF_QUEUE_INIT, 888, SBSIZE}, {LF_QUEUE_INIT, 896, SBSIZE},
				{LF_QUEUE_INIT, 904, SBSIZE}, {LF_QUEUE_INIT, 912, SBSIZE},
				{LF_QUEUE_INIT, 920, SBSIZE}, {LF_QUEUE_INIT, 928, SBSIZE},
				{LF_QUEUE_INIT, 936, SBSIZE}, {LF_QUEUE_INIT, 944, SBSIZE},
				{LF_QUEUE_INIT, 952, SBSIZE}, {LF_QUEUE_INIT, 960, SBSIZE},
				{LF_QUEUE_INIT, 968, SBSIZE}, {LF_QUEUE_INIT, 976, SBSIZE},
				{LF_QUEUE_INIT, 984, SBSIZE}, {LF_QUEUE_INIT, 992, SBSIZE},
				{LF_QUEUE_INIT, 1000, SBSIZE}, {LF_QUEUE_INIT, 1008, SBSIZE},
				{LF_QUEUE_INIT, 1016, SBSIZE}, {LF_QUEUE_INIT, 1024, SBSIZE},
				{LF_QUEUE_INIT, 1032, SBSIZE}, {LF_QUEUE_INIT, 1040, SBSIZE},
				{LF_QUEUE_INIT, 1048, SBSIZE}, {LF_QUEUE_INIT, 1056, SBSIZE},
				{LF_QUEUE_INIT, 1064, SBSIZE}, {LF_QUEUE_INIT, 1072, SBSIZE},
				{LF_QUEUE_INIT, 1080, SBSIZE}, {LF_QUEUE_INIT, 1088, SBSIZE},
				{LF_QUEUE_INIT, 1096, SBSIZE}, {LF_QUEUE_INIT, 1104, SBSIZE},
				{LF_QUEUE_INIT, 1112, SBSIZE}, {LF_QUEUE_INIT, 1120, SBSIZE},
				{LF_QUEUE_INIT, 1128, SBSIZE}, {LF_QUEUE_INIT, 1136, SBSIZE},
				{LF_QUEUE_INIT, 1144, SBSIZE}, {LF_QUEUE_INIT, 1152, SBSIZE},
				{LF_QUEUE_INIT, 1160, SBSIZE}, {LF_QUEUE_INIT, 1168, SBSIZE},
				{LF_QUEUE_INIT, 1176, SBSIZE}, {LF_QUEUE_INIT, 1184, SBSIZE},
				{LF_QUEUE_INIT, 1192, SBSIZE}, {LF_QUEUE_INIT, 1200, SBSIZE},
				{LF_QUEUE_INIT, 1208, SBSIZE}, {LF_QUEUE_INIT, 1216, SBSIZE},
				{LF_QUEUE_INIT, 1224, SBSIZE}, {LF_QUEUE_INIT, 1232, SBSIZE},
				{LF_QUEUE_INIT, 1240, SBSIZE}, {LF_QUEUE_INIT, 1248, SBSIZE},
				{LF_QUEUE_INIT, 1256, SBSIZE}, {LF_QUEUE_INIT, 1264, SBSIZE},
				{LF_QUEUE_INIT, 1272, SBSIZE}, {LF_QUEUE_INIT, 1280, SBSIZE},
				{LF_QUEUE_INIT, 1288, SBSIZE}, {LF_QUEUE_INIT, 1296, SBSIZE},
				{LF_QUEUE_INIT, 1304, SBSIZE}, {LF_QUEUE_INIT, 1312, SBSIZE},
				{LF_QUEUE_INIT, 1320, SBSIZE}, {LF_QUEUE_INIT, 1328, SBSIZE},
				{LF_QUEUE_INIT, 1336, SBSIZE}, {LF_QUEUE_INIT, 1344, SBSIZE},
				{LF_QUEUE_INIT, 1352, SBSIZE}, {LF_QUEUE_INIT, 1360, SBSIZE},
				{LF_QUEUE_INIT, 1368, SBSIZE}, {LF_QUEUE_INIT, 1376, SBSIZE},
				{LF_QUEUE_INIT, 1384, SBSIZE}, {LF_QUEUE_INIT, 1392, SBSIZE},
				{LF_QUEUE_INIT, 1400, SBSIZE}, {LF_QUEUE_INIT, 1408, SBSIZE},
				{LF_QUEUE_INIT, 1416, SBSIZE}, {LF_QUEUE_INIT, 1424, SBSIZE},
				{LF_QUEUE_INIT, 1432, SBSIZE}, {LF_QUEUE_INIT, 1440, SBSIZE},
				{LF_QUEUE_INIT, 1448, SBSIZE}, {LF_QUEUE_INIT, 1456, SBSIZE},
				{LF_QUEUE_INIT, 1464, SBSIZE}, {LF_QUEUE_INIT, 1472, SBSIZE},
				{LF_QUEUE_INIT, 1480, SBSIZE}, {LF_QUEUE_INIT, 1488, SBSIZE},
				{LF_QUEUE_INIT, 1496, SBSIZE}, {LF_QUEUE_INIT, 1504, SBSIZE},
				{LF_QUEUE_INIT, 1512, SBSIZE}, {LF_QUEUE_INIT, 1520, SBSIZE},
				{LF_QUEUE_INIT, 1528, SBSIZE}, {LF_QUEUE_INIT, 1536, SBSIZE},
				{LF_QUEUE_INIT, 1544, SBSIZE}, {LF_QUEUE_INIT, 1552, SBSIZE},
				{LF_QUEUE_INIT, 1560, SBSIZE}, {LF_QUEUE_INIT, 1568, SBSIZE},
				{LF_QUEUE_INIT, 1576, SBSIZE}, {LF_QUEUE_INIT, 1584, SBSIZE},
				{LF_QUEUE_INIT, 1592, SBSIZE}, {LF_QUEUE_INIT, 1600, SBSIZE},
				{LF_QUEUE_INIT, 1608, SBSIZE}, {LF_QUEUE_INIT, 1616, SBSIZE},
				{LF_QUEUE_INIT, 1624, SBSIZE}, {LF_QUEUE_INIT, 1632, SBSIZE},
				{LF_QUEUE_INIT, 1640, SBSIZE}, {LF_QUEUE_INIT, 1648, SBSIZE},
				{LF_QUEUE_INIT, 1656, SBSIZE}, {LF_QUEUE_INIT, 1664, SBSIZE},
				{LF_QUEUE_INIT, 1672, SBSIZE}, {LF_QUEUE_INIT, 1680, SBSIZE},
				{LF_QUEUE_INIT, 1688, SBSIZE}, {LF_QUEUE_INIT, 1696, SBSIZE},
				{LF_QUEUE_INIT, 1704, SBSIZE}, {LF_QUEUE_INIT, 1712, SBSIZE},
				{LF_QUEUE_INIT, 1720, SBSIZE}, {LF_QUEUE_INIT, 1728, SBSIZE},
				{LF_QUEUE_INIT, 1736, SBSIZE}, {LF_QUEUE_INIT, 1744, SBSIZE},
				{LF_QUEUE_INIT, 1752, SBSIZE}, {LF_QUEUE_INIT, 1760, SBSIZE},
				{LF_QUEUE_INIT, 1768, SBSIZE}, {LF_QUEUE_INIT, 1776, SBSIZE},
				{LF_QUEUE_INIT, 1784, SBSIZE}, {LF_QUEUE_INIT, 1792, SBSIZE},
				{LF_QUEUE_INIT, 1800, SBSIZE}, {LF_QUEUE_INIT, 1808, SBSIZE},
				{LF_QUEUE_INIT, 1816, SBSIZE}, {LF_QUEUE_INIT, 1824, SBSIZE},
				{LF_QUEUE_INIT, 1832, SBSIZE}, {LF_QUEUE_INIT, 1840, SBSIZE},
				{LF_QUEUE_INIT, 1848, SBSIZE}, {LF_QUEUE_INIT, 1856, SBSIZE},
				{LF_QUEUE_INIT, 1864, SBSIZE}, {LF_QUEUE_INIT, 1872, SBSIZE},
				{LF_QUEUE_INIT, 1880, SBSIZE}, {LF_QUEUE_INIT, 1888, SBSIZE},
				{LF_QUEUE_INIT, 1896, SBSIZE}, {LF_QUEUE_INIT, 1904, SBSIZE},
				{LF_QUEUE_INIT, 1912, SBSIZE}, {LF_QUEUE_INIT, 1920, SBSIZE},
				{LF_QUEUE_INIT, 1928, SBSIZE}, {LF_QUEUE_INIT, 1936, SBSIZE},
				{LF_QUEUE_INIT, 1944, SBSIZE}, {LF_QUEUE_INIT, 1952, SBSIZE},
				{LF_QUEUE_INIT, 1960, SBSIZE}, {LF_QUEUE_INIT, 1968, SBSIZE},
				{LF_QUEUE_INIT, 1976, SBSIZE}, {LF_QUEUE_INIT, 1984, SBSIZE},
				{LF_QUEUE_INIT, 1992, SBSIZE}, {LF_QUEUE_INIT, 2000, SBSIZE},
				{LF_QUEUE_INIT, 2008, SBSIZE}, {LF_QUEUE_INIT, 2016, SBSIZE},
				{LF_QUEUE_INIT, 2024, SBSIZE}, {LF_QUEUE_INIT, 2032, SBSIZE},
				{LF_QUEUE_INIT, 2040, SBSIZE}, {LF_QUEUE_INIT, 2048, SBSIZE},
				};

__thread procheap* heaps[2048 / GRANULARITY] =	{ };		//Thraed local heap

static volatile unsigned long long DescAvail;

static inline long min(long a, long b)
{
	return a < b ? a : b;
}

static inline long max(long a, long b)
{
	return a > b ? a : b;
}
//helper function to allocate new SB through OS support
static void* AllocNewSB(size_t size, unsigned long alignement)
{
	
	void* addr;
	if((addr = mmap(NULL,size,PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED){		
		//null => kernel chooses address for new mapping 
		#ifdef DEBUG
		printf("Memory error:Alloc New SB Fail: %e",errno);
		#endif
		exit(1);
	}
	if(addr == NULL){
		printf("Memory Fail");
		exit(1);
	}
		
	
	return addr;

}



//function to organize descriptors in a linked list

static void organize_desc_list(void* start, unsigned long count, unsigned long stride)
{
	unsigned int i;
	unsigned long currptr,nxtptr;
	currptr = (unsigned long)sb;
	for(i=0;i<maxcount-1;i++){
		currptr += i*sz;
		nextptr += (i+1)*sz;
		((descriptor *)currptr)->Next = (descriptor *)nextptr;		//Connect the next pointers. Make list
	}
	currptr = nextptr;
	((descriptor *)currptr)->Next = NULL;
}

//function to organize 'sizeclass' sized blocks of a SB in a linked list
static void organize_list(void* start, unsigned long count, unsigned long stride)
{
	unsigned int i;
	unsigned long currptr;
	currptr = (unsigned long)sb;
	for(i=0;i<maxcount-1;i++){
		currptr += i*sz;
		*((unsigned long *)currptr) = i+1;		//block number
	}
}

//function to allocate descriptors
static descriptor* DescAlloc() {
  
	unsigned long long oldAvail, newAvail;
	descriptor* desc;
  
	#ifdef DEBUG
	printf("In DescAlloc\n");
	
	#endif

	while(1) {
		oldAvail = DescAvail;
		#ifdef DEBUG
			printf("old_queue.DescAvail : % llu   %llu\n",oldAvail,DescAvail);
		#endif
		if (oldAvail) {
			#ifdef DEBUG
			printf("new desc point 1a\n");
			printf("next desc: %llu\n",(unsigned long long)(((descriptor*)oldAvail)->Next));
			#endif
			
			newAvail = (unsigned long long )(((descriptor*)oldAvail)->Next);
			
			#ifdef DEBUG
			printf("new desc point 1b\n");
			#endif
			if (__sync_val_compare_and_swap((volatile unsigned long long*)&DescAvail, *((unsigned long long*)&oldAvail), *((unsigned long long*)&newAvail))) {
			#ifdef DEBUG
			printf("new desc point 1c\n");
			#endif
				desc = (descriptor*)oldAvail;

				break;
			}
		}
		else {
			desc = AllocNewSB(DESCSBSIZE, sizeof(descriptor));
			organize_desc_list((void *)desc, DESCSBSIZE / sizeof(descriptor), sizeof(descriptor));
			#ifdef DEBUG
			printf("new desc point 2\n");
		#endif
			newAvail = (unsigned long long)desc;
			#ifdef DEBUG
			printf("new_queue.DescAvail: %llu\n",(unsigned long long)newAvail);
		#endif
			
			if (__sync_val_compare_and_swap((volatile unsigned long long*)&DescAvail, *((unsigned long long*)&oldAvail), *((unsigned long long*)&newAvail))) {
			#ifdef DEBUG
				printf("Returning descriptor %p from new descriptor block\n", desc);
				
			#endif
				break;
			}
			munmap((void*)desc, DESCSBSIZE);   
		}
	}

	return desc;
}

void DescRetire(descriptor* desc)
{
	unsigned long long oldAvail, newAvail;
	do {
		oldAvail = DescAvail;
		desc->Next = (descriptor*)oldAvail;
		newAvail = (unsigned long long)desc;
	} while (!__sync_val_compare_and_swap((volatile unsigned long long*)&DescAvail, *((unsigned long long*)&oldAvail), *((unsigned long long*)&newAvail)));
}


static descriptor* ListGetPartial(sizeclass* sc)
{
	#ifdef DEBUG
		printf("ListGetPartial:%p\n",sc->Partial);
	#endif
	if(*((unsigned long long *)&sc->Partial) == 0)
		return (descriptor *)NULL;
	return (descriptor*)lf_fifo_dequeue((lf_fifo_queue_t *)&sc->Partial);
}

static void ListPutPartial(descriptor* desc)
{
	lf_fifo_enqueue(&desc->heap->sc->Partial, (void*)desc);  
}

static void RemoveEmptyDesc(procheap* heap, descriptor* desc)
{
	if (__sync_val_compare_and_swap((volatile unsigned long long*)&heap->Partial, desc, NULL)) {
		DescRetire(desc);
	}
	
}

static descriptor* HeapGetPartial(procheap* heap)
{ 
	descriptor* desc;
  
	do {
		desc = *((descriptor**)&heap->Partial); // casts away the volatile
		#ifdef DEBUG
		printf("HeapGetPartial: heap->Partial:%p\n",desc);
		#endif
		if (desc == NULL) {
			return ListGetPartial(heap->sc);
		}
	} while (!__sync_val_compare_and_swap((volatile unsigned long long*)&heap->Partial, desc, NULL));

	return desc;
}

static void HeapPutPartial(descriptor* desc)
{
	descriptor* prev;

	do {
		prev = (descriptor*)desc->heap->Partial; // casts away volatile
	} while (!__sync_val_compare_and_swap((volatile unsigned long long*)&desc->heap->Partial, prev, desc));

	if (prev) {
		ListPutPartial(prev); 
	}
}

static void UpdateActive(procheap* heap, descriptor* desc, unsigned long morecredits)
{ 
	active oldactive, newactive;
	anchor oldanchor, newanchor;

#ifdef DEBUG
	printf("UpdateActive() heap->Active %p, credits %lu\n", *((void**)&heap->Active), morecredits);
#endif

	*((unsigned long long*)&oldactive) = 0;
	newactive.ptr = (unsigned long long)desc;
	newactive.credits = morecredits - 1;
	if (__sync_val_compare_and_swap((volatile unsigned long long*)&heap->Active, *((unsigned long long*)&oldactive), *((unsigned long long*)&newactive))) {
		return;
	}

	// Someone installed another active sb
	// Return credits to sb and make it partial
	do { 
		newanchor = oldanchor = desc->Anchor;
		newanchor.count += morecredits;
		newanchor.state = PARTIAL;
	} while (!__sync_val_compare_and_swap((volatile unsigned long long*)&desc->Anchor, *((unsigned long long*)&oldanchor), *((unsigned long long*)&newanchor)));

	HeapPutPartial(desc);
}

static descriptor* mask_credits(active oldactive)
{
	return (descriptor*)oldactive.ptr;
}

static void* MallocFromActive(procheap *heap) 
{
	active newactive, oldactive;
	descriptor* desc;
	anchor oldanchor, newanchor;
	void* addr;
	unsigned long morecredits = 0;
	unsigned int next = 0;

	// First step: reserve block
	do { 
		newactive = oldactive = heap->Active;
		if (!(*((unsigned long long*)(&oldactive)))) {
			return NULL;
		}
		if (oldactive.credits == 0) {
			*((unsigned long long*)(&newactive)) = 0;
#ifdef DEBUG
			printf("MallocFromActive() setting active to NULL, %lu, %d\n", newactive.ptr, newactive.credits);
			
#endif
		}
		else {
			--newactive.credits;
		}
	} while (!__sync_val_compare_and_swap((volatile unsigned long long*)&heap->Active, *((unsigned long long*)&oldactive), *((unsigned long long*)&newactive)));

#ifdef DEBUG
	printf("MallocFromActive() heap->Active %p, credits %hu\n", *((void**)&heap->Active), oldactive.credits);
	
#endif

	// Second step: pop block
	desc = mask_credits(oldactive);
	do {
		// state may be ACTIVE, PARTIAL or FULL
		newanchor = oldanchor = desc->Anchor;
		addr = (void *)((unsigned long long)desc->sb + oldanchor.avail * desc->sz);
		next = *(unsigned long long*)addr;
		newanchor.avail = next;
		++newanchor.tag;

		if (oldactive.credits == 0) {

			// state must be ACTIVE
			if (oldanchor.count == 0) {
#ifdef DEBUG
				printf("MallocFromActive() setting superblock %p to FULL\n", desc->sb);
				
#endif
				newanchor.state = FULL;
			}
			else { 
				morecredits = min(oldanchor.count, MAXCREDITS);
				newanchor.count -= morecredits;
			}
		} 
	} while (!__sync_val_compare_and_swap((volatile unsigned long long*)&desc->Anchor, *((unsigned long long*)&oldanchor), *((unsigned long long*)&newanchor)));

#ifdef DEBUG
	printf("MallocFromActive() sb %p, Active %p, avail %d, oldanchor.count %hu, newanchor.count %hu, morecredits %lu, MAX %d\n", 
			desc->sb, *((void**)&heap->Active), desc->Anchor.avail, oldanchor.count, newanchor.count, morecredits, MAXCREDITS);
	
#endif

	if (oldactive.credits == 0 && oldanchor.count > 0) {
		UpdateActive(heap, desc, morecredits);
	}

	*((char*)addr) = (char)SMALL; 
	addr += TYPE_SIZE;
	*((descriptor**)addr) = desc; 
	return ((void*)((unsigned long long)addr + PTR_SIZE));
}

static void* MallocFromPartial(procheap* heap)
{
	descriptor* desc;
	anchor oldanchor, newanchor;
	unsigned long morecredits;
	void* addr;
  
retry:
	#ifdef DEBUG
		printf("retry\n");
		
		#endif
	desc = HeapGetPartial(heap);
	#ifdef DEBUG
		printf("HeapGetPartial \n");
		
		#endif
	if (!desc) {
		return NULL;
	}

	desc->heap = heap;
	do {
		// reserve blocks
		newanchor = oldanchor = desc->Anchor;
		if (oldanchor.state == EMPTY) {
			DescRetire(desc); 
			goto retry;
		}

		// oldanchor state must be PARTIAL
		// oldanchor count must be > 0
		morecredits = min(oldanchor.count - 1, MAXCREDITS);
		newanchor.count -= morecredits + 1;
		newanchor.state = (morecredits > 0) ? ACTIVE : FULL;
	} while (!__sync_val_compare_and_swap((volatile unsigned long long*)&desc->Anchor, *((unsigned long long*)&oldanchor), *((unsigned long long*)&newanchor)));

	#ifdef DEBUG
		printf("HeapGetPartial CAS \n");
		
		#endif

	do { 
		// pop reserved block
		newanchor = oldanchor = desc->Anchor;
		addr = (void*)((unsigned long long)desc->sb + oldanchor.avail * desc->sz);

		newanchor.avail = *(unsigned long long*)addr;
		++newanchor.tag;
	} while (!__sync_val_compare_and_swap((volatile unsigned long long*)&desc->Anchor, *((unsigned long long*)&oldanchor), *((unsigned long long*)&newanchor)));

	if (morecredits > 0) {
		UpdateActive(heap, desc, morecredits);
	}

	*((char*)addr) = (char)SMALL; 
	addr += TYPE_SIZE;
	*((descriptor**)addr) = desc; 
	return ((void *)((unsigned long long)addr + PTR_SIZE));
}

static void* MallocFromNewSB(procheap* heap)
{
	descriptor* desc;
	void* addr;
	active newactive, oldactive;

	*((unsigned long long*)&oldactive) = 0;
	desc = DescAlloc();
	desc->sb = AllocNewSB(heap->sc->sbsize, SBSIZE);

	desc->heap = heap;
	desc->Anchor.avail = 1;
	desc->sz = heap->sc->sz;
	desc->maxcount = heap->sc->sbsize / desc->sz;

	// Organize blocks in a linked list starting with index 0.
	organize_list(desc->sb, desc->maxcount, desc->sz);

#ifdef DEBUG
	printf("New SB %p associated with desc %p (sz %u, sbsize %d, heap %p, Anchor.avail %hu, Anchor.count %hu)\n", 
			desc->sb, desc, desc->sz, heap->sc->sbsize, heap, desc->Anchor.avail, desc->Anchor.count);
	
#endif

	*((unsigned long long*)&newactive) = 0;
	newactive.ptr = (unsigned long long)desc;
	newactive.credits = min(desc->maxcount - 1, MAXCREDITS) - 1;

	desc->Anchor.count = max(((signed long long)desc->maxcount - 1 ) - ((signed long long)newactive.credits + 1), 0); // max added by Scott
	desc->Anchor.state = ACTIVE;

#ifdef DEBUG
	printf("MallocFromNewSB() sz %u, maxcount %u, Anchor.count %hu, newactive.credits %hu, max %ld\n", 
			desc->sz, desc->maxcount, desc->Anchor.count, newactive.credits, 
			((signed long long)desc->maxcount - 1 ) - ((signed long long)newactive.credits + 1));
	
#endif

	// memory fence.
	if (__sync_val_compare_and_swap((volatile unsigned long long*)&heap->Active, *((unsigned long long*)&oldactive), *((unsigned long long*)&newactive))) { 
		addr = desc->sb;
		*((char*)addr) = (char)SMALL; 
		addr += TYPE_SIZE;
		*((descriptor **)addr) = desc; 
		return (void *)((unsigned long long)addr + PTR_SIZE);
	} 
	else {
		//Free the superblock desc->sb.
		munmap(desc->sb, desc->heap->sc->sbsize);
		DescRetire(desc); 
		return NULL;
	}
}

static procheap* find_heap(size_t sz)
{
	procheap* heap;
  
	// We need to fit both the object and the descriptor in a single block
	sz += HEADER_SIZE;
	if (sz > 2048) {
		return NULL;
	}
  
	heap = heaps[sz / GRANULARITY];
	if (heap == NULL) {
		heap = mmap(NULL, sizeof(procheap), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		*((unsigned long long*)&(heap->Active)) = 0;
		heap->Partial = NULL;
		heap->sc = &sizeclasses[sz / GRANULARITY];
		heaps[sz / GRANULARITY] = heap;
	}
	
	return heap;
}

static void* alloc_large_block(size_t sz)
{
	void* addr;
	//addr = mmap(NULL, sz + HEADER_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	
	if((addr = mmap(NULL,sz + HEADER_SIZE,PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED){		
		//null => kernel chooses address for new mapping 
		printf("Memory error:Alloc Large Block Fail: %e",errno);
		exit(1);
	}
	
	// If the highest bit of the descriptor is 1, then the object is large (allocated / freed directly from / to the OS)
	*((char*)addr) = (char)LARGE;
	addr += TYPE_SIZE;
	*((unsigned long long*)addr) = sz + HEADER_SIZE;
	return (void*)(addr + PTR_SIZE); 
}

void* malloc(size_t sz)
{ 
	procheap *heap;
	void* addr;

	//printf("Malloc of size :%d  ",sz);

#ifdef DEBUG
	printf("malloc() sz %lu\n", sz);
	
#endif
	// Use sz and thread id to find heap.
	heap = find_heap(sz);

	#ifdef DEBUG
		printf("heap %p \n", heap);
		
#endif
	if (!heap) {
		// Large block
		addr = alloc_large_block(sz);
#ifdef DEBUG
		printf("Large block allocation: %p\n", addr);
		
#endif
		return addr;
	}

	while(1) { 
		addr = MallocFromActive(heap);
		#ifdef DEBUG
		printf("Mal Active: heap->sc->sz %u\n", heap->sc->sz);
		#endif
		if (addr) {
#ifdef DEBUG
			printf("malloc() return MallocFromActive %p\n", addr);
			
#endif
			return addr;
		}

		

		addr = MallocFromPartial(heap);
		#ifdef DEBUG
		printf("Mal Partial\n");
		
		#endif
		if (addr) {
#ifdef DEBUG
			printf("malloc() return MallocFromPartial %p\n", addr);
			
#endif
			return addr;
		}
		addr = MallocFromNewSB(heap);

		#ifdef DEBUG
		printf("Mal newSb\n");
		
		#endif

		if (addr) {
#ifdef DEBUG
			printf("malloc() return MallocFromNewSB %p\n", addr);
			
#endif
			return addr;
		}
	} 
}

void free(void* ptr) 
{
	descriptor* desc;
	void* sb;
	anchor oldanchor, newanchor;
	procheap* heap = NULL;

#ifdef DEBUG
	printf("Calling my free %p\n", ptr);
	
#endif

	if (!ptr) {
		return;
	}
	
	// get prefix
	ptr = (void*)((unsigned long long)ptr - HEADER_SIZE);  
	if (*((char*)ptr) == (char)LARGE) {
#ifdef DEBUG
		printf("Freeing large block\n");
		
#endif
		munmap(ptr, *((unsigned long long*)(ptr + TYPE_SIZE)));
#ifdef DEBUG
		printf(" Large free end");
		
#endif
		return;
	}
	desc = *((descriptor**)((unsigned long long)ptr + TYPE_SIZE));
	
	sb = desc->sb;

	#ifdef DEBUG
			printf("After sb = desc->sb\n");
			
	#endif
	
	do { 
		newanchor = oldanchor = desc->Anchor;

		*((unsigned long long*)ptr) = oldanchor.avail;
		newanchor.avail = ((unsigned long long)ptr - (unsigned long long)sb) / desc->sz;

		if (oldanchor.state == FULL) {
#ifdef DEBUG
			printf("Marking superblock %p as PARTIAL\n", sb);
			
#endif
			newanchor.state = PARTIAL;
		}

		if (oldanchor.count == desc->maxcount - 1) {
			heap = desc->heap;
			// instruction fence.
#ifdef DEBUG
			printf("Marking superblock %p as EMPTY; count %d\n", sb, oldanchor.count);
			
#endif
			newanchor.state = EMPTY;
		} 
		else {
			++newanchor.count;
		}
		// memory fence.
	} while (!__sync_val_compare_and_swap((volatile unsigned long long*)&desc->Anchor, *((unsigned long long*)&oldanchor), *((unsigned long long*)&newanchor)));

		#ifdef DEBUG
			printf("After free CAS\n");
			
#endif
	


//printf("No error");
	if (newanchor.state == EMPTY) {
#ifdef DEBUG
		printf("before freeing");
		printf("Freeing superblock %p with desc %p (count %hu)\n", sb, desc, desc->Anchor.count);
		
#endif

		munmap(sb, heap->sc->sbsize);
		RemoveEmptyDesc(heap, desc);
	} 
	else if (oldanchor.state == FULL) {
#ifdef DEBUG
		printf("Puting superblock %p to PARTIAL heap\n", sb);
		
#endif
		HeapPutPartial(desc);
	}


#ifdef DEBUG
		printf(" Free end");
		
#endif

}
