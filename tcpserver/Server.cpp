#include "StdAfx.h"
#include "Server.h"


Server::Server(string ip, int port, int client_count)
{
	m_server_socket = INVALID_SOCKET;
	m_server_ip = ip;
	m_server_port = port;
	m_client_maxcount = client_count;
	m_vec_clients.clear();
	m_hEventForListen = CreateEvent(NULL,TRUE,FALSE,_T("Accepted"));
	m_isbreak = false;
	//m_vec_clients.reserve(10);
}


Server::~Server(void)
{
}


int Server::init()
{
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2,2), &ws) != 0)
	{
		cout << "Failed to WSAstartup with error " << GetLastError() << endl;
		return 1;
	}

	// create server socket instance
	m_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (m_server_socket == INVALID_SOCKET)
	{
		cout << "Failed to create server socket with error " << GetLastError() << endl;
		return 2;
	}

	// socket address setting
	m_server_addr.sin_family = AF_INET;
	m_server_addr.sin_addr.s_addr = inet_addr(m_server_ip.data());
	m_server_addr.sin_port = m_server_port;
	memset(m_server_addr.sin_zero, 0, sizeof(m_server_addr.sin_zero));
	if(bind(m_server_socket, (sockaddr*)&m_server_addr, sizeof(m_server_addr)) != 0)
	{
		cout << "Failed to bind socket " << m_server_socket << "with " << m_server_ip << " at " << m_server_port <<endl;
		cout << "Error : " << GetLastError() << endl;
		return 3;
	}
	// listen socket
	if (listen(m_server_socket, m_client_maxcount) != 0)
	{
		cout << "Failed to listen socket" << m_server_socket << endl;
		return 4;
	}
	
	cout << "Succeed to initialize server socket.\n Server bind with ip " << m_server_ip <<" at port" << m_server_port<<endl;
	return 0;

}

// accept connection from client
DWORD WINAPI Server::thread_Listen_Client(LPVOID lp)
{
	Server *pthis = (Server*)lp;
	int addr_len = sizeof(sockaddr_in);
	SOCKET client_sock;
	while (1)
	{
		if((client_sock = accept(pthis->m_server_socket,(sockaddr*)&(pthis->m_server_addr), &addr_len)) != INVALID_SOCKET)
		{
			cout << "new client whose socket: " << client_sock << "connected. " << endl;
			SetEvent(pthis->m_hEventForListen);
			//pthis->m_vec_clients.push_back(client_sock);
		}
		Sleep(400);
	}
	return 0;
}

void Copy()
{
	SHFILEOPSTRUCT fop;
	ZeroMemory(&fop, sizeof fop);
	fop.wFunc = FO_COPY;
	fop.pFrom = _T("F:\\Macau_Code\\MO_DEVICE\\Device\\bin\\TVM.exe\0");
	fop.pTo = _T("\\\\172.18.119.187\\tvm\0");

	SHFileOperation(&fop);
}

// communicating with client
DWORD WINAPI Server::thread_send(LPVOID lp)
{
	Server* pthis = (Server*)lp;
	string strCmd;
	while(1)
	{
		cout << '\t'<< endl;
		getline(cin, strCmd, '\n');
		if (strCmd == "cp")
		{
			Copy();
			Sleep(400);
			continue;
			// ÒÆ¶¯ÎÄ¼þ
			//ReplaceFile();

		}
		if (strCmd == "exit")
		{
			pthis->m_isbreak = true;
			return 1;
		}

		if (pthis->m_vec_clients.empty())
		{
			// do nothing
		}
		else
		{
			int len = 0;
			if ((len = send(pthis->m_vec_clients.front().first, strCmd.data(), strCmd.size(), 0)) != 0)
			{
				//cout << "Send message " << len << endl;
			}
		}
		
		Sleep(400);
	}
	return 0;
}

DWORD WINAPI Server::thread_recv(LPVOID lp)
{
	Server* pthis = (Server*)lp;
	string strCmd;
	char buf[512] = {};
	memset(buf,0, 512);
	while (1)
	{

		int len = 0;
		if (!pthis->m_vec_clients.empty())
		{
			if (recv(pthis->m_vec_clients[0].first, buf, 512, 0) > 0)
			{
				cout << inet_ntoa(pthis->m_vec_clients[0].second.sin_addr) << ": " << buf << endl;
			}
		}
		Sleep(400);
	}
	return 0;
}


bool Server::run()
{
	HANDLE hListen;
	HANDLE hSend;
	HANDLE hRecv;

	if (init() == 0)
	{
		cout << "Server running..." << endl;

		hSend = CreateThread(NULL, 0, thread_send, this, 0, NULL);
		hRecv = CreateThread(NULL, 0, thread_recv, this, 0, NULL);

		sockaddr_in sock;
		int addr_len = sizeof(sockaddr_in);

		SOCKET client_sock;

		while (1)
		{
			if ((client_sock = accept(m_server_socket, (sockaddr*)&sock/*(m_server_addr)*/, &addr_len)) != INVALID_SOCKET)
			{
				cout << "new client whose socket: " << client_sock << "connected. " << endl;
				SetEvent(m_hEventForListen);
				
				m_vec_clients.push_back(make_pair(client_sock, sock)/*client_sock*/);

			}
			if (m_isbreak)
			{
				TerminateThread(thread_send, 0);
				WSACleanup();
				closesocket(m_server_socket);
				return true;
			}
			Sleep(400);
		}


	/*	hListen = CreateThread(NULL, 0, thread_Listen_Client, this,0, NULL);
		if(hListen == NULL){
			cout << "failed to create accept thread with error " << GetLastError() << endl;
			return false;
		}*/

		return true;
	}
	return false;
}

