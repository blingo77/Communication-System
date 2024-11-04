#ifndef  SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <mutex>
#include <unordered_map>

using namespace std;

namespace srv {
	class Server {

	public:

		unordered_map<int, vector<SOCKET>> roomMap;
		vector<SOCKET> allConnectedSockets;

		Server();
		Server(int portNumber);

		SOCKET start();
		void stop(SOCKET socketToClose);
		SOCKET acceptSocket(SOCKET serverSocket);

	private:

		mutex mtx;
		int port;

		int load_WSA_dll();
		SOCKET buildSocket();
		int bindSocket(int port, SOCKET serverSocket);
		int listenForConnection(SOCKET serverSocket);

	};

}

#endif // ! SERVER_H