#ifndef  SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <mutex>

using namespace std;

namespace srv {
	class Server {

	public:

		Server();
		Server(int portNumber);

		SOCKET start();
		void stop(SOCKET socketToClose);
		SOCKET acceptSocket(SOCKET serverSocket);
		void broadCastAlert(string alert, SOCKET clientSocket);
		void broadCastMessage(string buffer, SOCKET senderSocket);
		int receiveMessages(SOCKET clientSocket);
		int receiveIntData(SOCKET clientSocket);

	private:
		int port;
		vector<SOCKET> allConnectedSockets;
		mutex mtx;

		int load_WSA_dll();
		SOCKET buildSocket();
		int bindSocket(int port, SOCKET serverSocket);
		int listenForConnection(SOCKET serverSocket);

	};

}


#endif // ! SERVER_H

