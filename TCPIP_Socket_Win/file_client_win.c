#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA Wsadata;
	SOCKET hSocket;
	int readcnt;
	FILE* fp;
	char buf[BUF_SIZE];
	SOCKADDR_IN Serv_adr;

	if (argc != 3) {
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &Wsadata) != 0)
		ErrorHandling("WSAStartup() Error");

	fp = fopen("receive.dat", "wb");
	
	if ((hSocket = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		ErrorHandling("socket() Error");

	memset(&Serv_adr, 0, sizeof(Serv_adr));
	Serv_adr.sin_family = AF_INET;
	Serv_adr.sin_addr.S_un.S_addr = htonl(atoi(argv[1]));
	Serv_adr.sin_port = htons(atoi(argv[2]));

	connect(hSocket, (SOCKADDR *)&Serv_adr, sizeof(Serv_adr));
	while ((readcnt = recv(hSocket, buf, BUF_SIZE, 0)) != 0)
		fwrite((void*)buf, 1, readcnt, fp);
	puts("Received file data");
	send(hSocket, "Thank you", 10, 0);
	fclose(fp); closesocket(hSocket); WSACleanup();
	return 0;
}
void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}