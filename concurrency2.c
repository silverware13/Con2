/* Concurrency 2
 * CS444 Spring2018
 * ----------------
 * Name: Zachary Thomas
 * Email: thomasza@oregonstate.edu
 * Date: 4/23/2018
 * -------------------------------
 *  This program creates five philosophers
 *  who think, get forks, eat, and put forks down.
 *  It also manages these philosophers to prevent
 *  deadlock. More than one single philosopher
 *  can eat at the same time.
 */

#define PHILOSOPHERS 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

//global variable(s)

//function prototype(s)
void spawn_threads();
void* philosopher_thread();

//create mutex lock
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	printf("%d forks and %d plates of food have been placed on the table.\n", PHILOSOPHERS);

	//create threads and wait for their completion
	spawn_threads(producers, consumers);
	
	//destroy mutex lock	
	pthread_mutex_destroy(&lock);
	
	return 0;
}

/* Function: spawn_threads
 * -----------------------
 *  Spawns five philosopher threads, then waits for threads to
 *  finish execution and join. Since these threads will run forever, we expect
 *  to block here indefinitely.
 */
void spawn_threads()
{
	printf("\nCreating %d philosopher threads.\n\n", PHILOSOPHERS);
	
	pthread_t thrd;
	for(int i = PHILOSOPHERS; i > 0; i--){
		pthread_create(&thrd, NULL, philosopher_thread, NULL);
	}
	
	//join thread (this should never finish)
 	pthread_join(thrd, NULL);
}

/* Function: philosopher_thread
 * -------------------------
 *  This function is called by a new consumer thread when it is created.
 *  
 *  The consumer attempts to get the mutex lock before checking if the 
 *  buffer has at least one item. It then consumes the newest item in 
 *  the buffer which causes it to display a number. Consuming an item 
 *  takes a number of seconds specified by the item. Once this thread
 *  has consumed an item or there is not any items
 *  in the buffer it releases the mutex lock.
 */
void* philosopher_thread()
{
	while(true){
		int no_lock;
		no_lock = pthread_mutex_trylock(&lock);
		if(no_lock){
			sleep(1);
			continue;
		}
		printf("Consumer has mutex lock.\n");
		if(items_in_buffer > 0){
			items_in_buffer--;
			printf("Consumer is working for %d seconds to consume.\n", buffer[items_in_buffer].wait_period);
			sleep(buffer[items_in_buffer].wait_period);
			printf("%d\n", buffer[items_in_buffer].consumption_num);
			printf("Consumer has removed an item.\n");
		}	
		printf("Consumer has released mutex lock.\n\n");
		printf("Buffer is holding %d items.\n\n", items_in_buffer);
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
}
