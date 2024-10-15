#ifndef  SERVER_H
#define SERVER_H

#include <vector>

namespace srv {
	class Server {

	public:

		Server();
		Server(int portNumber);

		void test();

	private:
		int port;

		int load_WSA_dll();
		SOCKET socket();
		int bindSocket();
		int listen();
		SOCKET acceptSocket();
		void broadCastMessage();
		int receiveData();
	};

}


#endif // ! SERVER_H

