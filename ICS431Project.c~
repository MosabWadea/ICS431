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
	int resource[NUMBER_OF_RESOURCES];
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
	Max[i].type1_resouces=rand()%10; //update
	Max[i].type2_resouces=rand()%10;
	Max[i].type3_resouces=rand()%10;
}
}

void loadNeed(){
int i;
for(i=0;i<5;i++){
	Need[i].customer_number=Max[i].customer_number;
	Need[i].type1_resouces=Max[i].type1_resouces; //update
	Need[i].type2_resouces=Max[i].type2_resouces;
	Need[i].type3_resouces=Max[i].type3_resouces;
}
}

void loadAllocated(){
	int i;
	for(i=0;i<5;i++){
		Allocated[i].customer_number=i+1; //update
		Allocated[i].type1_resouces=0;
		Allocated[i].type2_resouces=0;
		Allocated[i].type3_resouces=0;
	}
}

int safetyAlgorithm(int work[],int need[], int allocated[]){
	int finish[NUMBER_OF_CUSTOMERS];
	int i, j;
	
	//init finish array
	for(i=0;i<NUMBER_OF_CUSTOMERS;i++){
		finish[i]=0;
	}
	
	//safety algorithm
	for(i=0;i<NUMBER_OF_CUSTOMERS;i++){
		for(j=0; j<NUMBER_OF_CUSTOMERS; j++){			
			if(finish[j]==0 && need[j].type1_resources<=work[0] && need[j].type2_resources<=work[1] && need[j].type3_resources<=work[2]){
				int m;
				for(m=0; m<NUMBER_OF_RESOURCES; m++)
					work[m] += allocated[j].resource[m];
				finish[j] = 1;
				break;
			}
		}
	}
	
	
	int ret = 1;
	for(i=0; i<NUMBER_OF_CUSTOMERS; i++){
		ret = ret * finish[i];
	}
	return ret;
}

int request_resources(int customer_number, int request[]){
	sem_wait(&m);
	int cn=customer_number;
	int t,r;
	if(rquest[0]>Need[cn-1].type1_resouces || rquest[1]>Need[cn-1].type2_resouces || rquest[2]>Need[cn-1].type3_resouces){
		printf("req > need!!");
		sem_post(&m);
		return -1;
	}
	else if(rquest[0]>available[0] || rquest[1]>available[1] || rquest[2]>available[2]){
		printf("req > available!! Deny");
		sem_post(&m);
		return -1;
	}
	else{
		int pavailable[3];
		Resources pneed[NUMBER_OF_CUSTOMERS] = Need;
		Resources pallocated[NUMBER_OF_CUSTOMERS] = Allocated;
		for(r=0; r<NUMBER_OF_RESOURCES; r++){
			pavailable[r] = available[r] - request[r];
		}		
		pneed[cn-1].type1_resouces = Need[cn-1].type1_resouces - request[0];
		pneed[cn-1].type2_resouces = Need[cn-1].type2_resouces - request[1];
		pneed[cn-1].type3_resouces = Need[cn-1].type3_resouces - request[2];

		pallocated[cn-1].type1_resouces = Allocated[cn-1].type1_resouces + request[0];
		pallocated[cn-1].type2_resouces = Allocated[cn-1].type2_resouces + request[1];
		pallocated[cn-1].type3_resouces = Allocated[cn-1].type3_resouces + request[2];

		if(safetyAlgorithm(pavailable,pneed,pallocated)){
			for(r=0; r<NUMBER_OF_RESOURCES; r++){
			available[r] = pavailable[r];
			}
			Need=pneed;
			Allocated=pallocated;
			sem_post(&m);
			return 0;
		}
		printf("the request is denied.\n");
		sem_post(&m);
		return -1;

	}
}
int release_resources(int customer_number, int release[]){
	sem_wait(&m);
	int cn=customer_number;
	int i;
	for(i=0,i<NUMBER_OF_RESOURCES;i++){
		available[i]+=release[i];
		Allocated[cn-1].resource[i]-=release[i];
	}
	printf("Customer %d released resources",cn);
	sem_post(&m);
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
int main(int argc, char *argv[]){
	int i;
	srand(time(0));
	loadMax();
	loadAllocated();
	loadNeed();
	
	//init semaphore
	sem_init(&m, 0, 1);


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
	
	for(t = 0; t < NUMBER_OF_CUSTOMERS; t++)
	{
		void* n;
		pthread_join(th[t], &n);
	
	}
	
	pthread_exit(NULL);

	//release sem
	sem_destroy(&m);
	return 0;
}
