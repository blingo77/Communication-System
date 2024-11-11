#include "..\headers\serverCommand.h"
#include <iostream>
#include "../headers/serverFunctions.h"

using namespace std;

// map for the commands
unordered_map<string, CommandType> commandMap = {
		{"/exit", CommandType::EXIT},
		{"/info", CommandType::INFO}
};

namespace cmd{

	void findCommand(string& command, const SOCKET& clientSocket) {

		auto it = commandMap.find(command);

		if (it != commandMap.end()) {

			CommandType commandType = it->second;

			switch (commandType) {
				case CommandType::EXIT:
					exitCommand(clientSocket);
					break;
				case CommandType::INFO:
					infoComamnd(clientSocket);
					break;
			}
		}
	}

	void exitCommand(const SOCKET& clientSocket){

		string msg = "You are now exiting a room!\n";
		ServerFuncs::broadCastAlert(msg, clientSocket);
	}

	void infoComamnd(const SOCKET& clientSocket){

		string msg = "Getting info for commands\n";
		ServerFuncs::broadCastAlert(msg, clientSocket);
	}
}