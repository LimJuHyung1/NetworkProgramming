#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Ws2tcpip.h>


#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define BUFSIZE 1024

void ErrorHandling(char* message);

int main(int argc, char** argv) {

	WSADATA wsaData;
	SOCKET servSock, clntSock;	//SOCKET은 사실 UINT_PTR 형이다
	SOCKADDR_IN servAddr, clntAddr;

	char sAddr[15];
	int sPort;
	int nRcv;
	unsigned int Addr;
	char message[BUFSIZE];

	struct hostent* host;

	printf("Server Address : ");
	gets(sAddr);
	printf("Port Number : ");
	gets(message);
	sPort = atoi(message);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("Load WinSock 2.2 DLL Error");

	clntSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clntSock == INVALID_SOCKET)
		ErrorHandling("Socket Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.s_addr = inet_addr(sAddr);
	servAddr.sin_addr.s_addr = inet_pton(AF_INET, sAddr, &Addr);
	servAddr.sin_port = htons(sPort);

	if (connect(clntSock, (void*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("Connection Error");
	}
	else {
		printf("Connect OK!\nStart...\n");
	}

	while (1) {
		printf("\nSend Message : ");
		gets(message);

		if (strcmp(message, "exit") == 0) {
			send(clntSock, message, (int)strlen(message), 0);
			break;
		}
		send(clntSock, message, (int)strlen(message), 0);
		printf("Message Receives ...\n");

		nRcv = recv(clntSock, message, sizeof(message) - 1, 0);
		if (nRcv == SOCKET_ERROR) {
			printf("Receive Error...\n");
			break;
		}
		message[nRcv] = '\0';

		if (strcmp(message, "exit") == 0) {
			printf("Close Server Connection...\n");
			break;
		}

		printf("Receive Message : %s", message);
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