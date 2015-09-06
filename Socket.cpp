
#include "Socket.h"
#include <arpa/inet.h> //htons
#include <sys/types.h> //tipos const
#include <sys/socket.h> //socket
#include <iostream> //cout
#include <unistd.h> //close
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <cstring>

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

int Socket::Connect(int Port, char * Hostname)
{
  char Host[100];
  HostnameToIP(Hostname, Host);

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

int Socket::Write(string text)
{
  int len = text.size();
  char *cstr = new char[len + 1];
  strcpy(cstr, text.c_str());
  int w = write(descriptor, cstr, len);
  delete [] cstr;
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


int Socket::HostnameToIP(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ( (he = gethostbyname( hostname ) ) == NULL)
    {
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++)
    {
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }

    return 1;
}
