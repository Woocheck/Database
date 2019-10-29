#ifndef _DB_THREADS__
#define _DB_THREADS__

#include "./API_db_library.h"
#include "./db_utility.h"

void * threadInitDataBase( void * local_db );
void * threadReadDataBase( void * local_db );
void * threadWriteDataBase( void * local_db );


#endif 
