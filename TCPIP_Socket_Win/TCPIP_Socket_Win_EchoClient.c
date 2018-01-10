#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char * message);

int main(int argc, char* argv[]) {
	WSADATA Wsadata;
	SOCKET hSock;
	int str_Len, recv_len, recv_cnt;
	SOCKADDR_IN servAdr;
	char message[BUF_SIZE];

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &Wsadata) != 0)
		ErrorHandling("WSAStartup() Error!");
	
	if ((hSock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		ErrorHandling("socket() Error!");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.S_un.S_addr = htonl(atoi(argv[1]));
	servAdr.sin_port = htons(atoi(argv[2]));

	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() Error!");
	else
		puts("Connected............");

	while (1) {
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		
		send(hSock, message, strlen(message), 0);
		str_Len = recv(hSock, message, BUF_SIZE-1, 0);
		message[str_Len] = 0;
		printf("Message from server: %s", message);
	}
	closesocket(hSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}