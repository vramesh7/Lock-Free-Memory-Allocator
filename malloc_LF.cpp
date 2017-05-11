
extern "C" {
#include "LockFreeAllocator.h"
}

#include <new>

void* operator new(std::size_t sz) throw (std::bad_alloc)
{
  return malloc(sz);
}

void* operator new (size_t sz, const std::nothrow_t&) throw()
{
  return malloc(sz);
}

void operator delete (void * ptr) throw()
{
  free(ptr);
}

void* operator new[](std::size_t sz) throw (std::bad_alloc)
{
  return malloc(sz);
}

void* operator new[] (size_t sz, const std::nothrow_t&) throw()
{
  return malloc(sz);
}

void operator delete[] (void * ptr) throw()
{
  free(ptr);
}

