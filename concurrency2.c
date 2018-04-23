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
#define FORKS 5
#include "mt19937ar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

//function prototype(s)
void spawn_threads();
void* philosopher_thread(void *thread_num);
void get_name(int select, char name[]);
void think(char name[]);
void get_forks(int select, char name[]);
void eat(char name[]);
void put_forks(int select, char name[]); 
int random_range(int min_val, int max_val);

//create mutex lock(s)
pthread_mutex_t fork1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork5 = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	//seed random number generation
	init_genrand(time(NULL));
	srand(time(NULL));	

	printf("\n%d forks and %d plates of food have been placed on the table.\n", FORKS, PHILOSOPHERS);

	//create threads and wait for their completion
	spawn_threads();
	
	//destroy mutex lock(s)	
	pthread_mutex_destroy(&fork1);
	pthread_mutex_destroy(&fork2);
	pthread_mutex_destroy(&fork3);
	pthread_mutex_destroy(&fork4);
	pthread_mutex_destroy(&fork5);
	
	return 0;
}

/* Function: spawn_threads
 * -----------------------
 * Spawns five philosopher threads, then waits for threads to
 * finish execution and join. Since these threads will run forever, we expect
 * to block here indefinitely.
 */
void spawn_threads()
{
	pthread_t thrd;
	for(int i = PHILOSOPHERS; i > 0; i--){
		int *thread_num = malloc(sizeof(*thread_num));
		*thread_num = i; //thread_num will act as an id for thread
		pthread_create(&thrd, NULL, philosopher_thread, (void *) thread_num);
	}
	
	//join thread (this should never finish)
 	pthread_join(thrd, NULL);
}

/* Function: philosopher_thread
 * -------------------------
 * This function is called by a new philosopher thread when it is created.
 * 
 * First this thread gets its name using the identifer passed to it.
 * Afterwards it loops forever, thinking, getting forks, eating,
 * and putting forks down.
 *
 * *thread_num: A pointer to an identifer for the thread.
 */
void* philosopher_thread(void *thread_num)
{
	int *select_ptr = (int*) thread_num; 
	int select = *select_ptr;
	char name[50];
	get_name(select, name);
	printf("%s sits down at the table.\n", name);
	sleep(1);

	while(true){
		think(name);
		get_forks(select, name);
		eat(name);
		put_forks(select, name);
	}

	free(thread_num);
}

/* Function: get_name
 * -------------------------
 * This function selects a philosopher name
 * and assigns it to the input char array.
 *
 * select: Used to a select a given name.
 * name: The array where we store the name.
 */
void get_name(int select, char name[])
{
	switch(select){
	
		case 1 :
			strncpy(name, "Descartes", 50);
			break;
		
		case 2 :
			strncpy(name, "Socrates", 50);
			break;

		case 3 :
			strncpy(name, "Confucius", 50);
			break;

		case 4 :
			strncpy(name, "Plato", 50);
			break;

		case 5 :
			strncpy(name, "Voltaire", 50);
			break;

		default :
			strncpy(name, "Philosopher", 50);
			break;
	}
}

/* Function: think
 * -------------------------
 * Thread sleeps between 1 and 20 seconds.
 *
 * name: The current philosopher's name.
 */
void think(char name[])
{
	int think_time = random_range(1, 20);
	printf("%s started thinking for %d seconds.\n", name, think_time);
	sleep(think_time);
}

/* Function: get_forks
 * -------------------------
 * The current philosopher attempts
 * to pickup the fork to their right and left
 * one philosopher always picks up the left fork
 * first, the rest pick up the right first.
 * In this context picking up a fork means to
 * lock a mutex.
 *
 * select: The id of the philosopher.
 * name: The current philosopher's name.
 */
