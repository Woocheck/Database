#include <sys/file.h>

#include "./db_threads.h"
#include "./db_utility.h"
#include "./API_db_library.h"

pthread_mutex_t mutexFileOperations;

void * threadInitDataBase( void * local_db )
{ 
    pthread_mutex_trylock( &mutexFileOperations );

        if( access( DATA_BASE_FILE, F_OK ) != -1 ) 
        {
            printf("exist\n\n");
            readFileDatabase( (DataBase*)local_db);
        } 
        else 
        {
            printf("init new\n\n");
            writeFileDatabase( (DataBase*) local_db); 
        }

    pthread_mutex_unlock( &mutexFileOperations );
    pthread_exit( NULL );
}

void * threadReadDataBase( void * local_db )
{
    pthread_mutex_trylock( &mutexFileOperations );

        readFileDatabase( (DataBase*) local_db );

    pthread_mutex_unlock( &mutexFileOperations );
    pthread_exit( NULL );
}

void * threadWriteDataBase( void * local_db )
{
    pthread_mutex_trylock( &mutexFileOperations );

        writeFileDatabase( (DataBase*) local_db );

        sendInfoAboutChanges( (DataBase*) local_db );   

    pthread_mutex_unlock( &mutexFileOperations );
    pthread_exit( NULL );
}