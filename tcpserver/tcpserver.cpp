// tcpserver.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Server.h"


int _tmain(int argc, _TCHAR* argv[])
{
	string stIP = "172.18.117.13";
	Server svr(stIP,5000,10);
	svr.run();

	system("pause");
	return 0;
}

