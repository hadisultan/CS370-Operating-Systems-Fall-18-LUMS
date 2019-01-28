#include <pthread.h>
#include "main.h"
#include "part2.h"
#include <stdio.h>
#include <semaphore.h>


const int INTER_ARRIVAL_TIME = 5;
const int NUM_TRAINS = 5;
int currentLoc[5];
sem_t sem12[50];
int nStations;
int maxN;
pthread_mutex_t lock[50];

void* funckTrains(void* lol){
	int start = (int)lol;
	// printf("Start is: %d\n", start);
	currentLoc[start] = start;
	int i, j;
	while(1){
		for(i = 0; i<50; i++){
			pthread_mutex_lock(&lock[i]);
			sem_post(&sem12[i]);
			pthread_mutex_unlock(&lock[i]);
		}
		sleep(1);
		// printf("Train %d moving from station %d to station ", start, currentLoc[start]);
		currentLoc[start] = (currentLoc[start]+1)%nStations;
		// printf("%d.\n", currentLoc[start]);
	}
}

/**
 * Do any initial setup work in this function.
 * numStations: Total number of stations. Will be >= 5. Assume that initially
 * the first train is at station 1, the second at 2 and so on.
 * maxNumPeople: The maximum number of people in a train
 */
void initializeP2(int numStations, int maxNumPeople) {
	nStations = numStations;
	maxN = maxNumPeople;
	pthread_t trains[5];
	int i;
	for(i=0; i<5; i++)
	{
		pthread_t tid1;
		pthread_create(&tid1, NULL, funckTrains, (void*)i);
	}
	
}

/**
 * Print in the following format:
 * If a user borads on train 0, from station 0 to station 1, and another boards
 * train 2 from station 2 to station 4, then the output will be
 * 0 0 1
 * 2 2 4
 */
void *goingFromToP2(void *argu) {
    struct argument *arg = (struct argument*)(argu);
    int i;
    int inside = 0;
    int train;
    while(1){
    	for(i = 0; i<5; i++){
    		// printf("ID %d waiting for %d.\n", arg->id, i);
    		sem_wait(&sem12[arg->id]);
    	}
    	for(i = 0; i<5; i++){
	    	if(currentLoc[i] == arg->from && inside!=1){
	    		inside = 1;
	    		train = i;
	    		// printf("Passenger %d is getting on train %d from station %d.\n", arg->id, train, arg->from);
	    		break;
	    	}
    	}
    	if(currentLoc[train] == arg->to && inside == 1){
    		// printf("Passenger %d is getting off train %d from station %d.\n", arg->id, train, arg->to);
    		printf("%d %d %d\n", train, arg->from, arg->to);
			pthread_exit(NULL);
    	}
    }
}

void startP2(){}
