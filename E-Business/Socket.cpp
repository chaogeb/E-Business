#include <winsock2.h>
#include <mswsock.h>
#include <iostream>
#include <string>
#include "FacadeController.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

using namespace std;

/*
客户端编程的步骤：
1：加载套接字库，创建套接字(WSAStartup()/socket())；
2：向服务器发出连接请求(connect())；
3：和服务器端进行通信(send()/recv())；
4：关闭套接字，关闭加载的套接字库(closesocket()/WSACleanup())。
*/
bool FacadeController::Client(const string cardNum, const string password, const double price)
{
	//client
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	bool auth = false;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		throw std::runtime_error("ERROR");
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		throw std::runtime_error("ERROR");
	}
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	char sendBuf[50];
	char recvBuf[50];

	unsigned long long value;
	value = price * 100;

	sprintf(sendBuf, "%d%2d%s%2d%s%llu", 0xff, cardNum.length(), cardNum.c_str(), password.length(), password.c_str(), value);
	send(sockClient, sendBuf, strlen(sendBuf) + 1, 0);
	recv(sockClient, recvBuf, 50, 0);
	if (atoi(recvBuf) == 0xac) {
		printf("银行验证通过\n");
		auth = true;
	}
	else if (atoi(recvBuf) == 0xfa) {
		printf("卡号不存在\n");
	}
	else if (atoi(recvBuf) == 0xfc) {
		printf("验证失败，请输入正确的卡号或密码\n");
	}
	else if (atoi(recvBuf) == 0xfd) {
		printf("余额不足\n");
	}
	else {
		printf("wrong format\n");
	}

	closesocket(sockClient);

	WSACleanup();
	return auth;
}