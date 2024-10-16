#ifndef  SERVER_H
#define SERVER_H

#include <vector>

namespace srv {
	class Server {

	public:

		Server();
		Server(int portNumber);

		void start();
		void stop(SOCKET socketToClose);

	private:
		int port;

		int load_WSA_dll();
		SOCKET buildSocket();
		int bindSocket(int port, SOCKET serverSocket);
		int listenForConnection(SOCKET serverSocket);
		SOCKET acceptSocket(SOCKET serverSocket);
		void broadCastMessage();
		int receiveData(SOCKET clientSocket);
	};

}


#endif // ! SERVER_H

