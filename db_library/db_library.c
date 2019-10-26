#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "./database.h"


void threadInitDataBase( struct Arguments * arguments )
{ 
    
    if( access( arguments->fileName , F_OK ) != -1 ) 
    {
        FILE * file = fopen("thisFile", "rb");
        if (file != NULL) 
        {
            fwrite( arguments->local_db, 
                    sizeof( struct DataBase ), 
                    1, 
                    file );
            fclose(file);
        }

    } 
    else 
    {
        FILE * file = fopen("thisFile", "br");
        if (file != NULL) 
        {
            fwrite( arguments->local_db, 
                    sizeof( struct DataBase ), 
                    1, 
                    file );
            fclose(file);
        } 
    }
    pthread_exit( NULL );
};

int InitDataBase( char * fileName, struct DataBase * local_db )
{
    struct Arguments arguments;

    arguments.fileName = fileName;
    arguments.local_db = local_db;

    pthread_mutex_lock( &mutexFileOperations );
    
    int threadResult;
    threadResult = pthread_create(&mutexFileOperations, 
                                   NULL, 
                                   threadInitDataBase, 
                                   &arguments );
    if( threadResult )
     {
         fprintf(stderr,"Error - pthread_create() return code: %d\n",threadResult);
         exit(EXIT_FAILURE);
     }
    
    pthread_mutex_unlock( &mutexFileOperations );
}; 

int threadReadDataBase( struct Arguments arguments )
{
    
};

int readDataBase( struct DataBase * local_db )
{
    pthread_mutex_lock( &mutexFileOperations );

    pthread_mutex_unlock( &mutexFileOperations );
};

int threadChangeElementDataBase( struct Arguments arguments )
{

};

int changeElementDataBase( struct Record * record )
{
    pthread_mutex_lock( &mutexFileOperations );

    int threadResult;
    threadResult = pthread_create((&mutexFileOperations, 
                                   NULL, 
                                   threadChangeElementDataBase, 
                                   &arguments );

    pthread_mutex_unlock( &mutexFileOperations );
};
