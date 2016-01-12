// tcpclient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#include <iostream>
#include "Execute.h"
using namespace std;
#pragma comment (lib, "ws2_32.lib")
#include <windows.h>
#include <atlstr.h>

SOCKET client_sock;
Execute exe;
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
	server_sock.sin_addr.s_addr = inet_addr("172.18.117.13");
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
	if (memcmp("kill", cmd, 4) == 0)
	{
		exe.kill_process(_T("Guardian.exe"));
		exe.kill_process(_T("TVM.exe"));
		return "ACK";
	}

	if (memcmp("go", cmd,2) == 0)
	{
		//system("cd C:\TVM");
		//system("tvm.exe");
		WinExec("C://TVM//TVM.exe", SW_SHOWMAXIMIZED);

		//STARTUPINFO StartInfo;
		//PROCESS_INFORMATION ProcInfo;
		//GetStartupInfo(&StartInfo);

		//StartInfo.dwFlags = STARTF_USESHOWWINDOW;    // 隐藏运行
		//StartInfo.wShowWindow = SW_HIDE;

		//// 程序名
		//CString exeFile = _T("C:\\TVM\\TVM.exe");

		//// 运行程序
		//BOOL ret_code = CreateProcess(exeFile,
		//	NULL,
		//	NULL,
		//	NULL,
		//	FALSE,
		//	NORMAL_PRIORITY_CLASS,
		//	NULL,
		//	NULL,
		//	&StartInfo,
		//	&ProcInfo);
		return "ACK";
	}
	return "NAK";
}
int _tmain(int argc, _TCHAR* argv[])
{
	int nLen = 0;
	char recv_buf[MAX_PATH] = {0};
	if (connect())
	{
		while(true)
		{
			memset(recv_buf,0,MAX_PATH);
			nLen = 0;
			nLen = recv(client_sock, recv_buf, MAX_PATH, 0);
			if(nLen > 0 && client_sock != INVALID_SOCKET)
			{
				cout << "Received message from server: " << recv_buf << ".Length : " << nLen<< endl;
				ParseCommand(recv_buf);
				//send(client_sock, ParseCommand(recv_buf).data(), ParseCommand(recv_buf).size(), 0);
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

