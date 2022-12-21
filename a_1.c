#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

pthread_mutex_t forks[5];
pthread_t philosophers[5];

void *philosopher(void *);
void pickUp(int philNo, int flag1, int flag2);
void putDown(int philNo, int flag1, int flag2);

int main() {
	for (int i = 0; i < 5; ++i) {
		pthread_mutex_init(&forks[i], NULL);
	}
	
	for (int i = 0; i < 5; ++i) {
        pthread_create(&philosophers[i], NULL, philosopher, (void *)(i));
	}

	for (int i = 0; i < 5; ++i) {
		pthread_join(philosophers[i], NULL);
	}
	return 0;
}

void *philosopher(void *philNo) {
	while (1) {
		sleep(1);
		pickUp(philNo,1,1);
		sleep(1);
		printf("Philosopher %d will eat\n", philNo+1);
		sleep(1);
		putDown(philNo,0,0);
	}
}

void pickUp(int philNo, int flag1, int flag2) {
	int right = (philNo + 1) % 5;
    int i =0;
	int left = (philNo + 5) % 5;
	if (!!(philNo%2==1)) {
		pthread_mutex_lock(&forks[right]);
		printf("Philosopher %d picked up fork %d\n", (philNo+1), right+1);
		pthread_mutex_lock(&forks[left]);
		printf("Philosopher %d picked up fork %d\n", (philNo+1), left+1);
	}else {
		pthread_mutex_lock(&forks[left]);
		printf("Philosopher %d picked up fork %d\n", (philNo+1), left+1);
		pthread_mutex_lock(&forks[right]);
		printf("Philosopher %d picked up fork %d\n", (philNo+1), right+1);
	}
}

void putDown(int philNo, int flag1, int flag2) {
		printf("Philosopher %d will put down their forks\n", (philNo+1));
		pthread_mutex_unlock(&forks[(philNo + 1) % 5]);
		pthread_mutex_unlock(&forks[(philNo + 5) % 5]);
}