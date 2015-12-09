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
}Resources;


Resources Max[NUMBER_OF_CUSTOMERS];
Resources Allocated[NUMBER_OF_CUSTOMERS];
Resources Need[NUMBER_OF_CUSTOMERS];
int available[3]={0,0,0};
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

return 0;
}
int release_resources(int customer_number, int release[]){

return 0;
}

void *run(void *t)
{	
	int i;
	int x=(int)t;
	while(Need[x-1].type1_resouces>0&&Need[x-1].type2_resouces>0&&Need[x-1].type3_resouces>0){
		int request[3]={rand()%Need[x-1].type1_resouces,rand()%Need[x-1].type2_resouces,rand()%Need[x-1].type3_resouces};
		if(!request_resources(x,request)){
			printf("Customer %d request is for resourcses of type 1=%d, type2=%d, type3=%d is granted\n",x,request[0],request[1],request[2]);
			sleep(2);
			int release[3]={rand()%Allocated[x-1].type1_resouces,rand()%Allocated[x-1].type2_resouces,rand()%Allocated[x-1].type3_resouces};
			release_resources(x,release);
		}
	}
	int release[3]={Allocated[x-1].type1_resouces,Allocated[x-1].type2_resouces,Allocated[x-1].type3_resouces};
	release_resources(x,release);
	printf("Customer %d terminated",x);
	pthread_exit();
}
int main(int argc, char *argv[])
{
	
	
	int i;
	srand(time(0));
	loadMax();
	loadAllocated();
	loadNeed();
	/*for(i=0;i<argc;i++){
	available[i-1]=atoi(argv[i]);
}
printf("type1=%d, type2=%d, type3=%d\n",available[0],available[1],available[2]);
	for( i=0;i<5;i++){
	printf("Max: cid=%d, t1=%d, t2=%d, t3=%d\n",Max[i].customer_number,Max[i].type1_resouces,Max[i].type2_resouces,Max[i].type3_resouces);
	}
for( i=0;i<5;i++){
	printf("Allocated: cid=%d, t1=%d, t2=%d, t3=%d\n",Allocated[i].customer_number,Allocated[i].type1_resouces,Allocated[i].type2_resouces,Allocated[i].type3_resouces);
	}
for( i=0;i<5;i++){
	printf("Need: cid=%d, t1=%d, t2=%d, t3=%d\n",Need[i].customer_number,Need[i].type1_resouces,Need[i].type2_resouces,Need[i].type3_resouces);
	}*/
	pthread_t th[NUMBER_OF_CUSTOMERS];
	int t;
	for(t = 0; t < NUMBER_OF_CUSTOMERS; t++)
	{
		printf("Creating thread %d\n", t);
		pthread_create(&th[t], NULL, run, (void *)t+1);
	}
	
	/*for(t = 0; t < MAX; t++)
	{
		void* n;
		printf("Joining thread %d\n", t);
		pthread_join(th[t], &n);
		sum=sum+(int)n;
	
	}*/
	
	printf("The sum = %d\n",sum);
	pthread_exit(NULL);
return 0;
}