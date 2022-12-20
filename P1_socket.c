#include<stdio.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<time.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

struct myStruct{
    char* myIdx;
    char* myStr;
};

void int_to_char(int curr, char **ptr, int flag){
    if(!!(curr>=10)){
        (*ptr)[0] = '0'+curr/10;
        (*ptr)[1] = '0'+curr%10;
        (*ptr)[2] ='\0';
    }
    else{
        (*ptr)[0] = '0'+curr;
        (*ptr)[1] = '\0';
    }
}

int char_to_int(char** ptr,int flag){
    int ans[1]={0};
    int i = 0;
    while((*ptr)[i]!='\0'){
        ans[0] *= 10;
        ans[0] += (int)((*ptr)[i]-'0');
        i++;
    }
    return ans[0];
}

void generate_n_rand_str(struct myStruct** myData, int n, int l, int flag){
    srand(time(NULL));
    *myData = (struct myStruct*) malloc(n*sizeof(struct myStruct));
    int curr = 0;
    while(curr<n){
        (*myData)[curr].myStr = (char*) malloc((l)*sizeof(char));
        (*myData)[curr].myIdx = (char*) malloc((curr<10?2:3)*sizeof(char));
        int_to_char(curr, &(*myData)[curr].myIdx,0);

        for(int i = 0; i <= l-2; i++){
            (*myData)[curr].myStr[i] = 33 + rand()%62;
        }
        (*myData)[curr].myStr[l-1] = '\0';
        curr++;
    }
}

void send_t_rand_str(struct myStruct* myData, int n, int l, int t, int* start){
    struct sockaddr_un address;
	int fd;

	address.sun_family = AF_UNIX;
	memcpy(address.sun_path, "./sockP1", strlen("./sockP1") + 1); 

	if((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1){
		perror("Socket cannot be initialized!");
		exit(EXIT_FAILURE);
	}

	unlink("./sockP1");
	if(bind(fd, (struct sockaddr*) &address, sizeof(address)) == -1){
		perror("Socket cannot be bound!");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_un destination;
	destination.sun_family = AF_UNIX;
	memcpy(destination.sun_path, "./sockP2", strlen("./sockP2") + 1); 

    for(int i=*start; i<MIN(*start+t,n); i++){
		// printf("Write a message: ");
		sendto(fd, myData[i].myIdx, (i<10?2:3), 0, (struct sockaddr*) &destination, sizeof(destination));
		sendto(fd, myData[i].myStr, l, 0, (struct sockaddr*) &destination, sizeof(destination));
		// connection_fd is marked as connected 
		// and it knows where the message should be directed
        printf("%s %s\n", myData[i].myIdx, myData[i].myStr);
    }

	close(fd); 
}

int receive_last_rand_str(struct myStruct** myData, int n, int l, int *start){
    struct sockaddr_un address;
	int fd;

	if((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1){
		perror("Socket cannot be initialized!");
		exit(EXIT_FAILURE);
	}

	address.sun_family = AF_UNIX;
	memcpy(address.sun_path, "./sockP1", strlen("./sockP1") + 1);

	unlink("./sockP1");
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
    // printf("Write a message: ");
    size = recvfrom(fd, temp->myIdx, ((*start)<10?2:3), 0, (struct sockaddr *) &emitter, &len);
    if(size == -1) { 
        if(errno == ECONNRESET) printf("ECONNRESET\n");
        close(fd);
        perror("Receiver"); exit(EXIT_FAILURE); 
    }
    size = recvfrom(fd, temp->myStr, l, 0, (struct sockaddr *) &emitter, &len);
    if(size == -1) { 
        if(errno == ECONNRESET) printf("ECONNRESET\n");
        close(fd);
        perror("Receiver"); exit(EXIT_FAILURE); 
    }
    // connection_fd is marked as connected 
    // and it knows where the message should be directed
    printf("%s\n", temp->myIdx);
    // printf("%s %s\n", temp->myIdx, temp->myStr);

    int r_val = char_to_int(&temp->myIdx,0);
	free(temp);
	close(fd);
    return r_val;
}

int main(int argc, const char* argv[]){
    struct myStruct* myData;
    int num_of_rand_str = 50;
    int len_of_rand_str = 6;
    generate_n_rand_str(&myData, num_of_rand_str, len_of_rand_str,0) ;
    int start = 0;
    while(start<num_of_rand_str){
        printf("Sent data:\n");
        send_t_rand_str(myData, num_of_rand_str, len_of_rand_str, 5, &start);
        printf("----------------\n");
        printf("Received data:\n");
        start = receive_last_rand_str(&myData, num_of_rand_str, len_of_rand_str, &start)+1;
        printf("----------------\n");
        sleep(1);
    }

    return 0;
}
