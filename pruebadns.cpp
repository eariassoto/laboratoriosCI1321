#include <stdio.h>
#include <stdlib.h>
#include "Socket.h"
#include <stdio.h>
#include "Socket.h"
#include <string.h>
#include <iostream>

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <vector>

#define MYMAX 1024
using namespace std;

int ReadRecord(int pos);
int getName(char * name, int pos);
int ReadName( unsigned char * buff, char * name, int pos );
int getAnswerRecordCount();
int getAuthRecordCount();
int getAdditionalRecordCount();
int ReadHeader();

// cosas globales
unsigned char buff[ MYMAX ];
vector<string> IPNamesList;
vector<string> domainList;

int main( int argc, char * argv[] )
{
  int len;
  int x;

	if(argv[1] == NULL)
	{
		 cout << "Error: Debe ingresar el nombre del host" << endl;
	}
	else
	{
    res_init();
  	len = res_query( argv[1], C_IN, T_MX, buff, MYMAX );
    x = ReadHeader();

    // agarro la seccion de answer
    if ( getAnswerRecordCount() > 0 ) {
      for (int i = 0; i < getAnswerRecordCount(); i++ ) {
         x = ReadRecord( x );
      }
   }

   // agarro la parte de authority
   if ( getAuthRecordCount() > 0 ) {
      for (int i = 0; i < getAuthRecordCount(); i++ ) {
         x = ReadRecord( x );
      }
   }

   // agarro la seccion adicional
   if ( getAdditionalRecordCount() > 0 ) {
      for (int i = 0; i < getAdditionalRecordCount(); i++ ) {
         x = ReadRecord( x );
      }
   }

	char* Hostname;
  Hostname = strcpy((char*)malloc((IPNamesList[0]).length()+1), (IPNamesList[0]).c_str());
	cout << "La IP del servidor es: " << Hostname << endl;

	}
}

int getAnswerRecordCount() {
   return ( ( buff[6] << 8 ) + buff[7] );
}


int getAuthRecordCount() {
   return ( ( buff[8] << 8 ) + buff[9] );
}

int getAdditionalRecordCount() {
   return ( ( buff[10] << 8 ) + buff[11] );
}

int getName( char * name, int pos ) {

   int j, k, l;

   k = strlen( name );

   do {
      l = buff[ pos++ ];
      if ( l > 0 ) {
          if ( l == 192 ) {
             name[ k ] = 0;
             getName( name, buff[ pos ] );
             l = 0;
             break;
          }
          for ( j = 0; j < l; j++ ) {
             name[ k++ ] = buff[ pos + j ];
          }
          name[ k++ ] = '.';
          pos += l;
      } else {
          name[ k ] = 0;
          l = 0;
      }
   } while ( l > 0 );

   return( 0 );

}

int ReadName( unsigned char * buff, char * name, int pos ) {

int j, k , l;

   k = 0;

   do {
      l = buff[ pos++ ];
      if ( l > 0 ) {
         if ( l == 192 ) {
            name[ k ] = 0;
	    getName( name, buff[ pos ] );
	    l = 0;
	    pos ++;	// Skips pointer itself
	    break;
	 }
	 for ( j = 0; j < l; j++ ) {
	     name[ k++ ] = buff[ pos + j ];
	 }
	 name[ k++ ] = '.';
	 pos += l;
      } else {
	 name[ k ] = 0;
	 l = 0;
      }
   } while ( l > 0 );

   return pos;

}

//Método que obtiene las IP
int ReadRecord(int pos)
{

    char name[ MYMAX ], domain[ MYMAX];
    int dn, len, pri, qc, qt, ttl;

   memset( name, 0, MYMAX );
   memset( domain, 0, MYMAX );

   dn = buff[ pos ];
   if ( dn == 192 ) {	// A pointer to domain name
      getName( domain, buff[pos + 1] );
      pos +=2;
   }
   qt =  ( buff[ pos ] << 8 ) + buff[ pos + 1 ];
   qc =  ( buff[ pos + 2 ] << 8 ) + buff[ pos + 3 ];
   ttl = ( buff[ pos + 4 ] << 24 ) + ( buff[ pos + 5 ] << 16 ) + ( buff[ pos + 6 ] << 8 ) + buff[ pos + 7 ];
   len = ( buff[ pos + 8 ] << 8 ) + buff[ pos + 9 ];

   pos += 10;	// Skip the 10 first bytes just processed above

	 if(qt == 1)
	 {
			sprintf( name, "%d.%d.%d.%d", buff[ pos ], buff[ pos + 1 ], buff[ pos + 2 ], buff[ pos + 3 ]);
			pos += 4;
			//printf( "%s\t\t%d\t%s\t%s\t%s\n", domain, ttl, (qc==1)?"IN":"OT", "A", name);
			IPNamesList.push_back(name);
			domainList.push_back(domain);
	 }

	 else
	 {
			pos += len;
	 }

	  return( pos );

}

int ReadHeader() {

   int a, j, k, l, res, qt, qc;
   char domain[ MYMAX ];

   k = 0;
   domain[0] = 0;
   a = 0;

   res = ( buff[0] << 8 ) + buff[1];
 //  printf( "[ id ] = %d\n", res );
   res = ( buff[2] << 8 ) + buff[3];
 //  printf( "[ flags ] = 0x%X\n", res );
   res = ( buff[4] << 8 ) + buff[5];
 //  printf( "[ Question count ] = %d\n", res );
   res = ( buff[6] << 8 ) + buff[7];
 //  printf( "[ Answer Record count ] = %d\n", res );
   res = ( buff[8] << 8 ) + buff[9];
 //  printf( "[ Name Server Record count ] = %d\n", res );
   res = ( buff[10] << 8 ) + buff[11];
 //  printf( "[ Additional Record count ] = %d\n", res );

   a = 12;

// Get the domain name in DNS string format, this is the query section
   do {
       l = buff[ a++ ];
       if ( l > 0 ) {
          for ( j = 0; j < l; j++ ) {
             domain[ k++ ] = buff[ a + j ];
          }
          domain[ k++ ] = '.';
          a += l;
       } else {
         domain[ k ] = 0;
       }
   } while ( l > 0 );

   qt = ( buff[ a ] << 8 ) + buff[ a + 1 ];
   qc = ( buff[ a + 2 ] << 8 ) + buff[ a + 3 ];

//   printf( ";;Question section:\n;%s\t\t\t%s\t%s\n", domain, (qc==1)?"IN":"OT", (qt==1)?"A":"MX" );

   a += 4;

   return( a );

}
