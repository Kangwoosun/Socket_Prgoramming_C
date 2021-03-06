#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(char* Message) {
	fputs(Message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[])
{
	WSADATA wsaDATA;
	SOCKET listen_socket, connect_socket;
	SOCKADDR_IN listen_addr, connect_addr;
	int sockaddr_in_size;
	char message[100];

	if (argc != 2) {
		printf("usage : %s <PORT>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaDATA) != 0) {
		ErrorHandling("WSAStartup() Error");
	}

	if ((listen_socket = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		ErrorHandling("socket() Error");
	}

	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	listen_addr.sin_port = htons(atoi(argv[1]));

	if (bind(listen_socket, (SOCKADDR*)&listen_addr, sizeof(listen_addr)) == SOCKET_ERROR) {
		ErrorHandling("bind() Error");
	}

	if (listen(listen_socket, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() Error");
	}
	sockaddr_in_size = sizeof(connect_addr);
	
	if ((connect_socket = accept(listen_socket, (SOCKADDR*)&connect_addr, &sockaddr_in_size)) == INVALID_SOCKET) {
		ErrorHandling("accept() Error");
	}
	recv(connect_socket, message, 12, 0);
	printf("클라이언트로부터 메세지 : %s\n", message);

	closesocket(connect_socket);
	closesocket(listen_socket);
	WSACleanup();
	return 0;

	WSACleanup();
    return 0;


}

