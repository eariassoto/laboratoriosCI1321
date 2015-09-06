#include "Socket.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cstring>

using namespace std;

int main(int argc, char** argv){
  Socket s;
  char buffer[512];
  for(int i = 0; i < 512; i++){
    buffer[i] = 0;
  }
  if(argc == 2){
    string str;
    s.Connect( "127.0.0.1", atoi(argv[1]) );
    cout << "Escriba el mensaje: ";
    cin >> str;
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    s.Write( cstr, 512 );
    s.Read( buffer, 512 );
    printf( "%s\n", buffer );
    delete [] cstr;
  }else if(argc == 3){
    s.Connect( "127.0.0.1", atoi(argv[1]) );
    s.Write( argv[2], 512 );
    s.Read( buffer, 512 );
    printf( "%s\n", buffer );
  }

}
