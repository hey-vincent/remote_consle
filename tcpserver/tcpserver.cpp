// tcpserver.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Server.h"
//#include <windows.h>
//#include <afx.h>

//CString strIP = _T("127.0.0.1");
//CString strPort = _T("5000");
#include <fstream>
using namespace std;

char ip[20] = "127.0.0.1";
int port = 6000;

void load_setting()
{
	ifstream file;
	file.open(".//server.txt");

	if (file.is_open())
	{
		
		file >> ip;
		
		file >> port;
	}
	cout << ip << port;
	/*
	CStdioFile file(_T(".//client.s"), CFile::modeNoTruncate | CFile::modeReadWrite);
	CString strLine = _T("");
	while (file.ReadString(strLine))
	{
		if (strLine.Left(2) == _T("ip"))
		{
			strIP = strLine.Mid(2);
		}
		if (strLine.Left(4) == _T("port"))
		{
			strPort = strLine.Mid(4);
		}
	}
	*/

}

int _tmain(int argc, _TCHAR* argv[])
{
	memset(ip, 0, 20);
	port = 5000;

	load_setting();
	Server svr(ip,port,10);
	svr.run();

	system("pause");
	return 0;
}


