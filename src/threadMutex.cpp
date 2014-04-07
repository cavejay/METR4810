
#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

void* function1(void* argument);
void* function2(void* argument);

// Add a global mutex variable
// This variable can be locked so that only one thread can change it at a time
pthread_mutex_t mutex_for_some_value = PTHREAD_MUTEX_INITIALIZER;

int main(void)
{
	// Instantiate and create each thread (of however many)
	// In this case, we want 4.
    pthread_t cam1, cam2, cam3, cam4 ; 
    pthread_create(&cam1, NULL, function1, NULL); // Note that we can run whatever function we choose for each thread
    pthread_create(&cam2, NULL, function2, NULL); // They can even be the same function with different parameters
	pthread_create(&cam3, NULL, function2, NULL);
	pthread_create(&cam4, NULL, function1, NULL); 
	
	// make sure to join the threads after splitting them off from the main thread
	pthread_join(cam1, NULL);
    pthread_join(cam2, NULL);
	pthread_join(cam3, NULL);
	pthread_join(cam4, NULL);
	
	// All threads are terminated when main exits
    return 0;
}

void * function1(void * argument)
{
    cout << " hello ";
    sleep(2); // Running functions on timers is bad practice, wait conditions on mutex locks is the correct method
    return 0;
}

void * function2(void * argument)
{
    cout << " world ";
    return 0;
}