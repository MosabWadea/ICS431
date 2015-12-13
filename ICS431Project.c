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
int available[NUMBER_OF_RESOURCES];
sem_t m;

////// populating Max array
void loadMax(){
	int i;
	for(i=0;i<NUMBER_OF_CUSTOMERS;i++){
		Max[i].customer_number=i;
		int j;
		for(j=0; j<NUMBER_OF_RESOURCES; j++)
			Max[i].resource[j]=(rand()+1)%10;
	}
}

////// populating Need array
void loadNeed(){
	int i,j;
	for(i=0;i<NUMBER_OF_CUSTOMERS;i++){
		Need[i].customer_number=Max[i].customer_number;
		for(j=0; j<NUMBER_OF_RESOURCES; j++)
			Need[i].resource[j]=Max[i].resource[j];
	}
}

////// populating Allocate array
void loadAllocated(){
	int i,j;
	for(i=0;i<NUMBER_OF_CUSTOMERS;i++){
		Allocated[i].customer_number=i;
		for(j=0; j<NUMBER_OF_RESOURCES; j++)
			Allocated[i].resource[j]=1;
	}
}


////// Saftey Algorithem
int safetyAlgorithm(int work[],Resources need[], Resources allocated[]){
	int finish[NUMBER_OF_CUSTOMERS];
	int i, j;
	int k;
	int flag = 1;
	
	//init finish array
	for(i=0;i<NUMBER_OF_CUSTOMERS;i++){
		finish[i]=0;
	}
	
	//safety algorithm
	for(i=0;i<NUMBER_OF_CUSTOMERS;i++){
		for(j=0; j<NUMBER_OF_CUSTOMERS; j++){
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


////// Banker's Algorithem
int request_resources(int customer_number, int request[]){
	sem_wait(&m);
	int cn=customer_number;
	int t,r;
	int x;
	int flag1, flag2;
	for(x=0; x<NUMBER_OF_RESOURCES; x++){
		flag1 = 0;
		flag2 = 0;
		if(request[x]>Need[cn].resource[x])
			flag1 = 1;
		if(request[x]>available[x])
			flag2 = 1;
	}
		
	if(flag1){
		printf("Customer %d : req > need!!\n",cn);
		sem_post(&m);
		return -1;
	}
	else if(flag2){
		printf("Customer %d : req > available!! Deny\n",cn);
		sem_post(&m);
		return -1;
	}
	else{
		printf("Customer %d is in Banker Algorithem, and it is running.\n",cn);
		//init the pretending arrays
		int pavailable[3];
		Resources pneed[NUMBER_OF_CUSTOMERS];
		Resources pallocated[NUMBER_OF_CUSTOMERS];
		for(t=0;t<NUMBER_OF_CUSTOMERS;t++){
			pneed[t] = Need[t];
			pallocated[t]=Allocated[t];
		}
		//populating the pretending arrays
		for(r=0; r<NUMBER_OF_RESOURCES; r++){
			pavailable[r] = available[r] - request[r];
			pneed[cn].resource[r] = Need[cn].resource[r] - request[r];
			pallocated[cn].resource[r] = Allocated[cn].resource[r] + request[r];
		}		

		if(!safetyAlgorithm(pavailable,pneed,pallocated)){
			for(r=0; r<NUMBER_OF_RESOURCES; r++){
				available[r] = pavailable[r];
			}
			for(t=0;t<NUMBER_OF_CUSTOMERS;t++){
			Need[t] = pneed[t];
			Allocated[t]=pallocated[t];
		}
			printf("THE REQUEST WAS GRANTED FOR CUSTOMER %d.\n",cn);
			for(r=0; r<NUMBER_OF_RESOURCES; r++){
				printf(" Need[%d]= %d\n",r,Need[r]);
			}
			sem_post(&m);
			return 0;
		}
		printf("the request for customer %d is denied.\n",cn);
		sem_post(&m);
		return -1;

	}
}

////// Releasing Resources Function
int release_resources(int customer_number, int release[]){
	sem_wait(&m);
	int cn=customer_number;
	int i;
	for(i=0; i<NUMBER_OF_RESOURCES;i++){
		if(Allocated[cn].resource[i]<release[i]){
			printf("Customer %d release resources are more than allocation\n",cn);
			return -1;
		}
	}
	for(i=0; i<NUMBER_OF_RESOURCES;i++){
		available[i]+=release[i];
		Allocated[cn].resource[i]-=release[i];
	}
	printf("Customer %d released resources\n",cn);
	sem_post(&m);
	return 0;
}


////// Customer Excution instructions
void *run(void *t)
{	
	int i,sum;
	int request[NUMBER_OF_RESOURCES],release[NUMBER_OF_RESOURCES];
	int x=(int)t;
	
	//int flag = 0;
	do{	
	
		for(i=0;i<NUMBER_OF_RESOURCES;i++){
			request[i]=((rand()%10)%Need[x].resource[i])+1;
			printf("Customer %d : Request [%d] = %d\n",x,i,request[i]);
		}
		
		if(!request_resources(x,request)){
			printf("Customer %d request is for resourcses of type 1=%d, type2=%d, type3=%d is granted\n",x,request[0],request[1],request[2]);
			sleep(2);
			for(i=0;i<NUMBER_OF_RESOURCES;i++){
				release[i] =( (rand()%10)%Allocated[x].resource[i])+1;
				printf("release%d = %d\t allocate = %d \n", i, release[i], Allocated[x].resource[i]);
			}
			release_resources(x,release);
		}
		int q;
		sum=0;
		printf("start for loop and sum=%d for customer= %d\n",sum,x);
		for(q=0; q<NUMBER_OF_RESOURCES; q++){
			sum += Need[x].resource[q];
			//flag=sum;
		}
	printf("sum = %d\n",sum);
	}while(sum>0);
	printf("end of do/while.\n");
	for(i=0;i<NUMBER_OF_RESOURCES;i++){
				release[i]=Allocated[x].resource[i];
			}
	release_resources(x,release);
	printf("Customer %d terminated\n",x);
	pthread_exit(NULL);
}

////// Main Function
int main(int argc, char *argv[]){
	pthread_t th[NUMBER_OF_CUSTOMERS];
	int t;
	int x;
	
	srand(time(0));
	loadMax();
	loadNeed();
	loadAllocated();
	for(x = 0; x < argc - 1; x++){
		available[x]= atoi(argv[x+1]);
		printf("avalilable[%d]=%d\n",x,available[x]);
		}
	
	//init semaphore
	sem_init(&m, 0, 1);
	printf("****\n");
	for(t = 0; t < NUMBER_OF_CUSTOMERS; t++)
	{
		printf("Creating customer %d\n", t);
		pthread_create(&th[t], NULL, run, (void *)t);
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
