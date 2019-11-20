#ifndef _DB_THREADS__
#define _DB_THREADS__


#include "./db_utility.h"
#include "./db_synchronization.h"

int initMutex();
void * threadInitDataBase( void * local_db );
void * threadReadDataBase( void * local_db );
void * threadWriteDataBase( void * local_db );



#endif 
