#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <time.h>

#include "./database.h"

pthread_mutex_t mutexFileOperations;

extern DataBase * newDataBase( size_t size )
{
    DataBase * newLocalDatabase = ( DataBase * ) malloc (sizeof ( DataBase ) );
    if ( newLocalDatabase == NULL)
        return NULL;

    newLocalDatabase->record =  ( Record * ) malloc ( size * sizeof ( Record ) );
    if ( newLocalDatabase->record == NULL ) 
    {
        free ( newLocalDatabase );
        return NULL;
    }

    newLocalDatabase->size = size;
    newLocalDatabase->timeStamp = time( NULL );
    
    return newLocalDatabase;
};

void deleteDataBase( DataBase * local_db )
{
    if( local_db != NULL )
    {
        free( local_db->record );
        free( local_db );
    }
};

extern void copyData(  int direction, Record * file, DataBase * local_db )
{
    for( int i = 0; i< MAX_DATABASE_SIZE; i++ )
    {
        if( direction == 1)
        {
            file[i].key = (local_db->record)[i].key;
            strcpy( file[i].name, (local_db->record)[i].name );
            file[i].value1 = (local_db->record)[i].value1;
            file[i].value2 = (local_db->record)[i].value2;
        }
        else
        {
            (local_db->record)[i].key = file[i].key;
            strcpy( (local_db->record)[i].name, file[i].name );
            (local_db->record)[i].value1 = file[i].value1;
            (local_db->record)[i].value2 = file[i].value2;
        }
        
    }
};

int lock( FILE * file)
{
    if( flock( fileno( file ), LOCK_EX ) < 0 )
    {
        printf("File locking failed.");
        return -1;
    }
    return 0;
}

void writeFileDatabase( DataBase * local_db )
{
    
    
    FILE * file = fopen( DATA_BASE_FILE, "wb" );
    if (file != NULL) 
    {     
        Record tempDataBase[ MAX_DATABASE_SIZE ];
        int toFile = 1;
        copyData( toFile, tempDataBase, local_db );
        
        lock( file );
        fwrite( tempDataBase, sizeof( Record ), MAX_DATABASE_SIZE, file );
        fclose(file);
    }
}

void readFileDatabase( DataBase * local_db )
{
    FILE * file = fopen( DATA_BASE_FILE, "rb" );
    if (file != NULL) 
    {   
        Record tempDataBase[ MAX_DATABASE_SIZE ];     
        
        lock( file );
        fread( tempDataBase, sizeof( Record ), MAX_DATABASE_SIZE, file );
        fclose(file);
        
        int toDatabase = 0;
        copyData( toDatabase, tempDataBase, local_db);
        local_db->timeStamp = time( NULL );
    }
}

void errorInfo( const char * elementName, int * threadResult )
{
    fprintf(stderr,"Error - create() in %s - code: %d\n", elementName,threadResult);
    exit(EXIT_FAILURE);
}



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
};

void InitDataBase( DataBase * local_db )
{
    pthread_t threadDataBaseInit;
    int threadResult;
    threadResult = pthread_create(&threadDataBaseInit, 
                                   NULL, 
                                   threadInitDataBase, 
                                   local_db );
    if( threadResult )
        errorInfo( "init database", &threadResult );
}; 

void * threadReadDataBase( void * local_db )
{
    pthread_mutex_trylock( &mutexFileOperations );

        readFileDatabase( (DataBase*) local_db );

    pthread_mutex_unlock( &mutexFileOperations );
    pthread_exit( NULL );
};

void readDataBase( DataBase * local_db )
{
    pthread_t threadDataBaseRead;
    int threadResult;
    threadResult = pthread_create(&threadDataBaseRead, 
                                   NULL, 
                                   threadReadDataBase, 
                                   local_db );
    if( threadResult )
        errorInfo( "read database", &threadResult );
};

void * threadWriteDataBase( void * local_db )
{
    pthread_mutex_trylock( &mutexFileOperations );

        writeFileDatabase( (DataBase*) local_db );

    pthread_mutex_unlock( &mutexFileOperations );
    pthread_exit( NULL );
};

void writeDataBase( DataBase * local_db )
{
    pthread_t threadDataBaseWrite;
    int threadResult;
    threadResult = pthread_create(&threadDataBaseWrite, 
                                   NULL, 
                                   threadWriteDataBase, 
                                   local_db );
    if( threadResult )
        errorInfo( "write database", &threadResult );
};

void changeRecord( Record * oldRecord , Record * newRecord )
{
    oldRecord->key = newRecord->key;
    strcpy( oldRecord->name, newRecord->name );
    oldRecord->value1 = newRecord->value1;
    oldRecord->value2 = newRecord->value2;
};

void changeElementDataBase( DataBase * local_db, Record * newRecord )
{
    Record * recordToChange = &(local_db->record)[newRecord->key];
    changeRecord(  recordToChange, newRecord);
};
