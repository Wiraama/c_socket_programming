#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
	int sock;
	struct sockaddr_in server_address;
	char buffer[BUFFER_SIZE];

	// create socket
	sock = socket(AF_
