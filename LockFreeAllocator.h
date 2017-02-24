#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "Stack.c"




/*
typedef struct {
unsigned long long anchorfield;		//avail:24, count:24, state:2, tag:12
}anchor;


typedef struct{

unsigned long long DescAvail;		//DescAvail:46 and tag:18	
}DescAvail;
*/

#define BLOCKSTAT(x)	x & 0x1
#define BLOCKDESC(x)	x & ~0x1

typedef unsigned long long ull;
//64 bit machine.
/*superblock descriptor*/
typedef struct{
	ull avail:24,count:24,state:2,tag:14;

}anchor;


/* active Heap descriptor */
typedef struct{

	ull ptr:58,credits:6;

}active;

typedef struct{

	ull DescAvail:48,tag:16;

}desc_avail;

typedef struct{
	//each SB contains 
	//LIst Partial
	stack_t Partial;
	unsigned int sz;		//blocksize
	unsigned int sbsize;		//superblock size  => no of blocks sbszie/sz
}sizeclass;

typedef struct{
	
	active Active;			//Initially Null 
	descriptor* Partial;		//Initially Null 
	sizeclass* sc;			//pointer to parent superblock
}procheap;


typedef struct{
	anchor Anchor;
	descriptor* Next;
	void* sb;			//pointer to superblock
	procheap* heap;			//pointer to owner heap
	unsigned int sz;		//block size
	unsigned int maxcount;		//superblock size/sz	

}descriptor;


#define GET_AVAIL(x)		((x>>38) & 0xFFFFF)
#define PUT_AVAIL(x,avail)	(x += (avail & 0xFFFFF)<<38)

#define GET_COUNT(x)		((x>>14) & 0xFFFFF)
#define PUT_COUNT(x,count)	(x += (count & 0xFFFFF)<<14)

#define GET_STATE(x)		((x>>12) & 0x3)
#define PUT_STATE(x,state)	 (x += (state & 0x3)<<12)	

#define GET_TAG(x)		(0xFFF & x)  
#define PUT_TAG(x,val)		(x += (0xFFF & val))

#define GET_DESCTAG(x)		(x & 0x3FFFF)		
#define PUT_DESCTAG(x,tag)	(x += 0x3FFFF & tag)

#define GET_DESCAVAIL(x)			(((x>>18) & 0x3FFFFFFFFFFF)		
#define PUT_DESCAVAIL(x,descavail)		((x += (descavail & 0x3FFFFFFFFFFF)<<18)
#define MIN(a,b)				(((a)>(b))?(b):(a))	

#define PGSIZE 		4096
#define	MAXCREDITS	64
#define ACTIVE 		0
#define FULL 		1
#define PARTIAL		2 
#define EMPTY 		3
#define NDESC		1024
#define DESCSBSIZE	(NDESC * sizeof(descriptor))
extern void *malloc(unsigned int);
extern void free(void *);

