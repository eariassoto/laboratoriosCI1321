Socket.o: Socket.h Socket.cpp
	g++ -c -g -Wno-write-strings Socket.cpp

mirror_client.o: Socket.h Socket.cpp mirror_client.cpp
	g++ -c -g -Wno-write-strings Socket.cpp mirror_client.cpp

mirror_server.o: Socket.h Socket.cpp mirror_server.cpp
	g++ -c -g -Wno-write-strings Socket.cpp mirror_server.cpp

lab5main.o: Socket.h Socket.cpp lab5main.cpp
	g++ -c -g -Wno-write-strings Socket.cpp lab5main.cpp

client: mirror_client.o Socket.o
	g++ mirror_client.o Socket.o -o client

server: mirror_server.o Socket.o
	g++ mirror_server.o Socket.o -o server

lab5: lab5main.o Socket.o
	g++ lab5main.o Socket.o -o lab5
