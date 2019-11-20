#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include "./API_db_library.h"
#include "./db_threads.h"

extern void initSynchronisation()
{
    initSubscriptionList();
}
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
    newLocalDatabase->listOfChanges = newListDBChanges();
    newLocalDatabase->isOwnSignal = 0;
    return newLocalDatabase;
}

void deleteDataBase( DataBase * local_db )
{
    if( local_db != NULL )
    {
        free( local_db->record );
        free( local_db );
    }
}

void InitDataBase( DataBase * local_db )
{
    initMutex();
    initSubscriptionList();
    pthread_t threadDataBaseInit;
    int threadResult;
    threadResult = pthread_create(&threadDataBaseInit, 
                                   NULL, 
                                   threadInitDataBase, 
                                   local_db );
    if( threadResult )
        errorInfo( "init database", threadResult );
} 

void readDataBase( DataBase * local_db )
{
    pthread_t threadDataBaseRead;
    int threadResult;
    threadResult = pthread_create(&threadDataBaseRead, 
                                   NULL, 
                                   threadReadDataBase, 
                                   local_db );
    if( threadResult )
        errorInfo( "read database", threadResult );
}

void writeDataBase( DataBase * local_db )
{
    pthread_t threadDataBaseWrite;
    int threadResult;
    threadResult = pthread_create(&threadDataBaseWrite, 
                                   NULL, 
                                   threadWriteDataBase, 
                                   local_db );
    if( threadResult )
        errorInfo( "write database", threadResult );
}

void changeElementDataBase( DataBase * local_db, Record * newRecord )
{
    Record * recordToChange = &(local_db->record)[newRecord->key];
    changeRecord(  recordToChange, newRecord);
    push( &(local_db->listOfChanges), newRecord->key );
}

List * newListDBChanges()
{
    List * newLocalList = ( List * ) malloc( sizeof ( List ) );
    if( newLocalList == NULL )
        return NULL;
    
    newLocalList->head = ( node * ) malloc( sizeof( node ) );
    if( newLocalList->head == NULL )
        return NULL;

    newLocalList->tail = ( node * ) malloc( sizeof( node ) );
    if( newLocalList->tail == NULL )
        return NULL;

    newLocalList->head = NULL;
    return newLocalList;
}

extern void upgradeLocalDatabase( DataBase * local_db )
{
    readRecordsChangesFromBuffer( local_db );
}

