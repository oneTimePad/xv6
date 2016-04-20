#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "pthread.h"

#define NUM_THREADS 7
#define TARGET_COUNT_PER_THREAD 100000

pthread_t threads[20];
void *lol(void *arg){
	printf(1, "Grandchild started\n");
	int i;
	int counter;

	sleep(10);
	for (i=0; i<TARGET_COUNT_PER_THREAD; i++) {
		sleep(0);
		counter++;
		sleep(0);
	}
	printf(1, "Grandchild exiting\n");
	pthread_exit(arg);
}

void *thread(void *arg)
{
	int i;
	int counter;

	sleep(10);
	printf(1, "thread %d: started...\n", *(int*)arg);

	int* wtf = (int*) malloc(sizeof(int));
	*wtf = NUM_THREADS;
	if(*(int*)arg == 0){
		int re = pthread_create(&threads[NUM_THREADS], 0, lol, wtf);
		printf(1,"id:%d\n",re);
	}

	for (i=0; i<TARGET_COUNT_PER_THREAD; i++) {
		sleep(0);
		counter++;
		sleep(0);
	}
	//pthread_join(threads[NUM_THREADS], (void**)&wtf);
	//printf(1, "Grandchild joined parent\n");

	pthread_exit(arg);
}

int main(int argc, char **argv)
{
	int i;
	int passed = 1;

	// Set up thread stuff
	// Threads

	// Args
	int *args[NUM_THREADS];

	// Allocate stacks and args and make sure we have them all
	// Bail if something fails
	for (i=0; i<NUM_THREADS; i++) {
		args[i] = (int*) malloc(sizeof(int));
		if (!args[i]) {
			printf(1, "main: could not get memory (for arg) for thread %d, exiting...\n");
			exit();
		}

		*args[i] = i;
	}

	printf(1, "main: running with %d threads...\n", NUM_THREADS);

	// Start all children
	for (i=0; i<NUM_THREADS; i++) {
		pthread_create(&threads[i], 0, thread, args[i]);
		printf(1, "main: created thread with pid %d\n", threads[i].pid);
	}

	//exit();
	// Wait for all children
	for (i=0; i<NUM_THREADS+1; i++) {
		void * retval;
		int r;
		r = pthread_join(threads[i], &retval);
		printf(1,"RET%d\n",r);
		if (r < 0) {
			passed = 0;
		}
		if (*(int*)retval != i) {
			passed = 0;
		}
		printf(1, "main: thread %d joined...retval=%d\n", i, *(int*)retval);
	}

	if (passed) {
		printf(1, "TEST PASSED!\n");
	}
	else {
		printf(1, "TEST FAILED!\n");
	}

	// Clean up memory
	for (i=0; i< NUM_THREADS; i++) {
		free(args[i]);
	}


	// Exit
	exit();
}
