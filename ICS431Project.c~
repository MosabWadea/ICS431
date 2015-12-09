#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <semaphore.h>
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

typedef struct
{
	int customer_number;
	int type1_resouces;
	int type2_resouces;
	int type3_resouces; 
}MaxRequestedResources;

typedef struct
{
	int customer_number;
	int type1_resouces;
	int type2_resouces;
	int type3_resouces; 
}NeededResources;

typedef struct
{
	int customer_number;
	int type1_resouces;
	int type2_resouces;
	int type3_resouces; 
}AllocatedResources;

MaxRequestedResources Max[NUMBER_OF_CUSTOMERS];
AllocatedResources Allocated[NUMBER_OF_CUSTOMERS];
NeededResources Need[NUMBER_OF_CUSTOMERS];
sem_t m;

void loadMax(){
int i;
for(i=0;i<5;i++){
	Max[i].customer_number=i+1;
	Max[i].type1_resouces=rand()%10;
	Max[i].type2_resouces=rand()%10;
	Max[i].type3_resouces=rand()%10;
}
}

void loadNeed(){
int i;
for(i=0;i<5;i++){
	Need[i].customer_number=Max[i].customer_number;
	Need[i].type1_resouces=Max[i].type1_resouces;
	Need[i].type2_resouces=Max[i].type2_resouces;
	Need[i].type3_resouces=Max[i].type3_resouces;
}
}

void loadAllocated(){
int i;
for(i=0;i<5;i++){
	Allocated[i].customer_number=i+1;
	Allocated[i].type1_resouces=0;
	Allocated[i].type2_resouces=0;
	Allocated[i].type3_resouces=0;
}
}

int request_resources(int customer_number, int request[]){

}
int release_resources(int customer_number, int release[]){
}

void *run()
{
	
	sleep(1);
	int number = rand()%10;
	printf("The random number is %d\n",number);
	pthread_exit((void *)number);
}
int main()
{
	int i;
	srand(time(0));
	loadMax();
	loadAllocated();
	loadNeed();
	for( i=0;i<5;i++){
	printf("Max: cid=%d, t1=%d, t2=%d, t3=%d\n",Max[i].customer_number,Max[i].type1_resouces,Max[i].type2_resouces,Max[i].type3_resouces);
	}
for( i=0;i<5;i++){
	printf("Allocated: cid=%d, t1=%d, t2=%d, t3=%d\n",Allocated[i].customer_number,Allocated[i].type1_resouces,Allocated[i].type2_resouces,Allocated[i].type3_resouces);
	}
for( i=0;i<5;i++){
	printf("Need: cid=%d, t1=%d, t2=%d, t3=%d\n",Need[i].customer_number,Need[i].type1_resouces,Need[i].type2_resouces,Need[i].type3_resouces);
	}
	/*pthread_t th[MAX];
	int t;
	int sum=0;
	for(t = 0; t < MAX; t++)
	{
		printf("Creating thread %d\n", t);
		pthread_create(&th[t], NULL, run, NULL);
	}
	
	for(t = 0; t < MAX; t++)
	{
		void* n;
		printf("Joining thread %d\n", t);
		pthread_join(th[t], &n);
		sum=sum+(int)n;
	
	}
	
	printf("The sum = %d\n",sum);
	pthread_exit(NULL);*/
return 0;
}
