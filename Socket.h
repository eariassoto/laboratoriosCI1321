#pragma once
#include <string>

using namespace std;

class Socket{

public:
  Socket();
  Socket(char, bool);
  Socket(int);
  ~Socket();
  void Close();
  int Connect(char*, int);
  int Connect(int, char*);
  int Read(char*, int);
  int Write(char*, int);
  int Write(string);
  int Shutdown(int);
  int Listen(int);
  int Bind(int);
  Socket * Accept();

private:

  int descriptor, domain;
  int HostnameToIP(char*, char*);

};
