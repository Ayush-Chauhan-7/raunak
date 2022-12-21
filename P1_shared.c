#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<sys/shm.h>  
#include<string.h>  
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>
#include <limits.h>
#include <math.h>

int main()
{
    struct timespec stime;
    clock_gettime(CLOCK_REALTIME, &stime);
    char *shared_memory;   
    int shmid;  
    shmid=shmget((key_t)2345, 1024, 0666|IPC_CREAT); 
    shared_memory=(char *)shmat(shmid,NULL,0);
    char strings[50][7];
    int row = 0;
    char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int column = 0;
    while(row<50) {
        while(column<6) {
            strings[row][column] = alphabet[rand() % 52];
            column++;
        }
        strings[row][6]='\0';
        row++;
    }
    for(int i=0;i<50;i+=5)
    {
        for(int j=0;j<5;j++)
        {
            strcpy(shared_memory,strings[i+j]);
            while(*shared_memory!=-1);
        }
        printf("Maximum ID recieved: %d\n\n",i+4);
    }
    struct timespec etime;
    clock_gettime(CLOCK_REALTIME, &etime);
    printf("Time taken: %lf\n",(etime.tv_sec - stime.tv_sec) +(etime.tv_nsec - stime.tv_nsec) / (double)1e9);
    return 0;
}
