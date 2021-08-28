#include "LocGenHeader.h"

char ip_server[MAX_BUFFER_LEN];
int running = 1, *room_checks, cur_room, data_no = 0;
pthread_t *thread_rooms;
pthread_mutex_t lock;

void *locGenControl(void *arg){
	char cmd_locGen[MAX_BUFFER_LEN];
	
	//Init config
	FILE *file_pointer;
	file_pointer = fopen("LocGenConfig.xml","w");
	if(file_pointer == NULL){
			printf("Error: Cannot open config.\n");
			return NULL;
	}
	fprintf(file_pointer,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	fprintf(file_pointer,"<!--Config of LocGen-->\n");
	fprintf(file_pointer,"<Data>\n");
	fclose(file_pointer);
	
	while(running){		
		fgets(cmd_locGen, MAX_BUFFER_LEN, stdin);
		for(int i = 0; cmd_locGen[i]; i++){
		  cmd_locGen[i] = tolower(cmd_locGen[i]);
		}

		if (strcmp(cmd_locGen, "shutdown\n") == 0)
			running = 0;	
	}
	//shutdown(socket_connect, SHUT_RD);
	//close(socket_connect);	
	for(int i = 0; i < MAX_THREADS; i++)
		room_checks[i] = 0;
	
	//write end of config
	file_pointer = fopen("LocGenConfig.xml","a");
	fprintf(file_pointer,"</Data>\n");
	fclose(file_pointer);	
	
	printf("LocGen control thread ended.\n");
}

void *sensorRoom(void *arg){
	int i = *(int *)arg, coord_x = 0, coord_y = 0, socket_connect_room, port_number, transfer;
	struct sockaddr_in server;
	char buffer_send[MAX_BUFFER_LEN], buffer_receive[MAX_BUFFER_LEN], coord_x_string[MAX_BUFFER_LEN], coord_y_string[MAX_BUFFER_LEN], sensor_number_string[MAX_BUFFER_LEN];
	
	snprintf(sensor_number_string, MAX_BUFFER_LEN, "%d", i);
	
	port_number = 3500;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port_number);
	if(inet_pton(AF_INET, ip_server, &server.sin_addr) < 0){
		printf("Error: Failed to convert ip.\n");
		return NULL;
	}
	
	printf("Sensor %d is on.\n", i);
	while(room_checks[i]){
		//coord_x = rand();
		//coord_y = rand();
		coord_x++;
		coord_y++;
		
		snprintf(coord_x_string, MAX_BUFFER_LEN, "%d", coord_x);
		snprintf(coord_y_string, MAX_BUFFER_LEN, "%d", coord_y);
		
		
		//Write XML config
		pthread_mutex_lock(&lock);
		FILE *file_pointer;
		file_pointer = fopen("LocGenConfig.xml","a");
		if(file_pointer == NULL){
			printf("Error: Cannot open config.\n");
			break;
		}
		fprintf(file_pointer, "  <Data_%d>\n", data_no);
		fprintf(file_pointer, "    <sensorNo> %d </sensorNo>\n", i);
		fprintf(file_pointer, "    <coordX> %d </coordX>\n", coord_x);
		fprintf(file_pointer, "    <coordY> %d </coordY>\n", coord_y);
		fprintf(file_pointer, "  </Data_%d>\n", data_no);
		fclose(file_pointer);
		data_no ++;
		pthread_mutex_unlock(&lock);
		
		//Send to server
		memset(buffer_send, 0, MAX_BUFFER_LEN);
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
		
		socket_connect_room =  socket(AF_INET, SOCK_STREAM, 0);
		if (socket_connect_room < 0){
			printf("Error: Could not open socket.\n");
			continue;
		}
		if (connect(socket_connect_room, (struct sockaddr *) &server, sizeof(server)) < 0){
			printf("Error: Sensor %d cannot connect to server.\n", i);
			continue;
		}
		
		//send name
		strcpy(buffer_send, sensor_number_string);
		transfer = write (socket_connect_room, buffer_send, strlen(buffer_send));
		if (transfer < 0){
			printf("Error: Cannot send name.\n");
			break;
		}
		transfer = read (socket_connect_room, buffer_receive, MAX_BUFFER_LEN);
		if (transfer < 0){
			printf("Error: Cannot read reply from server.\n");
			break;
		}
		memset(buffer_send, 0, MAX_BUFFER_LEN);
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
		
		//send x
		strcpy(buffer_send, coord_x_string);
		transfer = write (socket_connect_room, buffer_send, strlen(buffer_send));
		if (transfer < 0){
			printf("Error: Cannot send x.\n");
			break;
		}
		transfer = read (socket_connect_room, buffer_receive, MAX_BUFFER_LEN);
		if (transfer < 0){
			printf("Error: Cannot read reply from server.\n");
			break;
		}
		memset(buffer_send, 0, MAX_BUFFER_LEN);
		memset(buffer_receive, 0, MAX_BUFFER_LEN);
		
		//send y
		strcpy(buffer_send, coord_y_string);
		transfer = write (socket_connect_room, buffer_send, strlen(buffer_send));
		if (transfer < 0){
			printf("Error: Cannot send y.\n");
			break;
		}
		transfer = read (socket_connect_room, buffer_receive, MAX_BUFFER_LEN);
		if (transfer < 0){
			printf("Error: Cannot read reply from server.\n");
			break;
		}
		
		close(socket_connect_room);
		sleep(2);
	}
	
	printf("Sensor %d stopped.\n", i);
	
	//random data
	
}

