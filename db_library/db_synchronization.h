#ifndef _DB_CYNCH_ 
#define _DB_SYNCH_

#define BUFOR_SIZE 15



#include <time.h>
#include "./database.h"

typedef struct singleChange 
{
    int key;
    int time;

} SingleChange;

typedef struct bufor
{
    SingleChange chngeTab[ BUFOR_SIZE ];
    int head;
    int tail;
} Bufor;


typedef struct sharedMemoryStruct
{
    Bufor cyclicalBuffer;
    Record dbMap[ MAX_DATABASE_SIZE ];
} SharedMemoryStruct;

SharedMemoryStruct * initBuffer();
void writeDataToBuffer( DataBase * local_db,  Record * record );
void readDatafromBuffer( DataBase * local_db );

#endif