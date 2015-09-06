#include "Socket.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv){
  int childpid;

  Socket s1, *s2;

   s1.Bind( atoi( argv[1] ) );
   s1.Listen( 5 );

   for( ; ; ) {
     // espera a tener una solicitud
     s2 = s1.Accept();

     childpid = fork();
     if ( childpid < 0 )
         perror("server: fork error");
     else if (childpid == 0) {
       // proceso hijo
       char buf[512];
       for(int i = 0; i < 512; i++){
         buf[i] = 0;
       }
       // cierra el server
       s1.Close();

       // lee la peticion y la escribe de nuevo al cliente
       s2->Read( buf, 512 );
       cout << buf << endl;
       s2->Write( buf, 512 );
       exit( 0 );
     }
     s2->Close();
   }
}