void processCmd(int cmd_number){
	int flag; 
	switch (cmd_number){
		case 1:
			//find empty room;
			for(cur_room = 0; cur_room < MAX_THREADS; cur_room++)
				if (!room_checks[cur_room])
					break;
			room_checks[cur_room] = 1;
			flag = pthread_create(&thread_rooms[cur_room], NULL, sensorRoom, &cur_room);
			if (flag){
				printf("Error: Cannot create Sensor thread.\n");
				return;
			}
			break;
		case 2:
			for(cur_room =0; cur_room < MAX_THREADS; cur_room++)
				if (room_checks[cur_room])
					break;
			room_checks[cur_room] = 0;
			break;
		case 3:
			for(int i = 0; i < MAX_THREADS; i++)
				room_checks[i] = 0;
			break;
	}
}

void *getCmdFromVTS(void *arg){
	int port_number, transfer, cmd_number, socket_connect;
	struct sockaddr_in server;
	char buffer_send[MAX_BUFFER_LEN], buffer_recieve[MAX_BUFFER_LEN];
	port_number = 3502;
	
	socket_connect = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_connect < 0){
		printf("Error: Could not open sockets\n");
		return NULL;
	}
	
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port_number);
	
	if(inet_pton(AF_INET, ip_server, &server.sin_addr) < 0){
		printf("Error: Failed to convert ip.\n");
		return NULL;
	}
	if (connect(socket_connect, (struct sockaddr *) &server, sizeof(server)) < 0){
		printf("Error: Cannot connect to server.\n");
		return NULL;
	}
	
	while(running){	
		strcpy(buffer_send, "alive\n");
		transfer = write(socket_connect, buffer_send, MAX_BUFFER_LEN);
		if (transfer < 0){
			printf("Error: Cannot send alive signal to server.\n");
			break;
		}
		
		memset(buffer_recieve, 0, MAX_BUFFER_LEN);
		transfer = read(socket_connect, buffer_recieve, MAX_BUFFER_LEN);
		if (transfer < 0){
			printf("Error: Cannot read cmd from server.\n");
			break;
		}
		if (strcmp(buffer_recieve, "0")){
			printf("Execute cmd: %s\n", buffer_recieve);
			memset(buffer_send, 0, MAX_BUFFER_LEN);
			cmd_number = atoi(buffer_recieve);
		
		} else cmd_number = 0;
		
		//Processing
		if (cmd_number != 0)
			processCmd(cmd_number);
		
	}
	
	strcpy(buffer_send, "shutdown\n");
	transfer = write(socket_connect, buffer_send, MAX_BUFFER_LEN);
	if (transfer < 0)
		printf("Error: Cannot send shutdown signal to server.\n");
	
	close(socket_connect);	
	printf("Thread recieve from CLI ended.\n");
}

int main(int argc, char *argv[]){
	if (argc < 2){
		printf("Error: Missing server's ip.\n");
		return 1;
	}
	

	thread_rooms = (pthread_t *)malloc(MAX_THREADS * sizeof(pthread_t));
	room_checks = (int *)malloc(MAX_THREADS * sizeof(int));
	for(int i = 0; i < MAX_THREADS; i++)
		room_checks[i] = 0;
	
	strcpy(ip_server, argv[1]);
	pthread_t locGen_control, get_cmd_from_VTS;
	int flag;
	flag = pthread_create(&get_cmd_from_VTS, NULL, getCmdFromVTS, NULL);
	if (flag){
		printf("Error: Cannot create Get cmd from VTS thread.\n");
		return 1;
	}
	flag = pthread_create(&locGen_control, NULL, locGenControl, NULL);
	if (flag){
		printf("Error: Cannot create LocGen control thread.\n");
		return 1;
	}
	
	
	pthread_join(locGen_control, NULL);
	pthread_join(get_cmd_from_VTS, NULL);
	for (int i = 0; i < MAX_THREADS; i++)
		pthread_join(thread_rooms[i], NULL); 
	
	free(thread_rooms);
	free(room_checks);
	
	printf("LocGen is shutting down.\n");
	return 0;
}
