#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>

void convert_addr(char* buff);
int tcpsock;
int conn;
int main(int argc, char *argv[]) {
	tcpsock = socket(AF_INET, SOCK_STREAM, 0);
	if(tcpsock < 0) {
		perror("Socket Error");
		exit(0);
	}
	
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
	conn = accept(tcpsock, (struct sockaddr *)&cli, &len);

	//need to make this concurrent using fork() or pthread_create()

	read(conn, (char *)buff, sizeof(buff));
	printf("%s\n", buff);
	/* Unsure about convert_addr() function signature and return type */
	convert_addr(buff);

	close(conn);
	close(tcpsock);
	return 0;
}


void convert_addr(char* buff) {
	char *host_name;
	char *host;
	host_name = strtok(buff, " ");
	host_name = strtok(NULL, " ");
	host = strtok(host_name, "/");
	host = strtok(NULL, "/");
	
	printf("host:%s\n", host);
	printf("%d\n", sizeof((char*)&host_name));
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int status;

	status = getaddrinfo(host, "80", &hints, &res);
	if(status!=0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(2);
	}
	//connect using res.ai_addr
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
	
	char ipstr[128];

	inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
	printf("%s\n", ipstr);
}
