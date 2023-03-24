#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <reedsolomon.h>

#include <stdlib.h>
#include <time.h>


int main() {
	srand(time(NULL));

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		perror("Failed to create socket");
		return errno;
	}

	hostent *server;

	server = gethostbyname("localhost");

	if (server == NULL) {
		perror("Failed to get host name");
		return errno;
	}

	sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(9999);
	memcpy(&sock_addr.sin_addr.s_addr, server->h_addr, server->h_length);

	if (connect(sockfd, (sockaddr*) &sock_addr, sizeof(sock_addr)) < 0) {
		perror("Failed to connect to socket");
		return errno;
	}


	int N = 255;
	int K = 100;

	GPoly g = gen_g(N - K + 1);


	while (1) {
		char buffer[K];
		for (int i = 0; i < K; i++)
			buffer[i] = 0;

		int i = 0;

		while (true) {
			char ch = getc(stdin);
			if (ch == '\n')
				break;
			buffer[i++] = ch;
		}

		GPoly p(0);

		for (int i = 0; i < K; i++) {
			p.data.push_back(buffer[i]);
		}

		GPoly c = encode(p, g, N, K);
		c.fit(N);

		char data[N];

		for (int i = 0; i < N; i++) {
			if ((rand() % 100) < 30)
				data[i] = rand() & 255;
			else
				data[i] = c.data[i].value;
		}

		if (write(sockfd, data, N) < 0) {
			perror("Failed to write to server");
			return errno;
		}
	}

	close(sockfd);
}
