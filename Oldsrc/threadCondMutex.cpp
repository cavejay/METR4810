///*
//With reference to codebase.eu/tutorial
//
//If more than one thread is blocked by a cond variable, use pthread_cond_broadcast instead
//pthread_mutex_trylock is a reference to seeing whether the lock is open for use
//*/
//
//#include <pthread.h>
//#include <unistd.h>
//#include <iostream>
//
//using namespace std;
//
//void *function1(void* argument);
//void *function2(void* argument);
//int  count = 0;
//const int COUNT_DONE  = 10;
//
//// Add a global mutex variable and a condition element
//// This variable can be locked so that only one thread can change it at a time depending on the condition
//pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;
//
//int main(void)
//{
//	// Instantiate and create each thread (of however many)
//	// In this case, we want 4.
//    pthread_t cam1, cam2, cam3, cam4 ;
//    pthread_create(&cam1, NULL, function1, NULL); // Note that we can run whatever function we choose for each thread
//    pthread_create(&cam2, NULL, function2, NULL); // They can even be the same function with different parameters
//	pthread_create(&cam3, NULL, function2, NULL);
//	pthread_create(&cam4, NULL, function1, NULL);
//
//	// make sure to join the threads after splitting them off from the main thread
//	pthread_join(cam1, NULL);
//    pthread_join(cam2, NULL);
//	pthread_join(cam3, NULL);
//	pthread_join(cam4, NULL);
//
//	// All threads are terminated when main exits
//    return 0;
//}
//
//void *function1(void *argument)
//{
//    // function1 (thread1) likes to add 1 to count regardless of it's current value,
//    // but will only do it if function2 is done working with count.
//
//   for(;;)
//   {
//		// Lock mutex
//		pthread_mutex_lock(&count_mutex);
//
//		// Wait for function2 to work with count
//		pthread_cond_wait( &condition_var, &count_mutex );
//
//		// As the code has passed the wait condition, function2 must be done with count
//		// Function1 now just adds one to count
//		count++;
//
//		// Unlock the mutex after function1 is done with it
//		pthread_mutex_unlock( &count_mutex );
//
//      if(count >= COUNT_DONE) pthread_exit(NULL);
//    }
//}
//
//void *function2(void *argument)
//{
//    // Function 2 is essentially the same as function1 but will only add if count ==5
//
//    for(;;)
//    {
//        pthread_mutex_lock(&count_mutex);
//
//        if(count==5) // check if count is odd
//        {
//			count++;
//        }
//        else
//        {
//			// Signal that control is back to function1
//			pthread_cond_signal( &condition_var );
//        }
//
//		// Unlock the variable so that function1 can actually access it
//		pthread_mutex_unlock( &count_mutex );
//
//        if(count >= COUNT_DONE)pthread_exit(NULL);
//    }
//}
