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

typedef struct record {
    unsigned int key;
    char name[100];
    float value1;
    int value2;
} Record;

typedef struct database {
    Record record[ MAX_DATABASE_SIZE ];
} DataBase;

extern void threadInitDataBase( DataBase * local_db ); 
extern int InitDataBase( DataBase * local_db ); 

extern int threadReadDataBase( DataBase * local_db );
extern int readDataBase( DataBase * local_db );

extern int writeReadDataBase( DataBase * local_db );
extern int writeDataBase( DataBase * local_db );

extern int threadChangeElementDataBase( DataBase * local_db, 
                                        Record * record );
extern int changeElementDataBase( DataBase * local_db, 
                                  Record * record  );
                                  

#endif 