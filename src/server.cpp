#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <tchar.h>
#include "../headers/server.h"
#include <string>
#include <mutex>
#include <thread>
#include <cstdlib>

using namespace std;

namespace srv {

	// default constructor
	Server::Server(){
		port = 55555;

	}

	// constructor
	Server::Server(int portNumber)
		: port(portNumber){
	}

	SOCKET Server::start(){

		SOCKET servSocket;
		SOCKET acceptedSocket;

		this->load_WSA_dll();
		servSocket = this->buildSocket();
		bindSocket(this->port, servSocket);
		this->listenForConnection(servSocket);

		return servSocket;
	}
	                                                                           
	void Server::stop(SOCKET socketToClose = NULL){

		WSACleanup();
		if (socketToClose != NULL) {

			closesocket(socketToClose);
			cout << "Socket was closed." << endl;
		}
	}

	// Loads the Windows Socket API dll file
	int Server::load_WSA_dll(){

		WSADATA wsaData;						// WSAdata struct
		int wsaErr;

		WORD wVersRequested = MAKEWORD(2, 2);	// (2, 2) = Winsock version 2.2
		wsaErr = WSAStartup(wVersRequested, &wsaData);

		if (wsaErr == 0) {

			cout << "Winsock dll was found!" << endl;
			cout << "dll status: " << wsaData.szSystemStatus << endl;
		}
		else {
			cerr << "Winsock dll not found." << endl;
			this->stop();
			return 0;
		}
		return 0;
	}

	// create the server socket
	SOCKET Server::buildSocket(){

		// create a server socket
		SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	

		// check to see if the server socket was created successfully
		if (serverSocket != INVALID_SOCKET) {

			cout << "Server socket was successfully created!" << endl;
		}
		else {
			cerr << "Error creating the server socket: " << WSAGetLastError() << endl;
			this->stop();
			closesocket(serverSocket);	// close the socket
		}

		return serverSocket;
	}
	
	// Associate a local address wiith a socket
	int Server::bindSocket(int port, SOCKET serverSocket){


		/*
			sockaddr_in sturcture:

			Used by Window Sockets to specify a local or remote endpoint address to which
			to connect a socket

			struct sockaddr_in{
				short sin_family;			// Adress family (Must be AF_INET)
				unsigned short sin_port;	// the IP port
				struct in_addr sin_addrl	// the IP address
				char sin_zero[8]			// padding to make the structure the same size
				}
		*/

		sockaddr_in service;
		service.sin_family = AF_INET;		// sets the address family for the socket
		service.sin_port = htons(port);		// sets the value of the port in TCP network byte order

		// converts the IP address into its binary form and stores it in service.sin_addr.s_addr
		InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);

