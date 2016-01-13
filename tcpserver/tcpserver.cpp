// tcpserver.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Server.h"
#include <fstream>
#include <map>
#include <algorithm>
#include "tcpserver.h"
using namespace std;

#define MAX_FILENAME_LENGTH		0x40

char ip[20] = "127.0.0.1";
int port = 6000;
char *setting_file;

map<string, string> map_settings;

int _tmain(int argc, _TCHAR* argv[])
{
	load_setting();

	Server svr(map_settings["ip"], atoi(map_settings["port"].data()) , 10);
	svr.run();

	system("pause");
	return 0;
}

int init()
{
	return 0;
}

int load_setting()
{
	char buf[1024];                //临时保存读取出来的文件内容
	string message;
	ifstream infile;
	infile.open(".//myfile.txt");
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
		val.assign(iter+1, setting.end());
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