// tcpserver.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Server.h"


int _tmain(int argc, _TCHAR* argv[])
{
	string stIP = "192.168.0.111";
	Server svr(stIP,5000,10);
	svr.run();

	system("pause");
	return 0;
}

