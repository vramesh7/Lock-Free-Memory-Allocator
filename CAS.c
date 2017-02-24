#include <pthread.h>

//Test functions for CAS:LOCK BASED
int CompareAndSwapWrapper((unsigned long long *) add, unsigned long long old_val, unsigned long long new_val);
int CompareAndSwap((unsigned long long *) add, unsigned long long old_val, unsigned long long new_val);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int CompareAndSwapWrapper((unsigned long long *) add, unsigned long long old_val, unsigned long long new_val){
	int value;
	pthread_mutex_lock(&mutex);
	value = CompareAndSwap(add, old_val, new_val);
	pthread_mutex_unlock(&mutex);
	return value;
}


int CompareAndSwap((unsigned long long *) add, unsigned long long old_val, unsigned long long new_val)
{
	int value = *add;
	if(value == old_val)
		*add = new_val;
	return value;

}

