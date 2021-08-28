#include "VTSHeader.h"

int running = 1;
int socket_listen_user, socket_accept_user, socket_listen_locGen, socket_accept_locGen, cmd_number = 0, socket_listen_room, socket_accept_room, fd_pipe[2], data_no = 0;
char *cmd_list[COMMANDS_NUMBER] = {"add\n", "remove\n", "exit\n"};

void *serverControl(void *arg){
	//Write log
	openlog("VTSLog", 0, LOG_USER);
	syslog(LOG_NOTICE, "Server started by uid %d", getuid ());
	closelog();
	
	char cmd_server[MAX_BUFFER_LEN];
	while(running){		
		fgets(cmd_server, MAX_BUFFER_LEN, stdin);
		for(int i = 0; cmd_server[i]; i++){
		  cmd_server[i] = tolower(cmd_server[i]);
		}

		if (strcmp(cmd_server, "shutdown\n") == 0)
			running = 0;	
		if (strcmp(cmd_server, "viewlog\n") == 0)
			system("cat /var/log/syslog | grep \"VTSLog\" ");
	}
	
	shutdown(socket_listen_user, SHUT_RD);
	shutdown(socket_accept_user, SHUT_RD);
	shutdown(socket_listen_locGen, SHUT_RD);
	shutdown(socket_accept_locGen, SHUT_RD);
	shutdown(socket_listen_room, SHUT_RD);
	shutdown(socket_accept_room, SHUT_RD);
	
	close(socket_listen_user);
	close(socket_accept_user);
	close(socket_listen_locGen);
	close(socket_accept_locGen);	
	close(socket_listen_room);
	close(socket_accept_room);
	
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	
	printf("Server control thread ended.\n");
	
	//Write log
	openlog("VTSLog", 0, LOG_USER);
	syslog(LOG_NOTICE, "Server shuted down.");
	closelog();
}

void identifyCmd(char *cmd){
	if (strcmp(cmd, "exit\n") == 0)
		printf("Client exited.\n");
	
	for (int i = 0; i < COMMANDS_NUMBER; i++){
		if (strcmp(cmd, cmd_list[i]) == 0){
			cmd_number = i+1;
			printf("Command number %d\n", cmd_number);
			return;
		}
	}
	printf("Unknown command\n");
	cmd_number = 0;
}

void *getUserInput(void *arg){
	int port_number, transfer, len_address_client;
	char buffer_send[MAX_BUFFER_LEN], buffer_receive[MAX_BUFFER_LEN];
	
	struct sockaddr_in server, client;
	
	//Parameter config
	port_number = 3501;
	memset(&server, 0, sizeof(server));
	memset(&client, 0, sizeof(client));
	
	//Server config
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port_number);
	
	//Socket config
	socket_listen_user = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_listen_user < 0){
		printf("Error: Cannot open socket listen to user.\n");
		return NULL;
	}
	
	//bind socket
	if (bind(socket_listen_user, (struct sockaddr *) &server, sizeof(server)) < 0){
		printf("Error: Cannot bind socket listen to user.\n");
		return NULL;
	}
	
	//listen
	listen(socket_listen_user, 5);
	len_address_client = sizeof(client);
	
	//accept & communicate
	while(running){
		memset(buffer_send, 0, MAX_BUFFER_LEN);
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
		
		printf("Server is listening from client at port %d\n", port_number);
		socket_accept_user = accept(socket_listen_user, (struct sockaddr *) &client, &len_address_client);
		if(socket_accept_user < 0){
			if(running)
				printf("Error: Cannot accept connection from client.\n");
			break;
		}
		char ip_user[MAX_BUFFER_LEN];
		inet_ntop(client.sin_family, (struct sockaddr *) &client, ip_user, sizeof(ip_user));
		printf("Server got connection from client at ip: %s\n", ip_user);
		
		//Write log
		openlog("VTSLog", 0, LOG_USER);
		syslog(LOG_NOTICE, "Got connection from client at ip: %s", ip_user);
		closelog();
		
		while(strcmp(buffer_receive, "exit\n")){
			memset(buffer_send, 0, MAX_BUFFER_LEN);
			memset(buffer_receive, 0, MAX_BUFFER_LEN);
			
			//Get message from client
			transfer = read(socket_accept_user, buffer_receive, MAX_BUFFER_LEN);
			if (transfer < 0){
				printf("Error: Cannot read from client.\n");
				break;
			}
			printf("Client: %s", buffer_receive);
			
			//Normalize command
			for(int i = 0; buffer_receive[i]; i++){
			  buffer_receive[i] = tolower(buffer_receive[i]);
			}
			
			//Write log
			openlog("VTSLog", 0, LOG_USER);
			syslog(LOG_INFO, "client: %s", buffer_receive);
			closelog();
			
			//Reply to client
			if (strcmp(buffer_receive, "exit\n"))
				strcpy(buffer_send, "Got the message.\n");
			else strcpy(buffer_send, "Goodbye.\n");
			
			transfer = write(socket_accept_user, buffer_send, sizeof(buffer_send));
			if (transfer < 0){
				printf("Error: Cannot reply to client.\n");
				break;
			}
			
			identifyCmd(buffer_receive);
		}
		close(socket_accept_user);
		sleep(1);
	}
	printf("Receive CLI thread ended.\n");
}

