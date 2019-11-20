#ifndef _DB_SYNCH_ 
#define _DB_SYNCH_

#define BUFOR_SIZE 15
#define MAX_NUMBER_PROCESSES 20
#include <stdio.h>
#include <stdlib.h>    
#include <sys/mman.h>   
#include <fcntl.h>     
#include <unistd.h>    
#include <sys/wait.h>  
#include <string.h>
#include <semaphore.h>
#include <time.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#include "./API_db_library.h"

typedef struct singleChange 
{
    int key;
    int time;

} SingleChange;

typedef struct bufor
{
    SingleChange chngeTab[ BUFOR_SIZE ];
    int          head;
    int          tail;
} Bufor;

typedef struct sharedMemoryStruct
{
    sem_t sharedMemorySemaphore;
    Bufor   cyclicalBuffer;
    Record  dbMap[ MAX_DATABASE_SIZE ];
    int subscriptionList[ MAX_NUMBER_PROCESSES ];
} SharedMemoryStruct;

void initSubscriptionList();
SharedMemoryStruct * initBuffer();
void writeDataToBuffer( DataBase * local_db,  Record * record );
void readDatafromBuffer( DataBase * local_db );
void sendInfoAboutChanges( DataBase * local_db );
void readRecordsChangesFromBuffer( DataBase * local_db );

#endif