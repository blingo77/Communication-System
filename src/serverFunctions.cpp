#include "../headers/serverFunctions.h"
#include <mutex>

using namespace std;

mutex mtx;

namespace ServerFuncs {

	void broadCastAlert(string alert, const SOCKET& clientSocket) {

		lock_guard<mutex> lock(mtx);

		// send the alert to the client socket
		send(clientSocket, alert.c_str(), alert.size(), 0);
	}

	// broadcast messages sent between users/clients
	void broadCastMessage(string buffer, SOCKET senderSocket, vector<SOCKET>& allSockets) {

		lock_guard<mutex> lock(mtx);

		string msg;

		for (SOCKET client : allSockets) {

			// if client is the senderSocket let them know they sent it with "you"
			msg = client != senderSocket ? to_string(client) + ": " + buffer + "\n" : "You: " + buffer + "\n";
			send(client, msg.c_str(), msg.size(), 0);

		}
	}

	void routeMessage(string buffer, SOCKET clientSocket, unordered_map<int, vector<SOCKET>>& roomMap) {

		/*
			go through each room number in the room hashmap and check its vector
			if the sender socket exists in that vector. If it does send all the sockets
			in that vector the message the sender socket sent.
		*/

		string msg;

		// use an iterator for the roomMap hashmap
		for (auto it = roomMap.begin(); it != roomMap.end(); ++it) {

			// check if clientSocket is in the current vector of the hashmap
			auto checkVector = find(it->second.begin(), it->second.end(), clientSocket);

			// if it didnt reach the end then execute
			if (checkVector != it->second.end()) {

				for (SOCKET roomSockets : it->second) {

					// if roomSockets is client sockets send "You:" preceeding 'msg'
					msg = roomSockets != clientSocket ? to_string(roomSockets) + ": " + buffer + "\n" : "";
					send(roomSockets, msg.c_str(), msg.size(), 0);
				}
			}
			else {
				cout << "Socket was not in room: " << it->first << endl;
			}
		}

	}

	int receiveMessages(const SOCKET& clientSocket, unordered_map<int, vector<SOCKET>>& roomMap) {

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

			// check to see if they are entering a command
			if (msg[0] == '/') {
				cout << "COMMAND" << endl;
				//this->serverCommands->checkCommand(msg, clientSocket);

			}
			// if not, route the message to the room vector of sockets
			else {
				routeMessage(msg, clientSocket, roomMap);
			}

		}

		return 0;
	};

	int receiveIntData(const SOCKET& clientSocket) {

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

	void setSocketRoom(int roomNum, const SOCKET& clientSocket, unordered_map<int, vector<SOCKET>>& roomMap) {

		roomMap[roomNum].push_back(clientSocket);

		for (const auto& vals : roomMap[roomNum]) {

			cout << vals << ' ,' << endl;
		}

		broadCastAlert("You have entered a room!", clientSocket);
	}
}