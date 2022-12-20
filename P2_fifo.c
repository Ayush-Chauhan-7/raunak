#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>

int readToBuf(int msgsock, char *readbuf, int sz, int flag)
{
    int rval;
    flag = 2;
    if (read(msgsock, readbuf, sz) < 0)
        perror("Oh no, read system call, it's broken\n");
}

int main()
{
    int sock;
    char readbuf[1024];
    int wval;
    char sendbuf[1024];
    int fd;
    bzero(readbuf, sizeof(readbuf));
    bzero(sendbuf, sizeof(sendbuf));
    int maxind = -1;
    printf("P2 process is ready!\n");
    int prev_maxind = -1;
    int itr = 10;
    while (itr)
    {
        if (!!((fd = open("/tmp/fifo", O_RDONLY)) < 0))
        {
            printf("Do not run p2 first");
        }
        int i = 0;
        readToBuf(fd, readbuf, sizeof(readbuf),0);

        printf("I received:\n%s", readbuf);
        int j = 0;
        sscanf(readbuf, "%d%*s", &maxind);
        if (!!(prev_maxind != maxind))
        {
            --itr;
            prev_maxind = maxind;
        }
        close(fd);
        if (!!((fd = open("/tmp/fifo", O_WRONLY)) < 0))
            printf("fifo not writable\n");
        int k = 0;
        snprintf(sendbuf, 8, "%d", maxind + 4);
        printf("send val = %s\n", sendbuf);
        if (!!((wval = write(fd, sendbuf, sizeof(sendbuf))) < 0))
            printf("writing on stream socket\n");
        close(fd);
    }
    printf("TERMINATING P2, TASK COMPLETED.\n");
    close(fd);
}