void get_forks(int select, char name[])
{
	switch(select){
	
		case 1 :
			pthread_mutex_lock(&fork1);
			printf("%s got fork #1.\n", name);
			pthread_mutex_lock(&fork2);
			printf("%s got fork #2.\n", name);
			break;
		
		case 2 :
			pthread_mutex_lock(&fork2);
			printf("%s got fork #2.\n", name);
			pthread_mutex_lock(&fork3);
			printf("%s got fork #3.\n", name);
			break;

		case 3 :
			pthread_mutex_lock(&fork3);
			printf("%s got fork #3.\n", name);
			pthread_mutex_lock(&fork4);
			printf("%s got fork #4.\n", name);
			break;

		case 4 :
			pthread_mutex_lock(&fork4);
			printf("%s got fork #4.\n", name);
			pthread_mutex_lock(&fork5);
			printf("%s got fork #5.\n", name);
			break;

		case 5 :
			//philosopher #5 is left handed.
			pthread_mutex_lock(&fork1);
			printf("%s got fork #1.\n", name);
			pthread_mutex_lock(&fork5);
			printf("%s got fork #5.\n", name);
			break;
	}
}

/* Function: eat
 * -------------------------
 * Thread sleeps between 2 and 9 seconds.
 *
 * name: The current philosopher's name.
 */
void eat(char name[])
{
	int eat_time = random_range(2, 9);
	printf("%s started eating for %d seconds.\n", name, eat_time);
	sleep(eat_time);
}

/* Function: put_forks
 * -------------------------
 * The current philosopher attempts
 * to set down the fork to their right and left
 * one philosopher always sets down the left fork
 * first, the rest set down the right first.
 * In this context picking up a fork means to
 * unlock a mutex.
 *
 * select: The id of the philosopher.
 * name: The current philosopher's name.
 */
void put_forks(int select, char name[])
{
	switch(select){
	
		case 1 :
			pthread_mutex_unlock(&fork1);
			printf("%s set down fork #1.\n", name);
			pthread_mutex_unlock(&fork2);
			printf("%s set down fork #2.\n", name);
			break;
		
		case 2 :
			pthread_mutex_unlock(&fork2);
			printf("%s set down fork #2.\n", name);
			pthread_mutex_unlock(&fork3);
			printf("%s set down fork #3.\n", name);
			break;

		case 3 :
			pthread_mutex_unlock(&fork3);
			printf("%s set down fork #3.\n", name);
			pthread_mutex_unlock(&fork4);
			printf("%s set down fork #4.\n", name);
			break;

		case 4 :
			pthread_mutex_unlock(&fork4);
			printf("%s set down fork #4.\n", name);
			pthread_mutex_unlock(&fork5);
			printf("%s set down fork #5.\n", name);
			break;

		case 5 :
			//philosopher #5 is left handed.
			pthread_mutex_unlock(&fork1);
			printf("%s set down fork #1.\n", name);
			pthread_mutex_unlock(&fork5);
			printf("%s set down fork #5.\n", name);
			break;
	}
}
 
/* Function: random_range
 * ----------------------
 * This function finds a random number between a min and max value (inclusive).
 * The random value is created using rdrand x86 ASM on systems that support it,
 * and it uses Mersenne Twister on systems that do not support rdrand.
 *
 * min_val: The lowest possible random number.
 * max_val: The highest possible random number.
 *
 * returns: A random number in the given range. In the case that min_val is
 *          greater than max_val this function returns -1.
 */
int random_range(int min_val, int max_val)
{
	if(min_val > max_val)
		return -1;

	int output;
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;

	char vendor[13];
	
	eax = 0x01;

	__asm__ __volatile__(
	                     "cpuid;"
	                     : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
	                     : "a"(eax)
	                     );
	if(ecx & 0x40000000){
		//use rdrand
	__asm__ __volatile__(
	                     "rdrand %0"
                             : "=r"(output)
	                     );
	} else {
		//use mt19937
		output = genrand_int32();
	}

	//get random number in the range requested 
	output = (abs(output) % (max_val + 1 - min_val)) + min_val;
	return output;
}
