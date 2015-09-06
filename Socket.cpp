
#include "Socket.h"
#include <arpa/inet.h> //htons
#include <sys/types.h> //tipos const
#include <sys/socket.h> //socket
#include <iostream> //cout
#include <unistd.h> //close
#include <string.h>
#include <errno.h>

using namespace std;

Socket::Socket()
{
  descriptor = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int d)
{
  descriptor = d;
}


Socket::Socket(char type, bool ipv6 = false)
{
  int t;
  if(ipv6){
    domain = AF_INET6;
  }else{
    domain = AF_INET;
  }
  if(type == 's'){
    t = SOCK_STREAM;
  }else if(type == 'd'){
    t = SOCK_DGRAM;
  }

  descriptor = socket(domain, t, 0);
}

Socket::~Socket()
{
  close(descriptor);
}

void Socket::Close()
{
  close(descriptor);
}

int Socket::Connect(char * Host, int Port)
{
  struct sockaddr_in host_addr;
  host_addr.sin_family = AF_INET;
  inet_aton( Host,  &host_addr.sin_addr );
  host_addr.sin_port = htons( Port );
  socklen_t len = sizeof( host_addr );
  return connect( descriptor, (sockaddr *) & host_addr, len );
}

int Socket::Read(char * text, int len)
{
	return read(descriptor, text, len);
}

int Socket::Write(char * text, int len)
{
  int w = write(descriptor, text, len);
	return w;
}

int Socket::Shutdown(int how)
{
	return shutdown(descriptor, how);
}

int Socket::Listen(int Queue)
{
  return listen(descriptor, Queue);
}

int Socket::Bind(int Port)
{
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
  server_addr.sin_port = htons( Port );
  int len = sizeof( server_addr );
  return bind( descriptor, (const sockaddr *) & server_addr, len );
}

Socket * Socket::Accept()
{
  Socket *s = 0;
  struct sockaddr_in peer_addr;
  socklen_t peer_addr_size = sizeof(struct sockaddr_in);
  int a = accept(descriptor, (struct sockaddr *) &peer_addr, &peer_addr_size);
  if(a != -1)
  {
    s = new Socket(a);
  }
  return s;
}
