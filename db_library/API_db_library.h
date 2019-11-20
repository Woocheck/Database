#ifndef DATABASE_H_ 
#define DATABASE_H_

#include <pthread.h> 

#include "./db_changelist.h"

#define MAX_DATABASE_SIZE      10
#define MAX_DB_NAME_FIELD_SIZE 20
#define DATA_BASE_FILE         "./database.db"

typedef struct record {
    unsigned int key;
    char         name[ MAX_DB_NAME_FIELD_SIZE ];
    int          value1;
    int          value2;
} Record;

typedef struct database {
    Record *    record;
    size_t      size;
    int         timeStamp;
    List   *    listOfChanges;
    int isOwnSignal;
} DataBase;

extern DataBase * newDataBase( size_t size );
extern void InitDataBase( DataBase * local_db ); 
extern List * newListDBChanges();

extern void readDataBase( DataBase * local_db );
extern void writeDataBase( DataBase * local_db );
extern void changeElementDataBase( DataBase * local_db, 
                                   Record * record  );
extern void upgradeLocalDatabase( DataBase * local_db );

extern void deleteDataBase( DataBase * local_db );



#endif 