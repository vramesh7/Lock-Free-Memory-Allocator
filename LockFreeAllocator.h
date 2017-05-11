#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

//#include "atomic.h"
#include "queue.h"

struct Descriptor;
typedef struct Descriptor descriptor;
struct Procheap;
typedef struct Procheap procheap;

#define TYPE_SIZE	sizeof(void*)
#define PTR_SIZE	sizeof(void*)
#define HEADER_SIZE	(TYPE_SIZE + PTR_SIZE)

#define LARGE		0
#define SMALL		1

#define	PAGESIZE	4096
#define SBSIZE		(16 * PAGESIZE)
#define DESCSBSIZE	(1024 * sizeof(descriptor))

#define ACTIVE		0
#define FULL		1
#define PARTIAL		2
#define EMPTY		3

#define	MAXCREDITS	64 // 2^(bits for credits in active)
#define GRANULARITY	8
#define DEBUGx	1

typedef struct {
	unsigned long long 	avail:24,count:24, state:2, tag:14;
} anchor;

struct Descriptor {
	struct queue_elem_t	lf_fifo_queue_padding;
	volatile anchor		Anchor;
	descriptor*		Next;
	void*			sb;		// pointer to superblock
	procheap*		heap;		// pointer to owner procheap
	unsigned int		sz;		// block size
	unsigned int		maxcount;	// superblock size / sz
};

typedef struct {
	lf_fifo_queue_t		Partial;	// initially empty
	unsigned int		sz;		// block size
	unsigned int		sbsize;		// superblock size
} sizeclass;

typedef struct {
	unsigned long long	ptr:58, credits:6;
} active;

struct Procheap {
	volatile active		Active;		// initially NULL
	volatile descriptor*	Partial;	// initially NULL
	sizeclass*		sc;		// pointer to parent sizeclass
};

/*
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
*/
extern void* malloc(size_t sz);
extern void free(void* ptr);


