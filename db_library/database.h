#ifndef DATABASE_H_ 
#define DATABASE_H_

#include <pthread.h> 

#define MAX_DATABASE_SIZE 5
#define MAX_DB_NAME_FIELD_SIZE 20
#define DATA_BASE_FILE "/home/user/Workspace_database/Database/database.db"

typedef struct record {
    unsigned int key;
    char name[ MAX_DB_NAME_FIELD_SIZE ];
    int value1;
    int value2;
} Record;

typedef struct database {
    Record * record;
    size_t size;
} DataBase;

struct Arguments
{
    char * fileName;
    DataBase * local_db;
};

extern DataBase * newDataBase( size_t size );
extern void deleteDataBase( DataBase * local_db );

extern void copyDataBase( Record * dest[], DataBase * src, int direction );
extern void writeFileDatabase( DataBase * local_db );
extern void readFileDatabase( DataBase * local_db );

extern void * threadInitDataBase( void * local_db ); 
extern void InitDataBase( DataBase * local_db ); 

extern void * threadReadDataBase( void * local_db );
extern void readDataBase( DataBase * local_db );

extern void * threadWriteDataBase( void * local_db );
extern void writeDataBase( DataBase * local_db );

extern void threadChangeElementDataBase( DataBase * local_db, 
                                        Record * record );
extern void changeElementDataBase( DataBase * local_db, 
                                  Record * record  );
                                  

#endif 