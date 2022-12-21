#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<time.h>

char *socket_name= "mySocket.socket";
int buff_len= 6;

void randstrgenerator(char randstr[][6]){
    srand(time(NULL));
    for(int k=1;k<51;k++){
        randstr[k][0]=k;
    }
    for(int i=1;i<51;i++){
        for(int j=1;j<6;j++){
            randstr[i][j]=rand()%26 +65;
        }
    }
}
void error_check(int status, int flag){
	if(status==-1){
		if(flag==0){
			printf("Error! Couldn't create a socket!\n");
		}
		else if(flag==1){
			printf("Error! The Server if Down!\n");
		}
		else if(flag==2){
			printf("Error! Couldn't write using the socket!\n");
		}
		else if(flag==3){
			printf("Error! Couldn't write using the socket\n");
		}
		exit(1);
	}
}
void strprinter(char st[]){
    for(int i=1;i<6;i++){
        printf("%c",st[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
	struct sockaddr_un addr;
	int status;
	int socket_var;
	char buffer[buff_len];
	int l_idx=1;
	char randstr[51][6] = {{-1}};
	randstrgenerator(randstr);
	socket_var = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	error_check(socket_var,0);
	memset(&addr,0,sizeof(addr));
	addr.sun_family=AF_UNIX;
	strncpy(addr.sun_path,socket_name,sizeof(addr.sun_path)-1);
	status= connect(socket_var,(const struct sockaddr *)&addr,sizeof(addr));
	error_check(status,1);
	for(int i=1;i<51;i++){
            printf("%d ",randstr[i][0]);
            strprinter(randstr[i]);
        }
	printf("The above Strings have been randomly generated!\n");
	if(argc <=1) {
		while(1) {
			printf("Sending Strings from %d to %d to Process P2;\n",l_idx,l_idx+4);
			for(int i=l_idx;i<l_idx+5;i++) {
				status = write(socket_var, randstr[i], strlen(randstr[i]) + 1);
				error_check(status,2);
			}
			status= read(socket_var, buffer, sizeof(buffer));
			error_check(status,3);
			buffer[sizeof(buffer) - 1] = 0;
			int temp_idx = atoi(buffer);
			printf("Max ID sent by Process 2: %d\n", temp_idx);
			if(temp_idx>49){
				printf("Successfully sent all Strings\n");
				strncpy(buffer,"DOWN",sizeof("DOWN"));
				write(socket_var,buffer,sizeof(buffer));
				close(socket_var);
				break;
			}
			else{
				l_idx=temp_idx+1;
			}
		}
	}else {
		strncpy(buffer, "DOWN", sizeof("DOWN"));
		write(socket_var, buffer, sizeof(buffer));
		close(socket_var);
	}
	return 0;
}