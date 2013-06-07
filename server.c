#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>

void convert_addr(char* buff);

int main(int argc, char *argv[]) {
	int tcpsock = socket(AF_INET, SOCK_STREAM, 0);
	int udpsock = socket(AF_INET, SOCK_DGRAM, 0);
	if(tcpsock < 0) {
		perror("Socket Error");
		exit(0);
	}
	//create udp socket also to multiplex
	
	char buff[1024]; //for packet data

	//usual server bindings
	struct sockaddr_in prox_serv, cli, serv;
	prox_serv.sin_family = AF_INET;
	prox_serv.sin_addr.s_addr = INADDR_ANY;
	prox_serv.sin_port = htons(8888);
	
	serv.sin_family = AF_INET;
	serv.sin_port = htons(80);
	
	int test = bind(tcpsock, (struct sockaddr *)&prox_serv, sizeof(serv));
	if(test < -1) {
		perror("Bind Error");
		exit(0);
	}
	listen(tcpsock, 5);
	int len = sizeof(cli);
	int conn = accept(tcpsock, (struct sockaddr *)&cli, &len);

	//need to make this concurrent using fork() or pthread_create()

	read(conn, (char *)buff, sizeof(buff));
	//char* host_name;
	//char* index = strstr(buff, "Host");
	//char* str = strtok_r(index, "\n", &host_name);
	//str = strtok_r(str, " ", &host_name);

	/* Unsure about convert_addr() function signature and return type */
	convert_addr(buff);
	//printf("%s\n", str);
	//printf("%s\n", host_name);
	close(conn);

	close(tcpsock);
	return 0;
}


void convert_addr(char* buff) {
	char* host_name;
	char* request_str = strstr(buff, "Host");
	char* tok_str = strtok_r(request_str, "\n", &host_name);
	tok_str = strtok_r(tok_str, " ", &host_name);
	printf("%s\n", host_name);
	struct hostent* h;
	h = gethostbyname(host_name);
	if(h!=NULL) {
		//make connection to the requested host using this address
		
	} else {
		//display corresponding error message
	}

}
