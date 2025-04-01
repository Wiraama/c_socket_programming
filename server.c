#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

// Function to get AI response by calling Python script
char* get_ai_response(const char* input) {
		static char response[BUFFER_SIZE];
    char command[BUFFER_SIZE];
    snprintf(command, sizeof(command), "python3 ai_bot.py \"%s\"", input);
    
    FILE *fp = popen(command, "r");
    if (!fp) {
			perror("Python execution error");
			return "Error";
		}
	
    if (fgets(response, BUFFER_SIZE, fp) == NULL) {
			perror("Error reading AI response");
			return "Error";
		}
    pclose(fp);
    return response;
}

int server_fd, new_socket;

int main() {
	struct sockaddr_in address; // creates address
	socklen_t addr_len = sizeof(address); // store size of address
	char buffer[BUFFER_SIZE]; // store message
	char *ai_res;
	
	//create main socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		perror("Socket cretin error");
		exit(EXIT_FAILURE);
	}

	// adress setting
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	//binding
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("Bind failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	// listening to server
	if (listen(server_fd, 5) < 0) {
		perror("Listening Failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	printf("Listening to port %d...\n", PORT);

	// new conection to client
	new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len);

	if (new_socket < 0) {
		perror("Accept failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	printf("connected to client\n");

	// messaging
	while (1) {
		// read message from client
		int message_bytes = read(new_socket, buffer, BUFFER_SIZE);
		if (message_bytes < 0) {
			printf("Client disconnected");
			break;
		}

		printf("Client: %s\n", buffer);

		// take message from server
		ai_res = get_ai_response(buffer);
		printf("Server: %s", ai_res);
		send(new_socket, ai_res, strlen(ai_res), 0);

		if (strncmp(buffer, "exit", 4) == 0) {
			printf("Exiting connection");
			break;
		}
	}
	close(server_fd);
	close(new_socket);
	 return 0;
}
