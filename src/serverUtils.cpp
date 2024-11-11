#include "..\headers\serverUtils.h"

namespace ServerUtils {
	void ServerUtils::removeFromRoom(const SOCKET& clientSocket, unordered_map<int, vector<SOCKET>>& roomMap){


		// use an iterator for the roomMap hashmap
		for (auto it = roomMap.begin(); it != roomMap.end(); ++it) {

			// check if clientSocket is in the current vector of the hashmap
			auto vectorToRemove = find(it->second.begin(), it->second.end(), clientSocket);

			// if it didnt reach the end then execute
			if (vectorToRemove != it->second.end()) {

				it->second.erase(vectorToRemove);
				cout << "SOCKET: " << to_string(clientSocket) << " was removed from room " << it->first << endl;
			}
		}
	}

	bool ServerUtils::checkCommand(string& msg, const SOCKET& clientSocket){

		bool isCommand;

		if (msg[0] == '/') {

			cout << "COMMAND" << endl;
			cmd::findCommand(msg, clientSocket);
			isCommand = true;
		}
		else {

			isCommand = false;
		}

		return isCommand;
	}
}