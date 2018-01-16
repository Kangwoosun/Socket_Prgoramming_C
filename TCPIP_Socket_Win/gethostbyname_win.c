#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA Wsadata;
	int i;
	HOSTENT* host;
	
	if (argc != 2) {
		printf("Usage : %s <addr>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &Wsadata) != 0) {
		ErrorHandling("WSAStartup() Error!");
	}
	if (!(host = gethostbyname(argv[1])))
		ErrorHandling("gethostbyname() Error!");
	
	printf("Official name : %s \n", host->h_name);
	
	for (int i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s \n", i + 1, host->h_aliases);
	
	printf("Address type: %s \n",(host->h_addrtype==AF_INET ? "AF_INET"
		: "AF_INET6"));

	for (int i = 0; host->h_addr_list[i]; i++)
		printf("IP addr %d : %s \n", i + 1, 
			inet_ntoa(*(IN_ADDR*)host->h_addr_list[i]));

	WSACleanup();
	return 0;
}

void ErrorHandling(char * message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
