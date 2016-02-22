#include "StdAfx.h"
#include "Server.h"


Server::Server(string ip, int port, int client_count)
{
	m_server_socket = INVALID_SOCKET;
	m_server_ip = ip;
	m_server_port = port;
	m_client_maxcount = client_count;
	m_vec_clients.clear();
	m_isbreak = false;

	m_hQuitEvent = CreateEvent(NULL, TRUE, FALSE, _T("Quit"));
	ResetEvent(m_hQuitEvent);
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
DWORD WINAPI Server::thread_Listen(LPVOID lp)
{
	Server *pthis = (Server*)lp;
	int addr_len = sizeof(sockaddr_in);
	SOCKET client_sock;
	sockaddr_in client_addr;
	while (1)
	{
		if((client_sock = accept(pthis->m_server_socket,(sockaddr*)&client_addr, &addr_len)) != INVALID_SOCKET)
		{
			cout << endl <<inet_ntoa(client_addr.sin_addr) << client_sock << "connected. " << endl;
			pthis->push(client_sock, client_addr);
			SetEvent(pthis->m_hEventForListen);
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
	//WaitForSingleObject(pthis->m_hQuitEvent, INFINITE);
	while(1) 
	{
		cout << "\t>";
		getline(cin, strCmd, '\n');
		if (memcmp(strCmd.data(), "quit", 4) == 0)	// Release this mutex so that main thread can quit.
		{
			SetEvent(pthis->m_hQuitEvent);
		}

		if (pthis->Distribute(strCmd.data())) // true for this msg should be sent to client ; if false: Distrubute(msg) will process this message 
		{
			int len = 0;
			if ((len = send(pthis->client(), strCmd.data(), strCmd.size(), 0)) > 0)
			{
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
		if (pthis->client() != INVALID_SOCKET)
		{
			if (recv(pthis->client(), buf, 512, 0) > 0)
			{
				cout << inet_ntoa(pthis->m_vec_clients.back().second.sin_addr) << ": " << buf << endl;
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

		m_hSend = CreateThread(NULL, 0, thread_send, this, 0, NULL);	// Thread for sending message to client
		m_hRecv = CreateThread(NULL, 0, thread_recv, this, 0, NULL);	// Thread for receiving reply from client
		m_hLstn = CreateThread(NULL, 0, thread_Listen, this,0, NULL);	// Thread for listening client's connection
		if(m_hLstn == NULL){
			cout << "failed to create accept thread with error " << GetLastError() << endl;
			return false;
		}
		WaitForSingleObject(m_hQuitEvent, INFINITE);					// wait the signal for quit.
		/*WaitForSingleObject(m_hLstn, INFINITE);
		WaitForSingleObject(m_hRecv, INFINITE);
		WaitForSingleObject(m_hSend, INFINITE);*/

		return true;
	}
	return false;
}

void Server::push(SOCKET client_sock, sockaddr_in client_addr)
{
	vector<pair<SOCKET, sockaddr_in>>::iterator iter;
	for (iter = m_vec_clients.begin(); iter != m_vec_clients.end(); iter++)
	{
		if (iter->second.sin_addr.S_un.S_addr == client_addr.sin_addr.S_un.S_addr)
		{
			// if found: update information
			iter->first = client_sock;
			return;
		}
	}
	// if not found: insert
	m_vec_clients.push_back(make_pair(client_sock, client_addr));
}

SOCKET Server::client()
{
	if (!m_vec_clients.empty())
	{
		return m_vec_clients.back().first;
	}
	return INVALID_SOCKET;
}

bool Server:: Distribute( const char* msg)
{
	if (*msg == '\0' || *msg == ' ')
	{
		return false;
	}

	if (memcmp(msg, "clear", 6) == 0 || memcmp(msg, "cls", 4) == 0)
	{
		system("cls");
		return true;
	}

	if (memcmp(msg,"exit", 4) == 0)
	{
		TerminateThread(m_hSend, 0);
		TerminateThread(m_hRecv, 0);
		TerminateThread(m_hLstn, 0);
		
		WSACleanup();
		closesocket(m_server_socket);
		system("exit");
		return false;
	}

	if (memcmp(msg, "debug",5) == 0)
	{
		cout << "  :Replace File.";
		if (client() != INVALID_SOCKET)
		{
			send(client(), "kill", 5, 0);
			Sleep(200);
			// copy file
			Copy();
			Sleep(1500);
			// start
			send(client(), "go", 5, 0);
			return true;
		}
		return false;
	}

	if (memcmp(msg, "rp", 2) == 0)
	{
		cout << "  :Replace File.";
		if (client() != INVALID_SOCKET)
		{
			send(client(), "kill", 5, 0);
			Sleep(200);
			// copy file
			Copy();
			Sleep(1500);
			// start
			send(client(), "go", 5, 0);
			return true;
		}
		return false;
	}

	return true;
}