#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

char *socket_name= "mySocket.socket";
int buff_len= 8;
int error_checker(int status, int flag){
	if(status==-1){
		if(flag==0){
			printf("Error! Couldn't create a socket!\n");
		}
		else if(flag==1){
			printf("Error! Couldn't bind to the Socket Address!\n");
		}
		else if(flag==2){
			printf("Error! Couldn't listen to the the Socket!\n");
		}
		else if(flag==3){
			printf("Error! Our Data Socket WOn't Accept the Connecting Socket\n");
		}
		else if(flag==4){
			printf("Error! Couldn't Read the Data!\n");
		}
		else if(flag==5){
			printf("Error! Couldn;t Write the Data!\n");
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
int main(int argc, char *argv[]) {
	struct sockaddr_un servername;
	int flag = 0;
	int socket_var2;
	int status;
	int m_idx = 0;
	int socket_var;
	char buffer[buff_len];
	socket_var2 = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	error_checker(socket_var2,0);
	memset(&servername, 0, sizeof(servername));
	servername.sun_family = AF_UNIX;
	strncpy(servername.sun_path, socket_name, sizeof(servername.sun_path) - 1);
	status = bind(socket_var2, (const struct sockaddr *) &servername, sizeof(servername));
	error_checker(status,1);
	status = listen(socket_var2, 100);
	error_checker(status,2);
	while(1) {
		socket_var = accept(socket_var2, NULL, NULL);
		error_checker(socket_var,3);
		m_idx = 0;
		int temp_idx = m_idx;
		while(1) {
			status = read(socket_var, buffer, sizeof(buffer));
			error_checker(status,4);
			buffer[sizeof(buffer) - 1] = 0;
			temp_idx = buffer[0];
			if(!strncmp(buffer, "DOWN", sizeof(buffer))) {
				flag = 1;
				break;
			}		
			strprinter(buffer);
			sprintf(buffer, "%d", temp_idx);
			printf("ID Sent by Process 1: %d\n", temp_idx);		
			if(temp_idx == m_idx + 5) {
				m_idx = temp_idx;
				status = write(socket_var, buffer, sizeof(buffer));
				error_checker(status,5);
			}	
			if(temp_idx >49) 
				break;
		}
		close(socket_var);
		if(flag){
			printf("SHUTTING SERVER");
			close(socket_var2);
			unlink(socket_name);
			exit(EXIT_SUCCESS);
			break;
		}
	}
	return 0;
}