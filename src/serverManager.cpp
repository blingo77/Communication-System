#include <WinSock2.h>
#include <iostream>
#include <unordered_map>
#include "..\headers\serverManager.h"

using namespace std;

ServerManager::ServerManager(){
}

void ServerManager::startServer(){

	SOCKET serverSocket;
	SOCKET acceptedSocket;

	string roomChoice = "Pick a room (Enter a Room): \n 1. Room 1 \n 2. Room 2";

	serverSocket = this->server.start();

	while (true) {

		acceptedSocket = this->server.acceptSocket(serverSocket);

		if (acceptedSocket == INVALID_SOCKET) {
			break;
		}

		this->addToRoom(roomChoice, acceptedSocket);

		thread clientThread([this, acceptedSocket]() {
			this->server.receiveMessages(acceptedSocket);
			});

		clientThread.detach();
	}
}

void ServerManager::addToRoom(string message, SOCKET clientSocket){

	int room;
	string welcomeMsg;

	this->server.broadCastAlert(message, clientSocket);

	thread selectRoomThread([this, clientSocket, &room]() {
		room = this->server.receiveIntData(clientSocket);
		});

	selectRoomThread.join();
			// will give us the number entered

	// Make sure they input a valid room
	while (!room) {
		
		this->server.broadCastAlert("Not a valid room!", clientSocket);	// Warn client
		this->server.broadCastAlert(message, clientSocket);
		room = this->server.receiveIntData(clientSocket);		// will give us the number entered

	}

	this->server.setSocketRoom(room, clientSocket);

}
