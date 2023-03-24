#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <reedsolomon.h>


int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		fprintf(stderr, "Failed to create socket: %d\n", errno);
		return errno;
	}

	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(9999);
	sockaddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
		fprintf(stderr, "Failed to bind to port 9999: %d\n", errno);
		return errno;
	}

	if (listen(sockfd, 10) < 0) {
		fprintf(stderr, "Failed to listen on soket: %d\n", errno);
		return errno;
	}

	int addrlen = sizeof(sockaddr);
	int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);

	if (connection < 0) {
		fprintf(stderr, "Failed to grab connection: %d\n", errno);
		return errno;
	}


	int N = 255;
	int K = 100;

	GPoly g = gen_g(N - K + 1);


	while (connection) {
		char buffer[N];
		for (int i = 0; i < N; i++)
			buffer[i] = 0;
		int read_size = read(connection, buffer, N);

		if (read_size == 0) {
			printf("exiting\n");
			break;
		}

		GPoly c(0);

		for (int i = 0; i < N; i++)
			c.data.push_back(buffer[i]);

		GPoly c1 = decode(c, g, N, K);

		char data[K];

		for (int i = 0; i < K; i++)
			data[i] = c1.data[i].value;

		printf("%s\n", data);
	}

	close(connection);
	close(sockfd);
}