		if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) != SOCKET_ERROR) {

			cout << "The socket was binded successfully!" << endl;
		}
		else {
			cerr << "Failed to bind the socket, Error: " << WSAGetLastError << endl;
			this->stop(serverSocket);
		}

		return 0;
	}
	
	// makes a socket in a state in which it is listening for a incoming connection
	int Server::listenForConnection(SOCKET serverSocket){
		/*
			listen function:

			int listen(SOCKET serverSocket, int backlog)

			serverSocket: an unconnected Socket
			backlog: the maximum number of connections allowed
        */

		if (listen(serverSocket, 1) != SOCKET_ERROR) {

			cout << "Now listening for connections..." << endl;
		}
		else {

			cerr << "Problem Occured while listening for a connection : " << WSAGetLastError();
			return 0;
		}
		
		return 0;
	}

	// pauses the server until the user connects with the server
	SOCKET Server::acceptSocket(SOCKET serverSocket){
		/*
			accept() function:

			- this is a blocking function

			SOCKET accept(SOCKET s, struct sockaddr* addr, int* addrlen)

			-s: descriptor that identifies a socket that has been placed in a listening
			state with the listen() function

			-addr: optional structure containing the client address information
			-addrlen: optional size of the address structure

			-if no errors, accept() returns a value of type SOCKET that is a descriptor
			for the new socket that is connected to the client. The original socket can be
			used to listen for more incoming calls.
        */

		SOCKET newClientSocket;

		newClientSocket = accept(serverSocket, NULL, NULL);

		if (newClientSocket != INVALID_SOCKET) {
			
			cout << "Connected..." << endl;
			lock_guard<mutex> lock(this->mtx);
			allConnectedSockets.push_back(newClientSocket);
		}
		else {
			cerr << "Accept failed: " << WSAGetLastError() << endl;
			Server::stop();
			return -1;
		}

		return newClientSocket;
	}

	void Server::broadCastAlert(string alert, SOCKET clientSocket) {

		lock_guard<mutex> lock(this->mtx);

		// send the alert to the client socket
		send(clientSocket, alert.c_str(), alert.size(), 0);
	}

	// broadcast messages sent between users/clients
	void Server::broadCastMessage(string buffer, SOCKET senderSocket){

		lock_guard<mutex> lock(this->mtx);

		string msg ;

		for (SOCKET client : allConnectedSockets) {

			// if client is the senderSocket let them know they sent it with "you"
			msg = client != senderSocket ? to_string(client) + ": " + buffer + "\n" : "You: " + buffer + "\n";
			send(client, msg.c_str(), msg.size(), 0);
			
		}
	}

	void Server::routeMessage(string msg, SOCKET clientSocket) {

		/*
			go through each room number in the room hashmap and check its vector
			if the sender socket exists in that vector. If it does send all the sockets
			in that vector the message the sender socket sent.
		*/

		// i is set to 1 since our rooms start at 1
		for (int i = 0; i < this->roomMap.size(); i++) {

			auto it = find(roomMap[i].begin(), roomMap[i].end(), clientSocket);

			if (it != roomMap[i].end()) {
				
				for (SOCKET roomSockets : this->roomMap[i]) {
					send(roomSockets, msg.c_str(), msg.size(), 0);
				}
			}
			else {
				cout << "Socket was not in room: " << i << endl;
			}
		}

	}

	int Server::receiveMessages(SOCKET clientSocket){

		/*
			recv(function):

			- this function recieves data from a connected socket.

			int recv(SOCKET s, char* buf, int len, int flags)

			-s: The descriptor that identifies a connected socket.
			-buf: A pointer to the buffer to recieve incoming data.
			-len: The length in bytes of the buffer pointed to by the buf parameter.
			-flags: optional set of flags that influences the behavior of this function

			- if no errors, then recv returns the number of bytes received. If the
			connection has been closed gracefully, the return value is zero. Otherwise
			SOCKET_ERROR is returned
		*/

		char incomingBuffer[200] = "";

		while (true) {

			int byteCount = recv(clientSocket, incomingBuffer, sizeof(incomingBuffer), 0);

			string msg = incomingBuffer;

			if (byteCount < 0) {
				cerr << "Error recieving data: " << WSAGetLastError() << endl;
				break;
			}
			else {
				cout << "Recieved Data: " << incomingBuffer << endl;
			}

			//broadCastMessage(msg, clientSocket);
			this->routeMessage(msg, clientSocket);
		}

		return 0;
	};

	int Server::receiveIntData(SOCKET clientSocket) {
		
		char incomingBuffer[200] = "";

		while (true) {

			int num;
			int byteCount = recv(clientSocket, incomingBuffer, sizeof(incomingBuffer), 0);

			string msg = incomingBuffer;

			if (byteCount < 0) {
				cerr << "Error recieving data: " << WSAGetLastError() << endl;
				break;
			}
			else {
				cout << "Recieved Data: " << incomingBuffer << endl;
				num = atoi(incomingBuffer);	// turns the char array into the one integer entered

				return num;
			}

			return 0;

		}

	}

	void Server::setSocketRoom(int roomNum, SOCKET clientSocket) {

		this->roomMap[roomNum].push_back(clientSocket);

		for (const auto& vals : this->roomMap[roomNum]) {

			cout << vals << ' ,' << endl;
		}
	}
}