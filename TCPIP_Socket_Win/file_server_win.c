#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 30

void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA Wsadata;
	SOCKET hServSock, hClntSock;
	FILE* fp;
	char buf[BUF_SIZE];
	int readCnt;

	SOCKADDR_IN servAdr, clntAdr;
	int clntAdr_sz;

	if (argv != 2) {
		printf("Usage : %s <Port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &Wsadata) != 0)
		ErrorHandling("WSAStartup() Error!");

	fp = fopen("file_server_win.c", "rb");
	
	if ((hServSock = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) 
		ErrorHandling("bind() Error!");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() Error!");

	clntAdr_sz = sizeof(clntAdr);

	if ((hClntSock = accept(hServSock, (SOCKADDR*)&hClntSock, &clntAdr_sz)) == -1)
		ErrorHandling("accept() Error!");

	while (1) {
		readCnt = fread((void *)buf, 1, BUF_SIZE, fp);
		if (readCnt < BUF_SIZE)
		{
			send(hClntSock, (char *)&buf, readCnt, 0);
			break;
		}
		send(hClntSock, (char *)&buf, BUF_SIZE, 0);	
	}
	shutdown(hClntSock, SD_SEND);
	recv(hClntSock, (char*)buf, BUF_SIZE, 0);
	printf("Message from Client: %s \n", buf);
	fclose(fp);
	closesocket(hClntSock); closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
