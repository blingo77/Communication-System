#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H

#include <string>
#include <unordered_map>
#include <WinSock2.h>
#include "./server.h"


using namespace std;
using namespace srv;

enum class CommandType {
	EXIT,
	INFO
};

class ServerCommand{
public:
	ServerCommand(Server* server);
	void checkCommand(string& command, const SOCKET& clientSocket);

private:

	Server* server;
	// map for the commands
	unordered_map<string, CommandType> commandMap;

	void exitCommand(const SOCKET& clientSocket);
	void infoComamnd(const SOCKET& clientSocket);
};

#endif // !SERVER_COMMAND_H

