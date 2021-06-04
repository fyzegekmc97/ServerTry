#include <iostream>
#include <stdio.h>
#include <vector>
#include <winsock.h>

using namespace std;

struct sockaddr_in srv;
#define PORT 9909

fd_set fr, fw, fe; //fd_set is just a set of file descriptors and fr is holding descriptors from which we read, fw hols the ones to which we write and fe which is used for handling exceptions.

int nMaxFd;

int main()
{
	int nRet = 0;
	WSADATA ws;
	//Initialize the socket
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
	{
		std::cout << "Problem with initialization" << std::endl ;
		WSACleanup();
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
		WSACleanup();
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
	
	int nOptVal = 0;
	int nOptLen = sizeof(nOptVal);
	nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen); 
	if (!nRet)
	{
		cout << endl << "Set socket operation successful"; 
	}
	else
	{
		cout << endl << "Failed";
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	//Make the connection a binding socket
	//u_long optval = 0; //0 for blocking and 1 for nonblocking, the socket binding is blocking by default
	//nRet = ioctlsocket(nSocket, FIONBIO, &optval);
	//if (nRet != 0)
	//{
	//	cout << endl << "iotlsocket call not passed" ;
	//	WSACleanup();
	//}
	//else
	//{
	//	cout << endl << "iotlsocket call passed";
	//}

	//Bind the local port
	nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0)
	{
		cout << "Failed to bind local port" << endl ;
		WSACleanup();
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
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "Successfully started listening to local port" << endl;
	}

	int n = 100;
	unsigned short int required_file_desc_count = 64; //Maximum number of file descriptors that we can use with the structure fd_set
	for (int nIndex = 0; nIndex < 64; nIndex++)
	{
		FD_SET(n, &fr);
		n++;
	}

	cout << endl << fr.fd_count << endl ;

	for (int nIndex = 0; nIndex < 64; nIndex++)
	{
		cout << fr.fd_array[nIndex] << endl ;
	}

	nMaxFd = nSocket;

	//Keep waiting for new requests and proceed as per the request
	struct timeval tv; //Timeout structure
	tv.tv_sec = 1; //Waits for a change in any of the file descriptors for the defined amount of time, this in seconds
	tv.tv_usec = 0; //Waits for a change in any of the file descriptors for the defined amount of time, this in miliseconds

	

	while (1)
	{
		FD_ZERO(&fr);
		FD_ZERO(&fe);
		FD_ZERO(&fw);

		FD_SET(nSocket, &fr);
		FD_SET(nSocket, &fe);

		cout << endl << "Before select call: " << fr.fd_count;

		nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv);
		if (nRet > 0)
		{
			//Someone communicates with the server
			cout << endl << "Someone connected";
		}
		else if (nRet == 0)
		{
			//No connection or communication requested or socket descriptors are not ready
			cout << endl << "Nothing on port: " << PORT;
			
		}
		else
		{
			//Connection failed
			exit(EXIT_FAILURE);
			WSACleanup();
		}
		cout << endl << "After select call: " << fr.fd_count;
		Sleep(2000);
	}
}