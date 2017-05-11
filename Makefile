ASM		= x86

	BITS = -m64
	FPIC = -fPIC

#shared object make
CC		= gcc
CXX		= g++

CLFLAGS		= -lpthread -lm -lstdc++
CFLAGS		= -D$(ASM) -D_GNU_SOURCE -D_REENTRANT #-DDEBUG

GCC_CFLAGS	= -Wall $(BITS) -fno-strict-aliasing $(FPIC)
GCC_OPT		= -O3 -ggdb #-DDEBUG

ICC_CFLAGS	= -Wall -wd279 -wd981 -wd1418 -wd1469 -wd383 -wd869 -wd522 -wd810 -wd1684
ICC_OPT		= -O3 -pipe -finline-functions -fomit-frame-pointer  

ifeq ($(CC), gcc)
	OPT = $(GCC_OPT)
	CFLAGS += $(GCC_CFLAGS)
endif
ifeq ($(CC), icc)
	OPT = $(ICC_OPT)
	CFLAGS += $(ICC_CFLAGS)
endif

# Rules

all:	libLockFreeAllocator.so 

clean:
	rm -f *.o *.so

LockFreeAllocator.o:	LockFreeAllocator.h LockFreeAllocator.c  include-$(ASM)/queue.h
		$(CC) $(CFLAGS) $(OPT) -Iinclude-$(ASM) -c LockFreeAllocator.c

malloc_LF.o:	malloc_LF.cpp LockFreeAllocator.h
		$(CXX) $(CFLAGS) $(OPT) -Iinclude-$(ASM) -c malloc_LF.cpp

libLockFreeAllocator.so:	LockFreeAllocator.o malloc_LF.o
		$(CXX) $(CFLAGS) $(OPT) LockFreeAllocator.o malloc_LF.o -o libLockFreeAllocator.so $(CLFLAGS) -shared

