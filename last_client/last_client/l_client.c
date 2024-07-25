#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(void)
{
	printf("Ŭ���̾�Ʈ ���� ����... \n");

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	//1. ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) printf("Failed WSAStartup() \n");


	//2. socket ����
					//ipv4   //TCP ����	   //TCP ����
	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //family, type, protocol
	if (hSocket == INVALID_SOCKET) printf("Failed socket() \n");


	// 3. ���� ����
	// 3-1. ������ ���� �ּ� ����

	//���� ������ ���� ��� 0���� ����
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET; //������ �ּ�ü�踦 �����Ѵ�.
	//������ ���� �ּҸ� ���� ����ü��, servAddr�� �ּ�ü��, ip �ּҸ� �ִ´�.
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
	//������ Personal Computer�� CPU�� �޶� ����ϴ� ��Ʈ�� ���� �ٸ���.
	//������ ��Ʈ �԰ݿ� ���߱� ���ؼ� Ư���� �Լ��� �����ش�.
	servAddr.sin_port = htons(80);

	// 3-2. ������ ���� 
	//���ŷ �Լ���.(���� �Լ�) 
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		printf("Failed connect() \n");


	// 4. ������ ���� ��� ���
	int recvSize;
	char recvData[255];
	//���ú긦 �������� �ϴ� ������ �����־�� �Ѵ�
	//�׸��� ���ۿ� ������ ũ�������.
	//���ŷ �Լ���.(���� �Լ�)
	recvSize = recv(hSocket, recvData, sizeof(recvData), 0);
	if (recvSize == -1) printf("recv() Error \n");

	printf("recv %d message : %s \n", recvSize, recvData);

	// 5. ���� ���� -> ���� ����
	closesocket(hSocket);
	WSACleanup(); //���� ����

	system("pause");
	return 0;
}