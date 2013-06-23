#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>

struct sockaddr_in* convert_addr(char* buff);

int main(int argc, char *argv[]) {
	int tcpsock, conn;
	tcpsock = socket(AF_INET, SOCK_STREAM, 0);
	if(tcpsock < 0) {
		perror("Socket Error");
		exit(0);
	}
	
	char buff[1024], resp[1024]; //for packet data

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

	struct sockaddr_in* ipv4 = convert_addr(buff);
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(sock, (struct sockaddr *)&ipv4, sizeof(struct sockaddr));
	printf("ello\n");
	write(sock, (char*)buff, sizeof(buff));
	printf("written\n");
	read(sock, (char*)resp, sizeof(resp));
	printf("%s\n", resp);
	close(conn);
	close(tcpsock);
	return 0;
}


struct sockaddr_in* convert_addr(char* buff) {
	char test[8];
	char *host_name;
	char *host;
	strncpy(test, buff, 7);
	test[7] = '\0';
	if(!strcmp(test, "CONNECT")) {
		host_name = strtok(buff, " ");
		host_name = strtok(NULL, " ");
		char *host1 = strstr(host_name, ":");
		strncpy(host, host_name, (int)host1 - (int)host_name);
		printf("HHost:%s\n", host);
	} else {
		host_name = strtok(buff, "\n");
		printf("host%s\n", host_name);
		host_name = strtok(NULL, "\n");
		host = strstr(host_name, " ");
		(int)host++;
		printf("HHost:%s\n", host);
	}
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int status;

	status = getaddrinfo(host, NULL, &hints, &res);
	if(status!=0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(2);
	}
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
	char ipstr[128];
	inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
	printf("%s\n", ipstr);
	return (struct sockaddr_in*)res->ai_addr;
}
