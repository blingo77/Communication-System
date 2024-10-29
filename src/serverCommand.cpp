#include "..\headers\serverCommand.h"
#include <iostream>

using namespace std;

ServerCommand::ServerCommand(Server* server) : server(server){

	commandMap = {
		{"/exit", CommandType::EXIT},
		{"/info", CommandType::INFO}
	};
}

void ServerCommand::checkCommand(string& command, const SOCKET& clientSocket){

	auto it = this->commandMap.find(command);

	if (it != this->commandMap.end()) {

		CommandType commandType = it->second;

		switch (commandType) {
			case CommandType::EXIT:
				this->exitCommand(clientSocket);
				break;
			case CommandType::INFO:
				this->infoComamnd(clientSocket);
				break;
		}
	}
}

void ServerCommand::exitCommand(const SOCKET& clientSocket){

	string msg = "You are now exiting a room!\n";
	this->server->broadCastAlert(msg, clientSocket);
}

void ServerCommand::infoComamnd(const SOCKET& clientSocket){

	string msg = "Getting info for commands\n";
	this->server->broadCastAlert(msg, clientSocket);
}
