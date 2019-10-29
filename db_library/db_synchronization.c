#include <stdio.h>
#include <stdlib.h>    // EXIT_SUCCESS
#include <sys/mman.h>  // PROT_*, MAP_SHARED 
#include <fcntl.h>     // O_* constants
#include <unistd.h>    // ftruncate, close
#include <sys/wait.h>  // wait
#include <string.h>    // strcpy
#include <semaphore.h>
#include <time.h>
#include <sys/stat.h>
#include <signal.h>


#include "./database.h"
#include "./db_synchronization.h"

int getNewPositionInBuffer( SharedMemoryStruct * buffer )
{
    int head = buffer->cyclicalBuffer.head;
    head ++;
    if( head >= BUFOR_SIZE)
        head = 0;
    
    int tail = buffer->cyclicalBuffer.tail;
    tail = head + 1;
    if( tail >= BUFOR_SIZE )
        tail = 0;
    return head;
}

SharedMemoryStruct * initBuffer()
{
    SharedMemoryStruct * shm_ptr   = NULL;

    int  shm_fd = shm_open( "/buffer_shm", O_CREAT | O_EXCL | O_RDWR, 0600 );
    ftruncate( shm_fd, sizeof( SharedMemoryStruct ) );

	shm_ptr = (SharedMemoryStruct * ) mmap( NULL , 	              
				  sizeof( SharedMemoryStruct ),      
				  PROT_READ | PROT_WRITE, 
				  MAP_SHARED,             
				  shm_fd, 
				  0);

    return shm_ptr;
};

void writeRecordToBuffer( DataBase * local_db,  Record * record )
{
    SharedMemoryStruct * sharedBuffer = initBuffer();
    
    int timeStamp = time( NULL );
    int recordPosition = getNewPositionInBuffer( sharedBuffer );  

    sharedBuffer->cyclicalBuffer.chngeTab[ recordPosition ].key = record->key;
    sharedBuffer->cyclicalBuffer.chngeTab[ recordPosition ].time = timeStamp;

    changeRecord( &(sharedBuffer->dbMap[ record->key ]), record );

};

Record * readRecordFromBuffer( SharedMemoryStruct * sharedBffer, int positionInBuffer )
{
    Record * result = &(sharedBffer->dbMap[ positionInBuffer ]);
    return result;
}
void readRecordsChangesFromBuffer( DataBase * local_db )
{
    int localDatabaseTimeStamp = local_db->timeStamp;

    SharedMemoryStruct * sharedBuffer = initBuffer();
    
    int bufferTail = sharedBuffer->cyclicalBuffer.tail;
    int buforTailTimeStamp = sharedBuffer->cyclicalBuffer.chngeTab[ bufferTail ].time;
    
    if( localDatabaseTimeStamp < buforTailTimeStamp )
    {
        readDataBase( local_db );  
    }
    else
    {
        int positionInBuffer = sharedBuffer->cyclicalBuffer.head;
        int buforElementTimeStamp = sharedBuffer->cyclicalBuffer.chngeTab[ positionInBuffer ].time;
        
        while( localDatabaseTimeStamp > buforElementTimeStamp || positionInBuffer != bufferTail )
        {
            Record * recordFromBuffer = readRecordFromBuffer( sharedBuffer, positionInBuffer );
            changeElementDataBase( local_db, recordFromBuffer);
            positionInBuffer++;
            buforElementTimeStamp = sharedBuffer->cyclicalBuffer.chngeTab[ positionInBuffer ].time;
        }
        local_db->timeStamp = time( NULL );
    }
    
}