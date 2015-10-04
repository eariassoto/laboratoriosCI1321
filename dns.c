/* Compile with -lresolv */

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <string.h>

#define MYMAX 1024


/*
   DNS formats

Query
  ------------------------
 | Header | Query Section |
 |  (12)  |     (?)       | <- Size
  ------------------------   (bytes)


  Query Header
  ---------------------------------------------------------------------
 | Trans ID | Parameters | # Questions | # Answers | # Auth | # of Add |
 |   (2)    |     (2)    |     (2)     |    (2)    |    (2) |   (2)    |<- Size
  ---------------------------------------------------------------------  (bytes)
   Trans ID: 16 bit unique identifier

   Parameters:
     --------------------------------------------------------------------
    |  QR | Op Code | AA  | TC  | RD  | RA  | res1 | res2 | res3 | RCode | 
    | (1) |   (4)   | (1) | (1) | (1) | (1) | (1)  | (1)  | (1)  |  (4)  |<- Size
     --------------------------------------------------------------------  (bits)
     QR: Set to 0 by the questioner, 1 by the responder
     Op Code: 0 -> Standard Query
              1 -> IQuery (Inversed Query)
              2 -> Status, DNS status request
     AA: Authoritative Answer, valid in responses only
     TC: TrunCation, the message was truncated due to length greater than that permitted on the transmission channel
     RD: Recursion Desired
     RA: Recursion Available
     res i: Reserved bits
     RCode: 0 -> No error condition
            1 -> Format error
            2 -> Server failure
            3 -> Name error
            4 -> Not implemented
            5 -> Refused
  
  
  Query Section
  ----------------------------------------------
 | Query Domain Name | Query Type | Query Class |
 |        (?)        |     (2)    |    (2)      |<- Size
  ----------------------------------------------  (bytes)

      Query Domain Name: see note about string format below
      Query Type: 1   -> Requests the A record for the domain name
                  2   -> Requests the NS record(s) for the domain name
                  5   -> Requests the CNAME record(s) for the domain name
                  6   -> Requests the SOA record(s) for the domain name
                  11  -> Requests the WKS record(s) for the domain name
                  12  -> Requests the PTR record(s) for the domain name
                  15  -> Requests the MX record(s) for the domain name
                  33  -> Requests the SRV record(s) for the domain name
                  38  -> Requests the A6 record(s) for the domain name
                  255 -> Requests ANY resource record
      Query Class: 1   -> IN or Internet



Answer
  -------------------------------------------------------
 | Header  | Question |  Answer | Authority | Additional |
 | Section | Section  | Section |  Section  |  Section   |
  -------------------------------------------------------
      Answer, Authority and Additional sections are optional, they depend on header's counters

  Header
 0  Byte    2            4             6           8       10
  ---------------------------------------------------------------------
 | Trans ID | Parameters | # Questions | # Answers | # Auth | # of Add |
 |   (2)    |     (2)    |     (2)     |    (2)    |    (2) |   (2)    |<- Size
  ---------------------------------------------------------------------  (bytes)

 Question Section
  ------------------------------------------------------------------
 | Query Domain Name | Query Type | Query Class | Query Domain Name |
 |    (string)       |     (2)    |    (2)      |        (2)*       |<- Size
  ------------------------------------------------------------------  (bytes)
* - This is an indirect pointer string,  these two bytes are conformed by
       - First byte is 192 (192 is an indirect string indicator)
       - Second byte is the starting address of the referenced string
       
      Query Domain Name: A string representing the required domain, see string format below
      Query Type: see description above
      Query Class: see description above

 Answer
  ---------------------------------------------------------------
 | Answers Section | Authority Name servers | Additional Records |
 |       (?)       |         (?)            |         (?)        |<- Size
  ---------------------------------------------------------------  (bytes)

  ------------------------------------
 |          Answers Section           |
 |------------------------------------
 | QrT | QrC | TTL | Len | Pri | Name |
 | (2) | (2) | (4) | (2) | (2) |  (?) | <- Size
  ------------------------------------     (bytes)
   QrT -> Query Type
   QrC -> Query Class
   TTL -> Time-to-Live
   Len -> Data Length
   Pri -> In MX records represents the priority
   Name -> String

  ------------------------------------
 |     Authoritative Name Servers     |
 |------------------------------------
 | Same format as "Answer Section"    |
  ------------------------------------ 

  ------------------------------------
 |        Additional Records         |
 |------------------------------------
 | Same format as "Answer Section"    |
  ------------------------------------ 


Note:
   The strings are represented using this format:
  -------------------------------------   -------------------------------------
 |  len1 | First | Second | ... | Last | |  len2 | First | Second | ... | Last |
 |  (1)  | char  |  char  | ... | char | |  (1)  | char  |  char  | ... | char |
  -------------------------------------   -------------------------------------
     0       1       2            len1+1     0       1        2          len2+1   <-- Position (byte)

   String ends when a length of zero is found

*/


