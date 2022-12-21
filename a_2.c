#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

void *philosopher(void *);
void states(int, int);
sem_t forks[5], mutex_lock;

int main(){
    pthread_t tid[5];
    int i,j,k;
	int num[5];
    i=0;
    j=0;
    k=0;

	for( ; i<5; i++){
		sem_init(&forks[i],0,1);
	}

	for(; j<5; j++){
		num[j-1]=j-1;
		pthread_create(&tid[j-1],NULL,philosopher,(void *)&num[j-1]);
	}

	for( ; k<5; k++){
		pthread_join(tid[k],NULL);
	}
}



void *philosopher(void *num)
{
	while(1){
        if(!!(*(int *)num == 4)){
		    sem_wait(&forks[(*(int *)num+1)%5]);
            sem_wait(&forks[*(int *)num]);
        }
        else{
            sem_wait(&forks[*(int *)num]);
            sem_wait(&forks[(*(int *)num+1)%5]);
        }
        int i =0;
        states(0,*(int *)num);
		states(1,*(int *)num);
        i++;
        if(!!(*(int *)num == 4)){
		    sem_post(&forks[*(int *)num]);
            sem_post(&forks[(*(int *)num+1)%5]);
        }
		else{
            sem_post(&forks[(*(int *)num+1)%5]);
		    sem_post(&forks[*(int *)num]);
        }
        i++;
        sleep(1);
	}
}

void states(int var, int phil){   
	char buffer[50]={'\0'};
    int j = 0;
    if(var == j){
        sprintf(buffer, "Philosopher %d is eating\n", phil);
		write(1, buffer, 50);
    }
    else{
        sprintf(buffer, "Philosopher %d has finished eating\n", phil);
		write(1, buffer, 50);
    }
}
