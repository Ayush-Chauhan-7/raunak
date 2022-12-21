#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdbool.h>

bool status[5]={0,0,0,0,0};
sem_t forks[5];
sem_t common_sem;

void eating(int curr);
void* philo(void* args);

int main(void) {
    pthread_t thread1[5];
    sem_init(&common_sem, 0, 1);
    int num[5]={0,1,2,3,4};
    for(int i=0;i<5;i++) {
        sem_init(&forks[i], 0, 1);
    }

    for(int i=0;i<5;i++) {
        pthread_create(&thread1[i], NULL, philo, &num[i]);
    }

    for(int i=0;i<5;i++) {
        pthread_join(thread1[i], NULL);
    }
}

void eating(int curr) {
    if((!status[(curr-1)%5]) && (!status[(curr+1)%5])) {

         if(curr<4) {
            status[curr]=1;
            sem_wait(&forks[curr]);
            sem_wait(&forks[(curr+1)%5]);
            srand(time(0));
            
            printf("Philosopher %d picked up forks %d and %d and is eating\n", curr, curr, (curr+1)%5);
            usleep(1000000);
            
            sem_post(&forks[curr]);
            sem_post(&forks[(curr+1)%5]);
            status[curr]=0;
        }

        else {
            status[curr]=1;
            sem_wait(&forks[(curr+1)%5]);
            sem_wait(&forks[curr]);
            srand(time(0));
            
            printf("Philosopher %d picked up forks %d and %d and is eating\n", curr, curr, (curr+1)%5);
            usleep(1000000);
            
            sem_post(&forks[(curr+1)%5]);
            sem_post(&forks[curr]);
            status[curr]=0;
        }
    
    }
}

void* philo(void* args) {
    while(1) {
        int curr=*((int*)args);
        eating(curr);
    }
}

