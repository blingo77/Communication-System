#include <cstdlib>
#include <iostream>

#include "..\headers\serverCommand\serverCommand.h"
#include "../headers/serverFunctions.h"
#include "../headers/serverCommand/commandMessages.h"
#include "../headers/serverUtils.h"

using namespace std;

// map for the commands
unordered_map<string, CommandType> commandMap = {
		{"/exit", CommandType::EXIT},
		{"/info", CommandType::INFO},
		{"/color", CommandType::COLOR},
		{"/art", CommandType::ART}
};

namespace cmd{

	void findCommand(string& command, const SOCKET& clientSocket, unordered_map<int, vector<SOCKET>>& roomMap) {

		// find the 'command' in the commandMap
		auto it = commandMap.find(command);

		// if not reached the end, then it exists
		if (it != commandMap.end()) {

			// set 'commandType' to the commandType mapped with 'command'
			CommandType commandType = it->second;

			switch (commandType) {
				case CommandType::EXIT:
					exitCommand(clientSocket, roomMap);
					break;
				case CommandType::INFO:
					infoComamnd(clientSocket);
					break;
				case CommandType::COLOR:
					colorCommand(clientSocket);
					break;
				case CommandType::ART:
					artCommand(clientSocket);
					break;
				default:
					string msg = "NOT A VALID COMMAND";
					ServerFuncs::broadCastAlert(msg, clientSocket);
					break;
			}
		}
	}

	// allows user to leave the current room
	void exitCommand(const SOCKET& clientSocket, unordered_map<int, vector<SOCKET>>& roomMap){

		string msg = "You are now exiting a room!\n";
		ServerUtils::removeFromRoom(clientSocket, roomMap);
		ServerFuncs::broadCastAlert(msg, clientSocket);
	}

	// displays info on all of the commands
	void infoComamnd(const SOCKET& clientSocket){

		// send the infoMsg string to clientSocket
		ServerFuncs::broadCastAlert(cmdMsg::infoMsg, clientSocket);
	}

	// allows user to change the color of the font in the terminal
	void colorCommand(const SOCKET& clientSocket) {

		// TODO : make client able to detect differnce of user command and server command
		// this changes the color of the termial font for the server, NOT THE CLIENT

		string changeColor = "color 2";

		system(changeColor.c_str());
	}

	// send the client some beautiful art!
	void artCommand(const SOCKET& clientSocket) {

		// TODO
		//ServerFuncs::broadCastAlert(srvMsg::spider, clientSocket);
	}
}