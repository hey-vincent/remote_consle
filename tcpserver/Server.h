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

	// interface
	int init();		// initialize server settings
	bool run();		// run server

private:
	// attribute
	int	m_server_port;									// server socket port
	int m_client_maxcount;									// max client counts for listening
	string m_server_ip;									// server ip
	SOCKET m_server_socket;								// server socket instance
	
	struct sockaddr_in m_server_addr;						// server address information
	vector<pair<SOCKET, sockaddr_in>> m_vec_clients;			// clients waiting room
	HANDLE m_hEventForListen;
	bool	  m_isbreak;


	//method
	void push(SOCKET client_sock, sockaddr_in client_addr);	// arrange new client 
	SOCKET client();										// get current client
	bool Distribute(const char*);

	// thread
	HANDLE m_hListen;
	HANDLE m_hSend;
	HANDLE m_hRecv;

	static DWORD WINAPI thread_Listen(LPVOID lp);
	static DWORD WINAPI thread_send(LPVOID lp);
	static DWORD WINAPI thread_recv(LPVOID lp);

	

	
};