void *sendCmdToLocGen(void *arg){
	int port_number, transfer, len_address_locGen;
	char buffer_send[MAX_BUFFER_LEN], buffer_receive[MAX_BUFFER_LEN];
	
	struct sockaddr_in server, locGen;
	
	//Parameter config
	port_number = 3502;
	memset(&server, 0, sizeof(server));
	memset(&locGen, 0, sizeof(locGen));
	
	//Server config
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port_number);
	
	//Socket config
	socket_listen_locGen = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_listen_locGen < 0){
		printf("Error: Cannot open socket listen to LocGen.\n");
		return NULL;
	}
	
	//bind socket
	if (bind(socket_listen_locGen, (struct sockaddr *) &server, sizeof(server)) < 0){
		printf("Error: Cannot bind socket listen to LocGen.\n");
		return NULL;
	}
	
	//listen
	listen(socket_listen_locGen, 5);
	len_address_locGen = sizeof(locGen);
	
	//accept & communiacte
	while(running){
		printf("Server is listening from locGen at port %d\n", port_number);
		socket_accept_locGen = accept(socket_listen_locGen, (struct sockaddr *) &locGen, &len_address_locGen);
		if (socket_accept_locGen < 0){
			if (running)
				printf("Error: Cannot accept connection from LocGen.\n");
			continue;
		}
		char ip_locGen[MAX_BUFFER_LEN];
		inet_ntop(locGen.sin_family, (struct sockaddr *) &locGen, ip_locGen, sizeof(ip_locGen));
		printf("Server got connection from LocGen at ip: %s\n", ip_locGen);
		
		//Write log
		openlog("VTSLog", 0, LOG_USER);
		syslog(LOG_NOTICE, "Got connection from LocGen at ip: %s", ip_locGen);
		closelog();
		
		memset(buffer_send, 0, MAX_BUFFER_LEN);
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
		
		while(1){
			//receive from LocGen
			memset(buffer_receive, 0, MAX_BUFFER_LEN);
			transfer = read(socket_accept_locGen, buffer_receive, MAX_BUFFER_LEN);
			if (transfer < 0){
				printf("Error: No signal from LocGen.\n");
				break;
			}
			if (strcmp(buffer_receive, "shutdown\n") == 0){
				printf("LocGen: %s", buffer_receive);
				cmd_number = 0;
				break;
			} else {
				//send cmd to LocGen
				char cmd_string[MAX_BUFFER_LEN];
				snprintf(cmd_string, MAX_BUFFER_LEN, "%d", cmd_number);
				strcpy(buffer_send, cmd_string);
				transfer = write(socket_accept_locGen, buffer_send, sizeof(buffer_send));
				if (transfer < 0){
					printf("Error: Cannot send cmd to LocGen.\n");
					break;
				}
				cmd_number = 0;
			}
			//printf("cmd after converted: %s\n", buffer_send);
			sleep(1);
		}
		close(socket_accept_locGen);
		//sleep(1);
		//Write log
		openlog("VTSLog", 0, LOG_USER);
		syslog(LOG_NOTICE, "LocGen shutted down.");
		closelog();
	}
	printf("Send CLI to LocGen thread ended.\n");
}

