#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "./database.h"

int writeFileDatabase( DataBase * local_db )
{
    FILE * file = fopen( DATA_BASE_FILE, "bw" );
    if (file != NULL) 
    {        
        fwrite( local_db, sizeof( DataBase ), 1, file );
        fclose(file);
    }
}

int readFileDatabase( DataBase * local_db )
{
    FILE * file = fopen( DATA_BASE_FILE, "br" );
    if (file != NULL) 
    {        
        fwrite( local_db, sizeof( DataBase ), 1, file );
        fclose(file);
    }
}

void errorInfo( char * elementName, int* threadResult )
{
    fprintf(stderr,"Error - create() in %s - code: %d\n", elementName,threadResult);
    exit(EXIT_FAILURE);
};



void threadInitDataBase( DataBase * local_db )
{ 
    pthread_mutex_trylock( &mutexFileOperations );

        if( access( DATA_BASE_FILE, F_OK ) != -1 ) 
        {
            readFileDatabase( local_db);
        } 
        else 
        {
            writeFileDatabase( local_db); 
        }

    pthread_mutex_unlock( &mutexFileOperations );
    pthread_exit( NULL );
};

int InitDataBase( DataBase * local_db )
{
    int threadResult;
    threadResult = pthread_create(&mutexFileOperations, 
                                   NULL, 
                                   threadInitDataBase, 
                                   local_db );
    if( threadResult )
        errorInfo( "init database", threadResult );
}; 

int threadReadDataBase( DataBase * local_db )
{
    pthread_mutex_trylock( &mutexFileOperations );

        readFileDatabase( local_db );

    pthread_mutex_unlock( &mutexFileOperations );
    pthread_exit( NULL );
};

int readDataBase( DataBase * local_db )
{
     int threadResult;
    threadResult = pthread_create(&mutexFileOperations, 
                                   NULL, 
                                   threadChangeElementDataBase, 
                                   local_db );
    if( threadResult )
        errorInfo( "read database", threadResult );
};

int writeReadDataBase( DataBase * local_db )
{
    pthread_mutex_trylock( &mutexFileOperations );

        writeFileDatabase( local_db );

    pthread_mutex_unlock( &mutexFileOperations );
    pthread_exit( NULL );
};

int writeDataBase( DataBase * local_db )
{
    int threadResult;
    threadResult = pthread_create(&mutexFileOperations, 
                                   NULL, 
                                   threadChangeElementDataBase, 
                                   local_db );
    if( threadResult )
        errorInfo( "write database", threadResult );
};

int threadChangeElementDataBase( DataBase * local_db, 
                                 Record * record )
{
    pthread_mutex_trylock( &mutexFileOperations );
        
    pthread_mutex_unlock( &mutexFileOperations );
};

int changeElementDataBase( DataBase * local_db, Record * record )
{
    int threadResult;
    threadResult = pthread_create(&mutexFileOperations, 
                                   NULL, 
                                   threadChangeElementDataBase, 
                                   record );
    if( threadResult )
        errorInfo( "change element", threadResult );
};
