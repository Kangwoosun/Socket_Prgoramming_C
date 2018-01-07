#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void ErrorHandling(char * message);
int calculate(int opnum, int opnds[], char op);
int main(int argc, char * argv[]){
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i, operand_count, recv_len, recv_cnt;
	int result;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM,0);
	if(serv_sock == -1)
		ErrorHandling("socket() Error!");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		ErrorHandling("bind() Error!");

	if(listen(serv_sock, 5) == -1)
		ErrorHandling("listen() Error");

	clnt_adr_sz = sizeof(clnt_adr);
	for (i=0; i<5; i++){
		if((clnt_sock = accept(serv_sock, (struct socketaddr*)&clnt_adr, &clnt_adr_sz)) == -1)
			ErrorHandling("accept() Error");
		else
			printf("Connected client %d \n", i+1);
		
		read(clnt_sock, &operand_count, 1);
		recv_len =0;
		while((operand_count*4+1)>recv_len){
			recv_cnt = read(clnt_sock, &message[recv_len],BUF_SIZE-1);
			recv_len += recv_cnt;
		}

		result = calculate(operand_count,(int *)message,message[recv_len-1])
		write(clnt_sock, (char *)&result, sizeof(result));
		close(clnt_sock);
	}


/*
	clnt_adr_sz = sizeof(clnt_adr);

	for(i =0; i<5 ; i++){
		clnt_sock = accept(serv_scok, (struct socketaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock == -1)
			ErrorHandling("accept() Error");
		else
			printf("Connected client %d \n", i+1);

		while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
			write(clnt_sck, message, str_len);

		close(clnt_sock);
	}
*/
	close(serv_sock);
	return 0;
}

void ErrorHandling(char* message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

int calculate(int opnum, int opnds[], char op){
	int result = opnds[0], i;
	switch(op){
		case '+':
			for(i =1; i<opnum; i++) result += opnds[i];
			break;
		case '-':
			for(i =1; i<opnum; i++) result -= opnds[i];
			break;
		case '*':
			for(i =1; i<opnum; i++) result *= opnds[i];
			break;
	}
	return result;
}