#include <pthread.h>
#include "main.h"
#include "part3.h"
#include <stdio.h>
#include <sys/queue.h>
#include <semaphore.h>


TAILQ_HEAD(head_s, node) head[4][2];

sem_t sempt3;
sem_t sempt32;
sem_t sempt321;
int light;
int cars = 0;

struct node
{
    int c;
    int dest;
    TAILQ_ENTRY(node) nodes;
};

char* getRevEnumDir(int val) {
    char *directions[] = {"NORTH", "SOUTH", "EAST", "WEST"};
    return directions[val];    
}

char* getRevEnumLane(int val) {
    char *lanes[] = {"LEFT", "RIGHT"};
    return lanes[val];
}
void dequeue(int i, int j){
    struct node * e = TAILQ_FIRST(&head[i][j]);
    char* from = getRevEnumDir(i);
    char* to = getRevEnumDir(e->dest);
    char* l = getRevEnumLane(j);


    printf("%s %s %s \n", from, to, l);
    TAILQ_REMOVE(&head[i][j], e , nodes);

}


static void insertQueue(TAILQ_HEAD(head_s, node)* head, int string, int dst)
{
    int c = 0;
        struct node * e = malloc(sizeof(struct node));
        e->c = string;
        e->dest = dst;
        TAILQ_INSERT_TAIL(head, e, nodes);
        e = NULL;
   
}

void initializeP3() {
    // printf("LOL1\n");
	sem_init(&sempt321, 0, 1);

    int i, j;
	for(i = 0; i<4; i++){
		for(j=0; j<2; j++){
	    	TAILQ_INIT(&head[i][j]);
		}
	}
        // printf("LOL2\n");

	
}

void printQueue(){
	int i, j;

	for(i = 0; i < 4; i++){
		for(j = 0; j < 2; j++){
			printf("i: %d, j: %d ", i, j);
			struct node * e = NULL;
			TAILQ_FOREACH(e, &head[i][j], nodes)
		    {
		        // e = TAILQ_FIRST(&head[i][j]);
		        printf("%d", e->c);
		    }
		    printf("\n");
		}
	}
}

void popper(){
	int num = 0;
	while(1){
		num = 0;
		sem_wait(&sempt3);
		// printf("Popper called when light is: %d\n", light);
		while(num<5){

			if(!TAILQ_EMPTY(&head[light][0])){
				// printf("dequeue from left\n");
				dequeue(light, 0);
				cars--;
				num++;
			}
			else{
				break;
			}
		}
		while(num<5){
			if(!TAILQ_EMPTY(&head[light][1])){
				// printf("dequeue from right\n");
				dequeue(light, 1);
				cars--;
				num++;
			}
			else{
				break;
			}
		}
		sem_post(&sempt32);
	}
}

void* funckTraffic(){
	light = 0;

	while(1){
		if(cars == 0){
			return;
		}
		sem_post(&sempt3);
		// printf("Waiting for popper.\n");
		sem_wait(&sempt32);
		if(light == 0){
			light = 2;
		}
		else if(light == 1){
			light = 3;
		}
		else if(light == 2){
			light = 1;
		}
		else if(light == 3){
			light = 0;
		}
		// printf("Light is now: %d\n", light);
	}
}

/**
 * If there is a car going from SOUTH to NORTH, from lane LEFT,
 * print 
 * SOUTH NORTH LEFT
 * Also, if two cars can simulateneously travel in the two lanes,
 * first print all the cars in the LEFT lane, followed by all the
 * cars in the right lane
 */
void* goingFromToP3(void *argu){
	struct argumentP3 *arg = (struct argumentP3*)(argu);
	sem_wait(&sempt321);
    // printf("%d %d %d %d\n", arg->from, arg->to, arg->lane, arg->user_id);
    insertQueue(&head[arg->from][arg->lane], arg->user_id, arg->to);
    cars++;
	sem_post(&sempt321);

}

void startP3(){
	sleep(1);
    // printQueue();
    // printf("\n");
    pthread_t tid1;
	pthread_create(&tid1, NULL, popper, NULL);

    // pthread_t tid1;
	// pthread_create(&tid1, NULL, funckTraffic, NULL);
    funckTraffic();

}
