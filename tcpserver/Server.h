#pragma once
// c++ related
#include <iostream>
#include <string>
#include <vector>

//windows socket related
#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib")

//namespace and define
using namespace std;


class Server
{
public:
	Server(string ip, int port, int client_count);
	~Server(void);

	int init();		// initialize server settings
	bool run();		// run server

private:
	// attribute
	SOCKET m_server_socket;					// server socket instance
	vector<SOCKET> m_vec_clients;			// clients waiting room
	int	m_server_port;
	int m_client_maxcount;					// max client counts for listening
	string m_server_ip;
	struct sockaddr_in m_server_addr;		// server address information

	// method
	static DWORD WINAPI thread_Listen_Client(LPVOID lp);
	static DWORD WINAPI thread_communicate(LPVOID lp);
};

