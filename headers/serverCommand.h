#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H

#include <string>
#include <unordered_map>
#include <WinSock2.h>


using namespace std;

enum class CommandType {
	EXIT,
	INFO
};

namespace cmd{
	void findCommand(string& command, const SOCKET& clientSocket);
	void exitCommand(const SOCKET& clientSocket);
	void infoComamnd(const SOCKET& clientSocket);
}

#endif // !SERVER_COMMAND_H

