#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 5

void logexit(const char *str) {
	if (errno)
		perror(str);
	else
		puts(str);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	uint32_t counter = 0;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in clt,
			   srv = {.sin_family = AF_INET,
				   .sin_port = htons(51515),
				   .sin_addr.s_addr = htonl(INADDR_ANY)
				};

	// Creating socket and binding
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		logexit("socket");
	if (bind(sock, (struct sockaddr *)&srv, sizeof(srv)) < 0)
		logexit("binding");

	while (1) {
		int addrlen = sizeof(clt);

		fprintf(stdout, "Waiting for client\n");
		listen(sock, 1);

		// New client. Accepting.
		int newsock = accept(sock, (struct sockaddr *)&clt, &addrlen);
		if (newsock == -1)
			logexit("accept");

		// Receiving.
		int nrcv = recv(newsock, buffer, BUFFER_SIZE, MSG_WAITALL);
		if (nrcv == 0)
			logexit("recv");
		
		fprintf(stdout, "%s\n", buffer);
		
		// Sending an ack.
		send(newsock, "ack", sizeof("ack"), 0);

		close(newsock);
	}
	close(sock);

	exit(EXIT_SUCCESS);
}
