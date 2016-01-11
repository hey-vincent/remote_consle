// tcpclient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#include <iostream>
using namespace std;
#pragma comment (lib, "ws2_32.lib")

SOCKET client_sock;

int connect()
{
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2,2), &ws) != 0)
	{
		cout << "Failed to WSAstartup with error " << GetLastError() << endl;
		return 1;
	}

	// create server socket instance
	client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (client_sock == INVALID_SOCKET)
	{
		cout << "Failed to create server socket with error " << GetLastError() << endl;
		return 2;
	}

	SOCKADDR_IN server_sock;
	server_sock.sin_family = AF_INET;
	server_sock.sin_port = 5000;
	server_sock.sin_addr.s_addr = inet_addr("192.168.0.111");
	memset(server_sock.sin_zero,0,sizeof(server_sock.sin_zero));

	if( connect(client_sock,(sockaddr*)&server_sock,sizeof(SOCKADDR_IN)) == INVALID_SOCKET)
	{
		cout << "Failed to connect server with error " << GetLastError() << endl;
		return false;
	}
	cout << "Succeed to connect to server." << endl;
	return true;
}

typedef struct _cmd
{
	string stType;
	string stContent;

	_cmd()
	{
		stType = "";
		stContent = "";
	}
}Command;

string ParseCommand(char* cmd)
{
	string stCmd = cmd;
	if (cmd == "KILL" || cmd == "kill")
	{

	}

	if (cmd == "GO" || cmd == "go")
	{

	}
	return "ACK";
}
int _tmain(int argc, _TCHAR* argv[])
{
	char recv_buf[MAX_PATH] = {0};
	if (connect())
	{
		while(true)
		{
			if( recv(client_sock,recv_buf,MAX_PATH,0) > 0 && client_sock != INVALID_SOCKET)
			{
				cout << "Received message from server: " << recv_buf << endl;
				send(client_sock, ParseCommand(recv_buf).data(), ParseCommand(recv_buf).size(), 0);
			}
			else
			{
				connect();
			}
		}
		
	}
	system("pause");
	return 0;
}

