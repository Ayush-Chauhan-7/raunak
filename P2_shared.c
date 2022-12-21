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
    int k = 0;
    shmid=shmget((key_t)2345, 1024, 0666|IPC_CREAT);
    shared_memory=(char *)shmat(shmid,NULL,0);
    k++;
    for(int i=0;i<50;i+=5)
    {
        for(int j=0;j<5;j++)
        {
            while(*shared_memory==-1)
            printf("String ID: %d\n",i+j);
            k++;
            printf("String Value: %s\n",shared_memory);
            *shared_memory = -1;
        }
        printf("\n");
    }
    return 0;
}
