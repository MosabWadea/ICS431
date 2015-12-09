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
	int j;
	for(j=0; j<NUMBER_OF_RESOURCES; j++)
		Max[i].resouce[j]=rand()%10;
}
}

void loadNeed(){
int i;
for(i=0;i<5;i++){
	Need[i].customer_number=Max[i].customer_number;
	for(j=0; j<NUMBER_OF_RESOURCES; j++)
		Need[i].resouce[j]=Max[i].resouce[j];
}
}

void loadAllocated(){
	int i;
	for(i=0;i<5;i++){
		Allocated[i].customer_number=i+1;
		for(j=0; j<NUMBER_OF_RESOURCES; j++)
			Allocated[i].resouce[j]=0;
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
			int k;
			int flag = 1;
			for(k=0; k<NUMBER_OF_RESOURCES; k++){
				if(need[j].resource[k]<=work[k]){
					flag = 0;
					break;
				}
			}		
			if(finish[j]==0 && flag){
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
		//init the pretending arrays
		int pavailable[3];
		Resources pneed[NUMBER_OF_CUSTOMERS] = Need;
		Resources pallocated[NUMBER_OF_CUSTOMERS] = Allocated;
		//populating the pretending arrays
		for(r=0; r<NUMBER_OF_RESOURCES; r++){
			pavailable[r] = available[r] - request[r];
			pneed[cn-1].resource[r] = Need[cn-1].resouce[r] - request[r];
			pallocated[cn-1].resouce[r] = Allocated[cn-1].resouce[r] + request[r];
		}		

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
	while(Need[x-1].resource[0]>0&&Need[x-1].resource[1]>0&&Need[x-1].resource[2]>0){
		int request[3]={rand()%Need[x-1].resource[0],rand()%Need[x-1].resource[1],rand()%Need[x-1].resource[2]};
		if(!request_resources(x,request)){
			printf("Customer %d request is for resourcses of type 1=%d, type2=%d, type3=%d is granted\n",x,request[0],request[1],request[2]);
			sleep(2);
			int release[3]={rand()%Allocated[x-1].resource[0],rand()%Allocated[x-1].resource[1],rand()%Allocated[x-1].[2]};
			release_resources(x,release);
		}
	}
	int release[3]={Allocated[x-1].resource[0],Allocated[x-1].resource[1],Allocated[x-1].resource[2]};
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
