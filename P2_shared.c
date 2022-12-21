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

int main()
{
    char *shared_memory;  
    int shmid;  
    int i = 0;
    shmid=shmget((key_t)2345, 1024, 0666|IPC_CREAT);
    shared_memory=(char *)shmat(shmid,NULL,0);
    int j = 0;
    while(i<50)
    {
        while(j<5)
        {
            while(*shared_memory==-1)
            printf("String ID: %d\n",i+j);
            j++;
            printf("String Value: %s\n",shared_memory);
            *shared_memory = -1;
        }
        i+=5;
        printf("\n");
    }
    return 0;
}