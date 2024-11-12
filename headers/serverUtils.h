#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <WinSock2.h>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include "serverCommand\serverCommand.h"

using namespace std;

namespace ServerUtils {

	void removeFromRoom(const SOCKET& clientSocket, unordered_map<int, vector<SOCKET>>& roomMap);
	bool checkCommand(string& msg, const SOCKET& clientSocket, unordered_map<int, vector<SOCKET>>& roomMap);
}

#endif // !SERVER_UTILS_H