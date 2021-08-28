#include "ClientHeader.h"

int main(int argc, char *argv[]){
	if (argc < 2){
		printf("Error: Missing server's ip.\n");
		return 1;
	}
	
	int socket_connect, port_number, transfer;	
	struct sockaddr_in server;
	char buffer_send[MAX_BUFFER_LEN], buffer_recieve[MAX_BUFFER_LEN];
	
	//parameters config
	port_number = 3501;
	memset(buffer_send, 0, MAX_BUFFER_LEN);
	socket_connect = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_connect < 0){
		printf("Error: Could not open socket.\n");
		return 1;
	}
	
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port_number);

	if (inet_pton(AF_INET, argv[1], &server.sin_addr) < 0){
		printf("Error: Failed to convert ip.\n");
		return 1;
	}

	if (connect(socket_connect, (struct sockaddr *) &server, sizeof(server)) < 0){
		printf("Error: Cannot connect to server.\n");
		return 1;
	}
	while(strcmp(buffer_send, "exit\n")){
		
		printf("Enter message: ");
		memset(buffer_send, 0, MAX_BUFFER_LEN);
		fgets(buffer_send, MAX_BUFFER_LEN, stdin);
		
		transfer = write(socket_connect, buffer_send, strlen(buffer_send));
		if (transfer < 0){
			printf("Error: Cannot send message to server.\n");
			return 1;
		}
		
		memset(buffer_recieve, '0', MAX_BUFFER_LEN);
		transfer = read(socket_connect, buffer_recieve, MAX_BUFFER_LEN);
		if (transfer < 0){
			printf("Error: Cannot get reply from server.\n");
			return 1;
		}
		printf("Server: %s", buffer_recieve);
	}	
	close(socket_connect);
	return 0;
}
