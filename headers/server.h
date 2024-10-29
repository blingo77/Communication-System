#ifndef  SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <mutex>
#include <unordered_map>

class ServerCommand;

using namespace std;

namespace srv {
	class Server {

	public:

		Server();
		~Server();
		Server(int portNumber);

		SOCKET start();
		void stop(SOCKET socketToClose);
		SOCKET acceptSocket(SOCKET serverSocket);
		void broadCastAlert(string alert, const SOCKET& clientSocket);
		void broadCastMessage(string buffer, SOCKET senderSocket);
		int receiveMessages(const SOCKET &clientSocket);
		int receiveIntData(const SOCKET &clientSocket);
		void setSocketRoom(int roomNum, const SOCKET& clientSocket);

	private:

		unordered_map<int, vector<SOCKET>> roomMap;
		int port;
		vector<SOCKET> allConnectedSockets;
		mutex mtx;

		ServerCommand* serverCommands;

		int load_WSA_dll();
		SOCKET buildSocket();
		int bindSocket(int port, SOCKET serverSocket);
		int listenForConnection(SOCKET serverSocket);
		void routeMessage(string msg, SOCKET clientSocket);

	};

}


#endif // ! SERVER_H

