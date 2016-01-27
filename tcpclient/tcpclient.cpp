// tcpclient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>

#include "Execute.h"

#include <WinSock2.h>
#include <atlstr.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;
SOCKET client_sock;
Execute exe;
map<string, string> map_settings;


int load_setting();
void parse_setting(string setting);
void show_settins();


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
	server_sock.sin_port = atoi(map_settings["port"].data());
	server_sock.sin_addr.s_addr = inet_addr(map_settings["ip"].data());
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
	if (memcmp("clear", cmd, 5) == 0 || memcmp("cls", cmd, 5) == 0)
	{
		system("cls");
	}

	if (memcmp("kill", cmd, 5) == 0)
	{
		exe.kill_process(_T("Guardian.exe"));
		Sleep(100);
		exe.kill_process(_T("TVM.exe"));
		return "ACK";
	}

	if (memcmp("go", cmd,3) == 0)
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
	load_setting();

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

int load_setting()
{
	char buf[1024];                //临时保存读取出来的文件内容
	string message;
	ifstream infile;
	infile.open(".//trmlt.txt");
	if (infile.is_open())          //文件打开成功,说明曾经写入过东西
	{
		while (infile.good() && !infile.eof())
		{
			memset(buf, 0, 1024);
			infile.getline(buf, 1204);
			message = buf;
			parse_setting(message);
		}
		infile.close();
	}
	show_settins();
	return 0;
}

void parse_setting(string setting)
{
	string::iterator iter;
	string key = "";
	string val = "";
	if ((iter = find(setting.begin(), setting.end(), '=')) != setting.end())
	{
		key.assign(setting.begin(), iter);
		val.assign(iter + 1, setting.end());
		map_settings[key] = val;
	}
}

void show_settins()
{
	if (map_settings.empty())
	{
		cout << "settings empty" << endl;
		return;
	}
	map<string, string>::iterator iter = map_settings.begin();
	while (iter != map_settings.end())
	{
		cout << iter->first << "\t" << iter->second << endl;
		iter++;
	}
}