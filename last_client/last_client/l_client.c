#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(void)
{
	printf("클라이언트 파일 시작... \n");

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	//1. 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) printf("Failed WSAStartup() \n");


	//2. socket 생성
					//ipv4   //TCP 조합	   //TCP 조합
	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //family, type, protocol
	if (hSocket == INVALID_SOCKET) printf("Failed socket() \n");


	// 3. 서버 연결
	// 3-1. 연결할 서버 주소 셋팅

	//오류 방지를 위해 모두 0으로 셋팅
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET; //서버의 주소체계를 설정한다.
	//접속할 서버 주소를 넣을 구조체인, servAddr에 주소체계, ip 주소를 넣는다.
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
	//서버와 Personal Computer의 CPU는 달라서 사용하는 포트가 서로 다르다.
	//서버의 포트 규격에 맞추기 위해서 특수한 함수로 맞춰준다.
	servAddr.sin_port = htons(80);

	// 3-2. 서버에 연결 
	//블록킹 함수다.(동기 함수) 
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		printf("Failed connect() \n");


	// 4. 서버로 부터 통신 대기
	int recvSize;
	char recvData[255];
	//리시브를 받을려면 일단 소켓을 정해주어야 한다
	//그리고 버퍼와 버퍼의 크기까지다.
	//블록킹 함수다.(동기 함수)
	recvSize = recv(hSocket, recvData, sizeof(recvData), 0);
	if (recvSize == -1) printf("recv() Error \n");

	printf("recv %d message : %s \n", recvSize, recvData);

	// 5. 소켓 종료 -> 윈속 종료
	closesocket(hSocket);
	WSACleanup(); //윈속 종료

	system("pause");
	return 0;
}