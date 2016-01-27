#include "stdafx.h"
#include "Commander.h"
#include <string>
#include <map>
#include <fstream>

using namespace std;



int Commander::load_setting()
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

	return 0;
}

void Commander::parse_setting(string setting)
{
	string::iterator iter;
	string key = "";
	string val = "";
	if ((iter = find(setting.begin(), setting.end(), '=')) != setting.end())
	{
		key.assign(setting.begin(), iter);
		val.assign(iter + 1, setting.end());
		m_map_settings[key] = val;
	}
}


Commander::Commander()
{
}


Commander::~Commander()
{
}


// load settings
int Commander::initialize()
{
	//1. load all settins for command
	
	return 0;
}