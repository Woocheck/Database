
#include "./db_synchronization.h"
#include "./db_utility.h"



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
    int isShmCreated = shm_open( "/shm_buffer", O_CREAT | O_EXCL | O_RDWR, 0777 );

    int  shm_fd = shm_open( "/shm_buffer",  O_CREAT | O_RDWR, 0777 );
    
    ftruncate( shm_fd, sizeof( SharedMemoryStruct ) );
	shm_ptr = (SharedMemoryStruct * ) mmap( NULL , 	              
				  sizeof( SharedMemoryStruct ),      
				  PROT_READ | PROT_WRITE, 
				  MAP_SHARED,             
				  shm_fd, 
				  0);
    if ( isShmCreated > -1 )
    {
        sem_init( &(shm_ptr->sharedMemorySemaphore), 1, 1 );
        printf( " is not created shm \n");
    }
    return shm_ptr;
}

int getEmptyPosition( SharedMemoryStruct * sharedBuffer )
{
    int emptyPosition = 0;
    int listElement = 0;

    while( sharedBuffer->subscriptionList[ listElement ] != 0 &&
           listElement < MAX_NUMBER_PROCESSES )
    {
        listElement ++;
    }
    
    if( listElement < MAX_NUMBER_PROCESSES )
    {
        emptyPosition = listElement;
    }
    else
    {
        printf("There in no place on subscription list. \n");
        exit( 0 ); 
    }
    return emptyPosition;
}

int isProcessOnSubscriptionList( SharedMemoryStruct * sharedBuffer, int currentProcessPid )
{
    for( int x =0; x< MAX_NUMBER_PROCESSES; x++ )
    {
        if( sharedBuffer->subscriptionList[ x ] == currentProcessPid )
            return 1;
    }
    return 0;
}

void addToTheSubscriptionList( SharedMemoryStruct * sharedBuffer )
{
    int currentProcessPid = getpid();
    printf(" 1. \n");
    if( !isProcessOnSubscriptionList( sharedBuffer, currentProcessPid ) )
    {
        int emptyPosition = getEmptyPosition( sharedBuffer );
        printf(" empty position: %d \n", emptyPosition );
        sharedBuffer->subscriptionList[ emptyPosition ] = currentProcessPid;
    
    }
}

void initSubscriptionList()
{
    SharedMemoryStruct * sharedBuffer = initBuffer();
    printf("is 1. \n");
    sem_wait( &(sharedBuffer->sharedMemorySemaphore) );
    printf("is 2. \n");
        addToTheSubscriptionList( sharedBuffer );
    
    sem_post( &(sharedBuffer->sharedMemorySemaphore) );
}

void writeRecordToBuffer( DataBase * local_db,  Record * record )
{
    SharedMemoryStruct * sharedBuffer = initBuffer();
    
    sem_wait( &(sharedBuffer->sharedMemorySemaphore) );

    int timeStamp = time( NULL );
    int recordPosition = getNewPositionInBuffer( sharedBuffer );

    sharedBuffer->cyclicalBuffer.chngeTab[ recordPosition ].key = record->key;
    sharedBuffer->cyclicalBuffer.chngeTab[ recordPosition ].time = timeStamp;

    changeRecord( &(sharedBuffer->dbMap[ record->key ]), record );
    sem_post( &(sharedBuffer->sharedMemorySemaphore) );
}

Record * readRecordFromBuffer( SharedMemoryStruct * sharedBffer, int positionInBuffer )
{
    Record * result = &(sharedBffer->dbMap[ positionInBuffer ]);
    return result;
}

void readRecordsChangesFromBuffer( DataBase * local_db )
{
    printf("Signal from other proces\n");
    
    int localDatabaseTimeStamp = local_db->timeStamp;
    SharedMemoryStruct * sharedBuffer = initBuffer();
    
    sem_wait( &(sharedBuffer->sharedMemorySemaphore) );
    
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
        
        while( localDatabaseTimeStamp > buforElementTimeStamp && positionInBuffer != bufferTail )
        {
            Record * recordFromBuffer = readRecordFromBuffer( sharedBuffer, positionInBuffer );
            changeElementDataBase( local_db, recordFromBuffer);
            positionInBuffer++;
            buforElementTimeStamp = sharedBuffer->cyclicalBuffer.chngeTab[ positionInBuffer ].time;
        }
        local_db->timeStamp = time( NULL );
    }
    sem_post( &(sharedBuffer->sharedMemorySemaphore) );
    
}

void sendInfoAboutChanges( DataBase * local_db )
{
    while( local_db->listOfChanges->head != NULL )
    {
        int recordKey = pop( &(local_db->listOfChanges) );
        Record * elementToWrite =  &((local_db->record)[ recordKey ]);
        writeRecordToBuffer( local_db, elementToWrite );
    }
    
    
    SharedMemoryStruct * sharedBuffer = initBuffer();
    int currentPID = getpid();

    sem_wait( &(sharedBuffer->sharedMemorySemaphore) );
    
        for(int x = 0; x < MAX_NUMBER_PROCESSES; x++ )
        {
            int calledPID = sharedBuffer->subscriptionList[ x ];
            if( calledPID != currentPID ){printf("pid sa inne\n");
                kill( calledPID, SIGUSR1 );}
        }
    
    sem_post( &(sharedBuffer->sharedMemorySemaphore) );
}