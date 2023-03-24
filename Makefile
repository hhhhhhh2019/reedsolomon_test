all: server client

server:
	g++ -c server.cpp -o server.o -I ./include -fsanitize=address -g
	g++ server.o reedsolomon.o -o server -lasan

client:
	g++ -c client.cpp -o client.o -I ./include -fsanitize=address -g
	g++ client.o reedsolomon.o -o client -lasan
