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
    s.Connect( 25, "farroyo.ecci.ucr.ac.cr" );

    cout << "HELO elcielo.com" << endl;
    s.Write("HELO elcielo.com\n");
    s.Read( a, 2048 );
    cout << a << endl;
    for(int i = 0; i<2048; i++)
      a[i] = 0;

    cout << "MAIL FROM: <sanpedro@elcielo.com>" << endl;
    s.Write("MAIL FROM: <sanpedro@elcielo.com>\n");
    s.Read( a, 2048 );
    cout << a << endl;
    for(int i = 0; i<2048; i++)
      a[i] = 0;

    cout << "RCPT TO: <ci1320@farroyo.ecci.ucr.ac.cr>" << endl;
    s.Write("RCPT TO: <ci1320@farroyo.ecci.ucr.ac.cr>\n");
    s.Read( a, 2048 );
    cout << a << endl;
    for(int i = 0; i<2048; i++)
      a[i] = 0;

    cout << "DATA" << endl;
    s.Write("DATA\n");
    s.Read( a, 2048 );
    cout << a << endl;
    for(int i = 0; i<2048; i++)
      a[i] = 0;

    string str = "From: \"San Pedro\" <SanPedro@elcielo.com>\nTo: \"Usuario Redes\" <ci1320@farroyo.ecci.ucr.ac.cr>\nSubject: \"yolo swag2\"\n\nHola, \nEsto es una prueba de envio de correos a farroyo 9-set \n.\n";
    cout << str;
    s.Write(str);
    s.Read( a, 2048 );

  }
}
