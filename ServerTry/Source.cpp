#include <iostream>
#include <stdio.h>
#include <vector>
#include <winsock.h>

using namespace std;

struct sockaddr_in srv;
#define PORT 9909

int main()
{
	int nRet = 0;
	WSADATA ws;
	//Initialize the socket
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
	{
		std::cout << "Problem with initialization" << std::endl ;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "No problem with initialization" << std::endl;
	}

	//Open the socket
	int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0)
	{
		std::cout << "Problem with socket opening";
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "No problem in socket opening" << std::endl << nSocket << endl ;
	}

	//Initialize the sockaddr_in environment
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	size_t mem_size = 8;
	memset(&(srv.sin_zero), 0, mem_size);

	//Bind the local port
	nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0)
	{
		cout << "Failed to bind local port" << endl ;
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "Connected successfully bind to local port" << endl;
		
	}

	//Listen to the port
	int backlog_num = 5 ; //The active amount of requests that the server can listen to at a time
	nRet = listen(nSocket, backlog_num);
	if (nRet < 0)
	{
		cout << "Failed to start listening to local port" << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "Successfully started listening to local port" << endl;
	}
}