#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Ws2tcpip.h>



#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define BUFSIZE 1024

void ErrorHandling(char* message);

int main(int argc, char** argv) {

	WSADATA wsaData;
	SOCKET servSock, clntSock;	//SOCKET은 사실 UINT_PTR 형이다
	SOCKADDR_IN servAddr, clntAddr;

	char message[BUFSIZE];		//Message Buffer
	int strLen;
	int fromLen, nRcv;

	if (argc != 2) {
		printf("Please, Insert Port Number\n");
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("Load WinSock 2.2 DLL Error");

	servSock = socket(PF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET)
		ErrorHandling("Socket Error");

	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1]));
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(servSock, (void*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("Bind Error");

	if (listen(servSock, 2) == SOCKET_ERROR)
		ErrorHandling("Listen Error");

	fromLen = sizeof(clntAddr);

	clntSock = accept(servSock, (void*)&clntAddr, &fromLen);
	if (clntSock == INVALID_SOCKET) {
		ErrorHandling("Accept Error");
	}
	else {
		char addrStr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(clntAddr.sin_addr), addrStr, sizeof(addrStr));
		printf("%s Connection Complete!\n", addrStr);
		printf("Start ...\n");
	}

	/*else {
		char addrStr[sizeof(clntAddr.sin_addr)];
		inet_ntop(AF_INET, &(clntAddr.sin_addr), addrStr, sizeof(clntAddr.sin_addr));
		printf("%s Connection Complete!\n", addrStr);
		//printf("%s Connection Complete!\n", inet_ntop(clntAddr.sin_addr));
		printf("Start ...\n");
	}*/

	closesocket(servSock);

	while (1) {
		printf("Message Receives ...\n");
		nRcv = recv(clntSock, message, sizeof(message) - 1, 0);

		if (nRcv == SOCKET_ERROR) {
			printf("Receive Error...\n");
			break;
		}
		message[nRcv] = '\0';

		if (strcmp(message, "exit") == 0) {
			printf("Close Client Connection...\n");
			break;
		}

		printf("Receive Message : %s", message);
		printf("\nSend Message : ");

		if (fgets(message, sizeof(message), stdin) == NULL) {
			printf("Input Error...\n");
			break;
		}

		message[strcspn(message, "\n")] = '\0';

		if (strcmp(message, "exit") == 0) {
			send(clntSock, message, (int)strlen(message), 0);
			break;
		}

		send(clntSock, message, (int)strlen(message), 0);
	}

	closesocket(clntSock);
	WSACleanup();

	printf("Close Connection...\n");
	getchar();

	return 0;
}

void ErrorHandling(char* message) {
	WSACleanup();
	fputs(message, stderr);
	fputc('\n', stderr);
	getchar();
	exit(1);
}