all: server client

server:
	g++ -c server.cpp -o server.o -I ./include
	g++ server.o reedsolomon.o -o server

client:
	g++ -c client.cpp -o client.o -I ./include
	g++ client.o reedsolomon.o -o client
