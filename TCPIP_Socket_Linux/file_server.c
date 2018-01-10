#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void ErrorHandling(char* message);

int main(int argc, char * argv[]){
	int serv_sock, clnt_sock;
	FILE* fp;
	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_Adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	fp = fopen("file_server.c","rb");
	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);

	memset(&serv_Adr, 0, sizeof(serv_Adr));
	serv_Adr.sin_family = AF_INET;
	serv_Adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_Adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		ErrorHandling("bind() Error!");

	if(listen(serv_sock, 5) == -1)
		ErrorHandling("listen() Error");

	clnt_adr_sz = sizeof(clnt_adr);
	
	if((clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr,
		&clnt_adr_sz)) == -1)
		ErrorHandling("accept() Error");

	while(1){
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if(read_cnt < BUF_SIZE)
		{
			write(clnt_sock, buf, read_cnt);
			break;
		}
		write(clnt_sock, buf, BUF_SIZE);
	}
	shutdown(clnt_sock, SHUT_WR);
	read(clnt_sock, buf, BUF_SIZE);
	printf("Message from client: %s \n",buf);

	fclose(fp); close(clnt_sock); close(serv_sock);
	return 0;
}
void ErrorHandling(char* message){
	fputs(message,stderr);
	fputc('\n', stderr);
	exit(1);
}