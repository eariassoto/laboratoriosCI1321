#pragma once
class Socket{

public:
  Socket();
  Socket(char, bool);
  Socket(int);
  ~Socket();
  void Close();
  int Connect(char*, int);
  int Read(char*, int);
  int Write(char*, int);
  int Shutdown(int);
  int Listen(int);
  int Bind(int);
  Socket * Accept();

private:

  int descriptor, domain;

};
