#include "Socket.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc, char **argv){
  Socket s;
  char a[2048];

  if(argc == 2){
    // request http
    s.Connect( 80, argv[1] );
    s.Write(  "GET / HTTP/1.0\n\n", 16 );
    s.Read( a, 2048 );
    cout << a << endl;
  }else if(argc == 1){
    // mail
    s.Connect( 587, "farroyo.ecci.ucr.ac.cr" );
    s.Write("From: \"San Pedro\" <SanPedro@elcielo.com>");
    s.Write("To: \"Usuario Redes\" <ci1320@farroyo.ecci.ucr.ac.cr>\n");
    s.Write("Subject: \"Prueba de sockets\"" );
    s.Write("\n");
    s.Write("Hola, \n" );
    s.Write("Esto es una prueba de envio de correos a farroyo 5-set \n");
    s.Write(".\n");
    s.Read( a, 2048 );
    cout << a << endl;
  }
}
