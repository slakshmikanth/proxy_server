#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include "http_parser.h"

in_addr_t convert_addr(char* buff);

int main(int argc, char *argv[]) {
	int tcpsock, conn;
	tcpsock = socket(AF_INET, SOCK_STREAM, 0);
	if(tcpsock < 0) {
		perror("Socket Error");
		exit(0);
	}
	

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
	while(1)
	{
		int len = sizeof(cli);
		char buff[100000], resp[100000];
		char buffCopy[100000];
		conn = accept(tcpsock, (struct sockaddr *)&cli, &len);
	
		//need to make this concurrent using fork() or pthread_create()
	
		read(conn, (char *)buff, 100000);
		printf("%s\n", buff);
		strcpy(buffCopy, buff);
		/* Unsure about convert_addr() function signature and return type */
	
		struct sockaddr_in ipv4;
		ipv4.sin_family = AF_INET;
		ipv4.sin_port = htons(80);
		ipv4.sin_addr.s_addr = convert_addr(buffCopy);
		int sock = socket(AF_INET, SOCK_STREAM, 0);
		connect(sock, (struct sockaddr *)&ipv4, sizeof(struct sockaddr));
		write(sock, (char*)buff, strlen(buff));
		read(sock, (char*)resp, 100000);
		printf("%s\n", resp);
		write(conn, resp, strlen(resp));
		memset(buff, 0, sizeof(resp));
		memset(buffCopy, 0, sizeof(resp));
		memset(resp, 0, sizeof(resp));
		close(conn);
		close(sock);
	}
	close(tcpsock);
	return 0;
}


in_addr_t convert_addr(char* buff) {
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
	int i;
	for(i=0;host[i]!='\0';i++) {
		if(host[i] == 13) {
			printf("thaaaa%caaaaa\n", host[i]);
			printf("%d\n", (int)host[i]);
			host[i] = '\0';
		}


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
	return ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
}
