#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <WinSock2.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "server.h"

using namespace std;

class ServerManager {

public:

	ServerManager();

	void startServer();

private:
	srv::Server server;
	

	void addToRoom(string message, SOCKET clientSocket);
};

#endif // !SERVER_MANAGER_H
