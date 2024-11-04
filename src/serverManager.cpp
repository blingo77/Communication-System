#include <WinSock2.h>
#include <iostream>
#include <unordered_map>
#include "..\headers\serverManager.h"
#include "../headers/serverFunctions.h"

using namespace std;

ServerManager::ServerManager(){
}

void ServerManager::startServer(){

	SOCKET serverSocket;		// will give us the socket for the server
	SOCKET acceptedSocket;		// will give us the socket for client that is accepted

	string roomChoice = "Pick a room (Enter a Room): \n 1. Room 1 \n 2. Room 2";

	serverSocket = this->server.start();

	while (true) {

		acceptedSocket = this->server.acceptSocket(serverSocket);	// accepted client socket

		if (acceptedSocket == INVALID_SOCKET) {
			break;
		}

		this->addToRoom(roomChoice, acceptedSocket);

		thread clientThread([this, acceptedSocket ]() {
			ServerFuncs::receiveMessages(acceptedSocket, this->server.roomMap);
			});

		clientThread.detach();
	}
}

void ServerManager::addToRoom(string message, SOCKET clientSocket){

	int room;

	string welcomeMsg;

	ServerFuncs::broadCastAlert(message, clientSocket);

	thread selectRoomThread([this, clientSocket, &room]() {
		room = ServerFuncs::receiveIntData(clientSocket);
		});

	selectRoomThread.join();

	// Make sure they input a valid room
	while (!room) {
		
		ServerFuncs::broadCastAlert("Not a valid room!", clientSocket);	// Warn client
		ServerFuncs::broadCastAlert(message, clientSocket);
		room = ServerFuncs::receiveIntData(clientSocket);		// will give us the number entered

	}

	ServerFuncs::setSocketRoom(room, clientSocket, this->server.roomMap);

}
