#include "part1.h"
#include <pthread.h>
#include "main.h"
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

// #define _GNU_SOURCE


/**
 * Do any initial setup work in this function.
 * numFloors: Total number of floors elevator can go to
 * maxNumPeople: The maximum capacity of the elevator
 */

int nFloors, maxN;
int numPassangers;
int currentLoc = 0;
sem_t sem1[10];
sem_t sem2;
int decide[10];

void* moveElevator(){
	int j,k,i;
	sem_post(&sem2);
	// sem_post(&mutex_startelev);
	while(1){
		for (j = 0; j < nFloors; ++j)
		{
			currentLoc = j;
			// printf("Loc: %d\n", currentLoc);
			for (k = 0; k<10; k++)
			{
				sem_post(&sem1[k]);
				/* code */
			}
			// for(k = 0; k < )
			sleep(1);
			// for (i = 0; i < numPassangers; ++i)
			// {
				// if(decide[i] == 1){
					// move on
				// }else{
					// --j;
				// }
			// }

		}

		for (j = 9; j > 0; --j)
		{
			currentLoc = j;
			// printf("Loc: %d\n", currentLoc);

			for (k = 0; k<10; k++)
			{
				sem_post(&sem1[k]);
				/* code */
			}
			// for(k = 0; k < )
			sleep(1);
			// for (int i = 0; i < numPassangers; ++i)
			// {
			// 	if(decide[i] == 1){
			// 		//move on
			// 	}else{
			// 		--j;
			// 	}
			// }
		}
	}
	
}


// void initNumPass(int numP){
// 	numPassangers = numP;
// 	printf("passengers: %d\n", numPassangers);
// }

void initializeP1(int numFloors, int maxNumPeople) {
	nFloors = numFloors;
	maxN = maxNumPeople;
	pthread_t tid1;
	int k;

	for (k = 0; k<10; k++)
	{
		// sem_post(&sem1[k]);
		/* code */
		sem_init(&sem1[k], 0, 0);
	}
	// sem_init(&sem2, 0, 0);

	pthread_create(&tid1, NULL, moveElevator, NULL);

	sem_wait(&sem2);

	// printf("initializeP1.1\n");
	// pthread_join(tid1, NULL);
	// printf("initializeP1.2\n");

}

/**
 * This program should print data in the specific format
 * described below:
 * If there is a passenger (id 5) going from floor 2 to 4, followed
 * by a passenger (id 6) from 2 to 5, the output should be:
 * 5 2 4
 * 6 2 5
 * i.e. there should be a single space, and each journey should be 
 * on a new line. Incorrectly formatted output will get 0 marks
 */
void* goingFromToP1(void *arg) {
	int i = 0;
	int inside = 0;
	struct argument *temp = (struct argument *) arg;
	// printf("Initialized for %d\n", temp->id);
	while(1){
		sem_wait(&sem1[temp->id]);
		// printf("-----------%d\n", temp->id);
		if(currentLoc == temp->from){
			// printf("%d is entering lift\n", temp->id);
			inside = 1;
			// decide[temp->id] = 1;		
		}
		if(currentLoc == temp->to && inside == 1){

			// printf("%d is exiting lift\n", temp->id);
			// decide[temp->id] = 1;
		    printf("%d %d %d\n", temp->id, temp->from, temp->to);
		    // numPassangers--;
			// inside = 1;
			pthread_exit(NULL);
		}
		// else{
		// 	decide[temp->id] = 1;
		// }
	}

   
}

void startP1(){}
