#include <WinSock2.h>
#include <iostream>
#include "headers/server.h"

using namespace std;

namespace srv {
	Server::Server(){
		port = 55555;
	}

	Server::Server(int portNumber)
		: port(portNumber) {
	}
	void Server::test()	{

		cout << port << endl;
	}
	int Server::load_WSA_dll(){
		return 0;
	}
	SOCKET Server::socket()
	{
		return SOCKET();
	}
	int Server::bindSocket()
	{
		return 0;
	}
	int Server::listen()
	{
		return 0;
	}
	;


}