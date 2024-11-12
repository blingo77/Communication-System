#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H

#include <string>
#include <unordered_map>
#include <WinSock2.h>

using namespace std;

enum class CommandType {
	EXIT,
	INFO,
	COLOR,
	ART
};

namespace cmd{
	void findCommand(string& command, const SOCKET& clientSocket, unordered_map<int, vector<SOCKET>>& roomMap);
	void exitCommand(const SOCKET& clientSocket, unordered_map<int, vector<SOCKET>>& roomMap);
	void infoComamnd(const SOCKET& clientSocket);
	void colorCommand(const SOCKET& clientSocket);
	void artCommand(const SOCKET& clientSocket);
}

#endif // !SERVER_COMMAND_H

