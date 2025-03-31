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
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	// connecting
	if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
		perror("Connection failed");
		close(sock);
		exit(EXIT_FAILURE);
	}

	// two way chatting
	while (1) {
		// gets client massage
		printf("Client: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		send(sock, buffer, strlen(buffer), 0);

		memset(buffer, 0, BUFFER_SIZE);
		int bytes_received = read(sock, buffer, BUFFER_SIZE);
		if (bytes_received <= 0) {
			printf("Server Failed");
			close(sock);
			exit(EXIT_FAILURE);
		}
		printf("Server: %s", buffer);
	}

	close(sock);
	return 0;
}
