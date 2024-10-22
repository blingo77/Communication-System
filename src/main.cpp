#include <WinSock2.h>
#include "../headers/server.h"
#include "../headers/serverManager.h"
using namespace std;
int main() {

	/*
	srv::Server room1;
	thread room1Thread(&srv::Server::start, &room1);
	room1Thread.join();
	*/

	ServerManager servManager;
	thread server1Thread(&ServerManager::startServer, &servManager);
	server1Thread.join();


	return 0;
}