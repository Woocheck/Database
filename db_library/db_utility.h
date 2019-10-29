#ifndef _DB_AUXILARY_
#define _DB_AUXILARY_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <signal.h>

#include "./API_db_library.h"

void copyToFile( Record * file, DataBase * local_db );
void copyFromFile( Record * file, DataBase * local_db );
void copyDataBase( Record * dest[], DataBase * src, int direction );

int lock( FILE * file);
void writeFileDatabase( DataBase * local_db );
void readFileDatabase( DataBase * local_db );

void errorInfo( const char * elementName, int threadResult );
void changeRecord( Record * oldRecord , Record * newRecord );
 
#endif