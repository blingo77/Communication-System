#include <WinSock2.h>
#include "../headers/server.h"

int main() {

	srv::Server ser;
	ser.start();
	return 0;
}