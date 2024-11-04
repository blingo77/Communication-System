#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <tchar.h>
#include "../headers/server.h"
#include <string>
#include <mutex>
#include <thread>
#include <cstdlib>
#include "../headers/serverCommand.h"

namespace ServerFuncs {

	void broadCastAlert(string alert, const SOCKET& clientSocket);
	void broadCastMessage(string buffer, SOCKET senderSocket, vector<SOCKET>& allSockets);
	void routeMessage(string buffer, SOCKET clientSocket, unordered_map<int, vector<SOCKET>>& roomMap);
	int receiveMessages(const SOCKET& clientSocket, unordered_map<int, vector<SOCKET>>& roomMap);
	int receiveIntData(const SOCKET& clientSocket);
	void setSocketRoom(int roomNum, const SOCKET& clientSocket, unordered_map<int, vector<SOCKET>>& roomMap);
}

#endif // !SERVER_FUNCTIONS_H