void *receiveDataFromRoom (void *arg){
	int port_number, transfer, len_address_room;
	char buffer_send[MAX_BUFFER_LEN], buffer_receive[MAX_BUFFER_LEN];
	struct sockaddr_in server, room;
	
	//Parameter config
	port_number = 3500;
	memset(&server, 0, sizeof(server));
	memset(&room, 0, sizeof(room));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port_number);
	
	//socket listen and bind
	socket_listen_room = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_listen_room < 0){
		printf("Error: Cannot open socket listen to thread room.\n");
		return NULL;
	}
	
	if (bind(socket_listen_room, (struct sockaddr *) &server, sizeof(server)) <0 ){
		printf("Error: Cannot bind socket listen to thread room.\n");
		return NULL;
	}
	listen(socket_listen_room, 5);
	len_address_room = sizeof(room);
	
	while (running){
		printf("Server is listening from sensors at port %d\n", port_number);
		socket_accept_room = accept(socket_listen_room, (struct sockaddr *)&room, &len_address_room);
		if (socket_accept_room < 0){
			if(running)
				printf("Error: Cannot accept connection from sensor.\n");
				
			continue;
		}
		char ip_room[MAX_BUFFER_LEN];
		inet_ntop(room.sin_family, (struct sockaddr *) &room, ip_room, sizeof(ip_room));
		printf("Server got connection from room at ip: %s\n", ip_room);
		
		memset(buffer_send, 0, MAX_BUFFER_LEN);
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
		
		//Read name
		transfer = read(socket_accept_room, buffer_receive, MAX_BUFFER_LEN);
		printf("[Sensor: %s]\n", buffer_receive);
		if (transfer < 0){
			printf("Error: Cannot read sensor name.\n");
			break;
		}
		strcpy(buffer_send, "OK");
		transfer = write(socket_accept_room, buffer_send, sizeof(buffer_send));
		if (transfer < 0){
			printf("Error: Cannot reply to sensor.\n");
			break;
		}
			//Send name through pipe
		transfer = write(fd_pipe[1], buffer_receive, sizeof(buffer_receive));
		if (transfer < 0){
			printf("Error: Cannot send name through pipe.\n");
			break;
		}
		
		memset(buffer_send, 0, MAX_BUFFER_LEN);
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
			
		
		
		//Read x
		transfer = read(socket_accept_room, buffer_receive, MAX_BUFFER_LEN);
		printf("[Coordinate x: %s]\n", buffer_receive);
		if (transfer < 0){
			printf("Error: Cannot read x.\n");
			break;
		}
		strcpy(buffer_send, "OK");
		transfer = write(socket_accept_room, buffer_send, sizeof(buffer_send));
		if (transfer < 0){
			printf("Error: Cannot reply to sensor.\n");
			break;
		}
			//Send x through pipe
		transfer = write(fd_pipe[1], buffer_receive, sizeof(buffer_receive));
		if (transfer < 0){
			printf("Error: Cannot send x through pipe.\n");
			break;
		}
		memset(buffer_send, 0, MAX_BUFFER_LEN);
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
			
		//Read y
		transfer = read(socket_accept_room, buffer_receive, MAX_BUFFER_LEN);
		printf("[Coordinate y: %s]\n", buffer_receive);
		if (transfer < 0){
			printf("Error: Cannot read y.\n");
			break;
		}
		strcpy(buffer_send, "OK");
		transfer = write(socket_accept_room, buffer_send, sizeof(buffer_send));
		if (transfer < 0){
			printf("Error: Cannot reply to sensor.\n");
			break;
		}
			//Send y through pipe
		transfer = write(fd_pipe[1], buffer_receive, sizeof(buffer_receive));
		if (transfer < 0){
			printf("Error: Cannot send y through pipe.\n");
			break;
		}
		memset(buffer_send, 0, MAX_BUFFER_LEN);
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
		
		close(socket_accept_room);
		//sleep(1);
	}
	printf("Receive data from sensors thread ended.\n");
}

