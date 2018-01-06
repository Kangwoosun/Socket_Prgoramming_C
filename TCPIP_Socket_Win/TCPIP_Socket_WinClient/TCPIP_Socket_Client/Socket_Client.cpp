#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
	WSADATA wsaDATA;
	SOCKET connect_socket;
	SOCKADDR_IN connect_addr;

	if (argc != 3) {
		printf("usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaDATA) != 0) {
		ErrorHandling("WSAStartup() Error");
	}
		
	if ((connect_socket = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		ErrorHandling("socket() Error");
	}

	memset(&connect_addr, 0, sizeof(connect_addr));
	connect_addr.sin_family = AF_INET;
	connect_addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	connect_addr.sin_port = htons(atoi(argv[2]));

	if (connect(connect_socket, (SOCKADDR*)&connect_addr, sizeof(connect_addr))) {
		ErrorHandling("connect() Error");
	}

	send(connect_socket, "Hello World", 12, 0);
	puts("전송 성공, 프로그램 종료");

	closesocket(connect_socket);
	WSACleanup();
	return 0;

}