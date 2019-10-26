#ifndef DATABASE_H_ 
#define DATABASE_H_

#include <pthread.h> // pthread

#define MAX_DATABASE_SIZE 500
#define DATA_BASE_FILE "./database"

pthread_mutex_t mutexFileOperations;
struct Arguments
{
    char * fileName;
    struct DataBase * local_db;
};

struct Record {
    unsigned int key;
    char name[100];
    float value1;
    int value2;
};

struct DataBase {
    Record record[ MAX_DATABASE_SIZE ];
};

extern void threadInitDataBase( struct Arguments arguments ); 
extern int InitDataBase( char * fileName, DataBase * local_db ); 

extern int threadReadDataBase( struct Arguments arguments );
extern int readDataBase( DataBase * local_db );

extern int threadChangeElementDataBase( struct Arguments arguments );
extern int changeElementDataBase( Record * record  );

#endif 