unsigned char buff[ MYMAX ];


int getAnswerRecordCount() {

   return ( ( buff[6] << 8 ) + buff[7] );

}


int getAuthRecordCount() {

   return ( ( buff[8] << 8 ) + buff[9] );

}


int getAdditionalRecordCount() {

   return ( ( buff[10] << 8 ) + buff[11] );

}


int ReadHeader() {

   int a, j, k, l, res, qt, qc;
   char domain[ MYMAX ];

   k = 0;
   domain[0] = 0;
   a = 0;

   res = ( buff[0] << 8 ) + buff[1];
   printf( "[ id ] = %d\n", res );
   res = ( buff[2] << 8 ) + buff[3];
   printf( "[ flags ] = 0x%X\n", res );
   res = ( buff[4] << 8 ) + buff[5];
   printf( "[ Question count ] = %d\n", res );
   res = ( buff[6] << 8 ) + buff[7];
   printf( "[ Answer Record count ] = %d\n", res );
   res = ( buff[8] << 8 ) + buff[9];
   printf( "[ Name Server Record count ] = %d\n", res );
   res = ( buff[10] << 8 ) + buff[11];
   printf( "[ Additional Record count ] = %d\n", res );

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

   printf( ";;Question section:\n;%s\t\t\t%s\t%s\n", domain, (qc==1)?"IN":"OT", (qt==1)?"A":"MX" );

   a += 4;

   return( a );

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


int ReadRecord( int pos ) {
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

   switch ( qt ) {
      case 1:		// An A record
         sprintf( name, "%d.%d.%d.%d", buff[ pos ], buff[ pos + 1 ], buff[ pos + 2 ], buff[ pos + 3 ]);
         pos += 4;
         printf( "%s\t\t%d\t%s\t%s\t%s\n", domain, ttl, (qc==1)?"IN":"OT", "A", name);
         break;

      case 2:		// A NS record
         pos = ReadName( buff, name, pos );
         printf( "%s\t\t%d\t%s\t%s\t%s\n", domain, ttl, (qc==1)?"IN":"OT", "NS", name);
         break;

      case 15:		// A MX record
         pri = ( buff[ pos ] << 8 ) + buff[ pos + 1 ];
         pos += 2;
         pos = ReadName( buff, name, pos );

         printf( "%s\t\t%d\t%s\t%s\t%d %s\n", domain, ttl, (qc==1)?"IN":"OT", "MX", pri, name);
         break;
	 
      default:		// For now, skip all the other record types
	 pos += len;
	 break;
   }

   return( pos );

}



int main( int argc, char ** argv ) {

   char domain[ MYMAX ];
   int len = 0;
   unsigned int res;
   int a, i, j, k, l, n, t;

   res_init();

   len = res_query( argv[1], C_IN, T_MX, buff, MYMAX );

   printf( "MX records for gmail.com len =%d\n", len );

   a = ReadHeader();

   if ( getAnswerRecordCount() > 0 ) {
      printf("\n;; Answer section:\n");
      for ( i = 0; i < getAnswerRecordCount(); i++ ) {
         a = ReadRecord( a );
      }
   }

   if ( getAuthRecordCount() > 0 ) {
      printf("\n;; Authority section:\n");
      for ( i = 0; i < getAuthRecordCount(); i++ ) {
         a = ReadRecord( a );
      }
   }

   if ( getAdditionalRecordCount() > 0 ) {
      printf("\n;; Additional section:\n");
      for ( i = 0; i < getAdditionalRecordCount(); i++ ) {
         a = ReadRecord( a );
      }
   }

/*
   for( a=0; a < len; a++ ) {
      printf( "[ %d ] = %d, %c\n", a, buff[ a ], buff[ a ] );
   }

   for( ; a < len; a++ ) {
      printf( "buff[ %d ] = %d, %c\n", a, buff[ a ], buff[ a ] );
   }
*/   

}

