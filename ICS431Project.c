#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <semaphore.h>


#define NUMBER_OF_CUSTOMERS 3
#define NUMBER_OF_RESOURCES 3

/// Max, Allocated, Need arrays the will contain this struct
typedef struct
{
    int customer_number;
    int resource[NUMBER_OF_RESOURCES];
}Resources;

/// it will contain the maximum number of resources from each type for All customers
Resources Max[NUMBER_OF_CUSTOMERS];

/// it will contain the number of allocated resources from each type for All customers
Resources Allocated[NUMBER_OF_CUSTOMERS];

/// it will contain the number of resources from each type that customers needs.
Resources Need[NUMBER_OF_CUSTOMERS];


/// it will contain the number of allocated resources from each type for All customers
int available[NUMBER_OF_RESOURCES];

//declaring the semaphore
sem_t m;

////// populating Max array
void loadMax(){
    int i;
    for(i=0;i<NUMBER_OF_CUSTOMERS;i++){
        Max[i].customer_number=i;
        int j;
        for(j=0; j<NUMBER_OF_RESOURCES; j++)
            Max[i].resource[j]=(rand()%10)+1;
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
            Allocated[i].resource[j]=0;
    }
}


///////////////////////    Saftey Algorithem     ////////////////////////////////
int safetyAlgorithm(int work[],Resources need[], Resources allocated[]){
    int finish[NUMBER_OF_CUSTOMERS];
    int i, j, k, flag;
    
    //init finish array
    for(i=0;i<NUMBER_OF_CUSTOMERS;i++){
        finish[i]=0;
    }
    
    //safety algorithm
    for(i=0;i<NUMBER_OF_CUSTOMERS;i++){
        for(j=0; j<NUMBER_OF_CUSTOMERS; j++){
            flag = 1;
            for(k=0; k<NUMBER_OF_RESOURCES; k++){
                if(need[j].resource[k]<=work[k]){
                    flag = 0;
                    break;
                }
            }
            if(finish[j]==0 && !flag){
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
////////////////////////////////////////////////////



////// Banker's Algorithem ///////////////////////////
int request_resources(int customer_number, int request[]){
    sem_wait(&m);
    int cn=customer_number;
    int t,r;
    int x;
    int flag1, flag2;
    for(x=0; x<NUMBER_OF_RESOURCES; x++){
        flag1 = 0;
        flag2 = 0;
        if(request[x]>Need[cn].resource[x]){
            printf("Customer %d : req > need!!\n",cn);
            sem_post(&m);
            return -1;
        }
        if(request[x]>available[x]){
            printf("Customer %d : req > available!! Deny\n",cn);
            sem_post(&m);
            return -1;
        }
    }
    
    
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

    /// Checking the Safety Algorithm
    if(safetyAlgorithm(pavailable,pneed,pallocated)){
        for(r=0; r<NUMBER_OF_RESOURCES; r++){
            available[r] -= request[r];
        }
        for(t=0;t<NUMBER_OF_CUSTOMERS;t++){
            for(r=0; r<NUMBER_OF_RESOURCES; r++){
                Need[cn].resource[r] = pneed[cn].resource[r];
                Allocated[cn].resource[r] = pallocated[cn].resource[r];
            }
        }
        printf("THE REQUEST WAS GRANTED FOR CUSTOMER %d.\n",cn);
        sem_post(&m);
        return 0;
    }
    printf("the request for customer %d is denied.\n",cn);
    sem_post(&m);
    return -1;

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
        //populating the request array
        for(i=0;i<NUMBER_OF_RESOURCES;i++){
            request[i] = 0;
            if(Need[x].resource[i]){
                request[i]=(rand()%(Need[x].resource[i]+1));
            }
        }
        
        // Calling Banker's Algorithm
        if(!request_resources(x,request)){
            sleep(2);
            
            //populating the release array
            for(i=0;i<NUMBER_OF_RESOURCES;i++){
                release[i] = 0;
                if(Allocated[x].resource[i]){
                    release[i] =(rand()%(Allocated[x].resource[i]+1));
                }
            }
            release_resources(x,release);
            for(i=0;i<NUMBER_OF_RESOURCES;i++){
                printf("Customer %d : Max[%d]= %d\n",x,i,Max[x].resource[i]);
            }
            for(i=0;i<NUMBER_OF_RESOURCES;i++){
                printf("Customer %d : Request[%d]= %d\n",x,i,request[i]);
            }
            for(i=0;i<NUMBER_OF_RESOURCES;i++){
                printf("Customer %d : Allocated[%d]= %d\n",x,i,Allocated[x].resource[i]);
            }
            for(i=0;i<NUMBER_OF_RESOURCES;i++){
                printf("Customer %d : Release[%d]= %d\n",x,i,release[i]);
            }
            for(i=0;i<NUMBER_OF_RESOURCES;i++){
                printf("Customer %d : Need[%d]= %d\n",x,i,Need[x].resource[i]);
            }
            for(i=0;i<NUMBER_OF_RESOURCES;i++){
                printf("Customer %d : Available[%d]=%d\n",x,i,available[i]);
            }
            sleep(2);
        }
        int q;
        sum=0;
        for(q=0; q<NUMBER_OF_RESOURCES; q++){
            sum += Need[x].resource[q];
        }
    printf("sum = %d\n",sum);
    }while(sum>0);
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

    // populating the available array
    for(x = 0; x < argc - 1; x++){
        available[x]= atoi(argv[x+1]);
        }
    
    //init semaphore
    sem_init(&m, 0, 1);

    // Creating Customers (Threads)
    for(t = 0; t < NUMBER_OF_CUSTOMERS; t++){
        printf("Creating customer %d\n", t);
        pthread_create(&th[t], NULL, run, (void *)t);
        
    }
    
    for(t = 0; t < NUMBER_OF_CUSTOMERS; t++){
        pthread_join(th[t], NULL);
    
    }

    pthread_exit(NULL);

    //release sem
    sem_destroy(&m);
    return 0;
}
