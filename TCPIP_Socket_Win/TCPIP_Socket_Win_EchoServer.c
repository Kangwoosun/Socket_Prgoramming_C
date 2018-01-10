#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA Wsadata;
	SOCKET hServSock, hClntSock;
	char message[BUF_SIZE];
	int strLen, i;

	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &Wsadata) != 0) {
		ErrorHandling("WSAStartup() Error!");
	}
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) {
		ErrorHandling("socket() Error!");
	}
	memset(&servAdr, 0, sizeof(servAdr));

	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) {
		ErrorHandling("bind() Error!");
	}
	if (listen(hServSock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() Error!");
	}

	clntAdrSize = sizeof(clntAdr);

	for (i = 0; i < 5; i++) {
		if ((hClntSock = accept(hServSock, (SOCKADDR*)&hClntSock, &clntAdrSize)) == -1)
			ErrorHandling("accept() Error!");
		else
			printf("Connected client %d \n", i + 1);

		while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0) 
			send(hClntSock, message, BUF_SIZE, 0);

			closesocket(hClntSock);
		
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char * message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
