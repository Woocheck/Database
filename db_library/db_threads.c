#include <sys/file.h>

#include "./db_threads.h"

pthread_mutex_t mutexFileOperations;

int initMutex()
{
    if (pthread_mutex_init( &mutexFileOperations, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    return 0;
}

void * threadInitDataBase( void * local_db )
{ 
    pthread_mutex_lock( &mutexFileOperations );

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
    pthread_mutex_lock( &mutexFileOperations );

        readFileDatabase( (DataBase*) local_db );

    pthread_mutex_unlock( &mutexFileOperations );
    pthread_exit( NULL );
}

void * threadWriteDataBase( void * local_db )
{
    pthread_mutex_lock( &mutexFileOperations );

        writeFileDatabase( (DataBase*) local_db );

        sendInfoAboutChanges( (DataBase*) local_db );   

    pthread_mutex_unlock( &mutexFileOperations );
    pthread_exit( NULL );
}