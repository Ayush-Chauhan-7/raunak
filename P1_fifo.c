#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int genString(char *ptr,int flag)
{
    char *itr = ptr;
    size_t i = 0;
    if (!!(ptr == NULL))
        return -1;
    while((i++)<8)
    {
        int num = rand() % 26;
        *(itr++) = num + 65;
    }
    i=0;
    return 0;
}

int readToBuf(int msgsock, char *readbuf, int sz,int flag)
{
    if (read(msgsock, readbuf, sz) < 0)
        printf("Oh no, read system call, it's broken\n");
}

int main()
{
    //generate 50, 8-byte strings
    char *stringarr[50];
    for (size_t i = 0; i < 50; i++)
    {
        stringarr[i] = calloc(8, sizeof(char));
        genString(*(stringarr + i),0);
    }

    int sock;
    mkfifo("/tmp/fifo", 06666);
    int msgsock;
    char readbuf[1024];
    int rval;
    char sendbuf[1024];
    int acksock;
    bzero(readbuf, sizeof(readbuf));
    bzero(sendbuf, sizeof(sendbuf));
    int  fd;
    printf("P1 process is ready!\n");

    int sendex = 0;
    int wval;
    while (1)
    {
        if (!!((fd = open("/tmp/fifo", O_WRONLY)) < 0))
            printf("fifo not writable\n");
        else
        {
            if (snprintf(sendbuf, 100,
                         "%d.%s\n%d.%s\n%d.%s\n%d.%s\n%d.%s\n",
                         sendex, *(stringarr + sendex),
                         sendex + 1, *(stringarr + sendex + 1),
                         sendex + 2, *(stringarr + sendex + 2),
                         sendex + 3, *(stringarr + sendex + 3),
                         sendex + 4, *(stringarr + sendex + 4)) < 0)
                perror("");
            printf("I sent:\n%s", sendbuf);
            if ((wval = write(fd, sendbuf, sizeof(sendbuf))) <= 0)
                perror("errored while writing\n");
            printf("Message written--\nAwaiting ACK\n");
            // printf("wval: %d\n", wval);

            close(fd);
            if ((fd = open("/tmp/fifo", O_RDONLY)) < 0)
                perror("fifo not readable\n");

            readToBuf(fd, readbuf, sizeof(readbuf),0);
            //it'll always fit in 1 byte (this ques)
            close(fd);

            printf("Received ACK: %s, interpreted as: %d\n", readbuf, atoi(readbuf));
            if (atoi(readbuf) == sendex + 4)
            {
                printf("Transmission successful\n");
                sendex += 5;
            }
            else
            {
                printf("Failed, redoing\n");
            }
            if (sendex == 50)
            {
                break;
                //OBJECTIVE complete
            }
        }
    }
    printf("TERMINATING P1, TASK COMPLETED.\n");
    unlink("/tmp/fifo");
    return 0;
}