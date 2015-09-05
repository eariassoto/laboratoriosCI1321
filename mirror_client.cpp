#include "Socket.h"
#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv){
  Socket s;
  char buffer[512];
  for(int i = 0; i < 512; i++){
    buffer[i] = 0;
  }
   s.Connect( "127.0.0.1", atoi(argv[1]) );
   s.Write( argv[2], 512 );
   s.Read( buffer, 512 );		
   printf( "%s\n", buffer );	
}