void *processAndStoreData(void *arg){
	char buffer_receive[MAX_BUFFER_LEN];
	int transfer;
	FILE *file_pointer;
	file_pointer = fopen("ServerConfig.xml", "w");
	if (file_pointer == NULL){
		printf("Error: Cannot open config.\n");
		return NULL;
	}
	fprintf(file_pointer,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	fprintf(file_pointer,"<!--Config of Server-->\n");
	fprintf(file_pointer,"<Data>\n");
	fclose(file_pointer);
	while(running){
		file_pointer = fopen("ServerConfig.xml", "a");
		if (file_pointer == NULL){
			printf("Error: Cannot open config.\n");
			break;
		}
		//Name
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
		transfer = read(fd_pipe[0], buffer_receive, MAX_BUFFER_LEN);
		if (transfer < 0){
			printf("Error: Cannot receive name from Get data thread.\n");
			break;
		}
		if(strcmp(buffer_receive, "\0") == 0)
			continue;
		fprintf(file_pointer, "  <Data_%d>\n", data_no);
		fprintf(file_pointer, "    <sensorNo> %s </sensorNo>\n", buffer_receive);
		//x
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
		transfer = read(fd_pipe[0], buffer_receive, MAX_BUFFER_LEN);
		if (transfer < 0){
			printf("Error: Cannot receive x from Get data thread.\n");
			break;
		}
		fprintf(file_pointer, "    <coordX> %s </coordX>\n", buffer_receive);
		//y
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
		transfer = read(fd_pipe[0], buffer_receive, MAX_BUFFER_LEN);
		if (transfer < 0){
			printf("Error: Cannot receive y from Get data thread.\n");
			break;
		}
		fprintf(file_pointer, "    <coordY> %s </coordY>\n", buffer_receive);
		fprintf(file_pointer, "  </Data_%d>\n", data_no);
		fclose(file_pointer);
		data_no++;
		//printf("<Receive: %s>\n", buffer_receive); 
	}
	fprintf(file_pointer,"</Data>\n");
	printf("Process and store data thread ended.\n");
}

int main(){
	pthread_t receive_CLI, server_control, send_cmd_to_locGen, receive_data_from_room, process_and_store_data;
	int flag;
	flag = pipe(fd_pipe);
	if (flag < 0){
		printf("Error: Cannot create pipe.\n");
		return 1;
	}
	flag = pthread_create(&receive_CLI, NULL, getUserInput, NULL);
	if (flag){
		printf("Error: Cannot create receive from CLI thread.\n");
		return 1;
	}
	flag = pthread_create(&server_control, NULL, serverControl, NULL);
	if (flag){
		printf("Error: Unable to control server.\n");
		return 1;
	}
	flag = pthread_create(&send_cmd_to_locGen, NULL, sendCmdToLocGen, NULL);
	if (flag){
		printf("Error: Cannot create receive from CLI to LocGen thread.\n");
		return 1;
	}
	flag = pthread_create(&receive_data_from_room, NULL, receiveDataFromRoom, NULL); 
	if (flag){
		printf("Error: Cannot create receive data from sensor thread.\n");
		return 1;
	}	
	flag = pthread_create(&process_and_store_data, NULL, processAndStoreData, NULL);
	if (flag){
		printf("Error: Cannot create process and store data thread.\n");
		return 1;
	}
	
	pthread_join(server_control, NULL);
	pthread_join(receive_CLI, NULL);
	pthread_join(send_cmd_to_locGen, NULL);
	pthread_join(receive_data_from_room, NULL);
	pthread_join(process_and_store_data, NULL);
	printf("Server is shutting down.\n");
	return 0;
}
