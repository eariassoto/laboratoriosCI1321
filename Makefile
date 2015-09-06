Socket.o: Socket.h Socket.cpp
	g++ -c -g Socket.cpp

mirror_client.o: Socket.h Socket.cpp mirror_client.cpp
	g++ -c -g Socket.cpp mirror_client.cpp

mirror_server.o: Socket.h Socket.cpp mirror_server.cpp
	g++ -c -g Socket.cpp mirror_server.cpp

client: mirror_client.o Socket.o
	g++ -Wno-write-strings mirror_client.o Socket.o -o client

server: mirror_server.o Socket.o
	g++ mirror_server.o Socket.o -o server
