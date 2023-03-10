#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/un.h>

#define LOCAL       "./sockP2"
#define DESTINATION "./sockP1" 

struct myStruct{
    char* myIdx;
    char* myStr;
};

void receive_t_rand_str(struct myStruct** myData, int n, int l, int* start, int flag){
	struct sockaddr_un address;
	int fd;

	if((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1){
		perror("Socket cannot be initialized!");
		exit(EXIT_FAILURE);
	}

	address.sun_family = AF_UNIX;
	memcpy(address.sun_path, LOCAL, strlen(LOCAL) + 1);

	unlink(LOCAL);
	if(bind(fd, (struct sockaddr*) &address, sizeof(address)) == -1){
		perror("Socket cannot be bound!");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_un emitter;
	socklen_t len;

	struct myStruct *temp = (struct myStruct*) malloc(sizeof(struct myStruct));
	temp->myIdx = (char*) malloc(sizeof(char)*((*start)<10?2:3));
	temp->myStr = (char*) malloc(sizeof(char)*6);
	size_t size;
	int ii = 0;
	while(ii!=5){
		// printf("Write a message: ");
		size = recvfrom(fd, temp->myIdx, ((*start)<10?2:3), 0, (struct sockaddr *) &emitter, &len);
		if(size == -1) { 
			if(errno == ECONNRESET) printf("ECONNRESET\n");
			close(fd);
			perror("Receiver"); exit(EXIT_FAILURE); 
		}
		if((*start)!=0 && strcmp(temp->myIdx, (*myData)[(*start)-1].myIdx)==0){
			break;
		}
        memcpy((*myData)[*start].myIdx, temp->myIdx, ((*start)<10?2:3));
		size = recvfrom(fd, temp->myStr, l, 0, (struct sockaddr *) &emitter, &len);
		if(size == -1) { 
			if(errno == ECONNRESET) printf("ECONNRESET\n");
			close(fd);
			perror("Receiver"); exit(EXIT_FAILURE); 
		}
        memcpy((*myData)[*start].myStr, temp->myStr, l);

        printf("%s %s\n", (*myData)[*start].myIdx, (*myData)[*start].myStr);
		*start=*start+1;
		if(*start>=n){
			break;
		}
		ii++;
	}
	free(temp);
	close(fd);
}

void send_last_rand_str(struct myStruct* myData, int n, int l, int *start,int flag){
    struct sockaddr_un address;
	int fd;

	address.sun_family = AF_UNIX;
	memcpy(address.sun_path, LOCAL, strlen(LOCAL) + 1); 

	if((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1){
		perror("Socket cannot be initialized!");
		exit(EXIT_FAILURE);
	}

	unlink(LOCAL);
	if(bind(fd, (struct sockaddr*) &address, sizeof(address)) == -1){
		perror("Socket cannot be bound!");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_un destination;
	destination.sun_family = AF_UNIX;
	memcpy(destination.sun_path, DESTINATION, strlen(DESTINATION) + 1); 

	sendto(fd, myData[(*start)-1].myIdx, ((*start)-1<10?2:3), 0, (struct sockaddr*) &destination, sizeof(destination));
	sendto(fd, myData[(*start)-1].myStr, l, 0, (struct sockaddr*) &destination, sizeof(destination));

	printf("%s\n", myData[(*start)-1].myIdx);
	// printf("%s %s\n", myData[(*start)-1].myIdx, myData[(*start)-1].myStr);

	close(fd); 
}


int main(int argc, const char* argv[]){
    int num_of_rand_str = 50;
    int len_of_rand_str = 6;

    struct myStruct *myData = (struct myStruct*) malloc(num_of_rand_str*sizeof(struct myStruct));
	
	for (int i = 0; i < num_of_rand_str; i++){
        myData[i].myStr = (char*) malloc((len_of_rand_str)*sizeof(char));
        myData[i].myIdx = (char*) malloc((i<10?2:3)*sizeof(char));
	}
	
	int start=0;
	while(start<num_of_rand_str){
        printf("Received data:\n");
		receive_t_rand_str(&myData, num_of_rand_str, len_of_rand_str, &start,0);
        printf("----------------\n");
		sleep(1);
		printf("Sent data:\n");
		send_last_rand_str(myData, num_of_rand_str, len_of_rand_str, &start,0);
        printf("----------------\n");
	}

    return 0;
}
