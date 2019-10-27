

#include "./testData.h"

Record *  makeRecord( int recordNumber ) 
{
    static Record record;

    int seed;
    time_t tt;
    seed = time(&tt);
    srand(seed);

    char name[10][100] = { "Kompot", "Wiadro", "Parapet", 
                           "Okno", "Winda", "Taki Maly dynks", 
                           "Kaloryfer", "Zegarek", "Krzeslo", "Kubek" };
      
        record.key = recordNumber;

        int randomNamePosition = rand() % sizeof( name );
        strncpy( record.name, name[ randomNamePosition ], 100);
        
        pid_t pidNumber = getpid();
        record.value1 = pidNumber;

        record.value2 = pidNumber + recordNumber;

    return &record;
};

DataBase * makeDatabase( DataBase * local_db )
{
    for( int i = 0; i< MAX_DATABASE_SIZE; i++ )
    {
        Record * record = makeRecord( i ); 
        local_db->record->key = record->key;
        strncpy( local_db->record->name, record->name, 100 );
        local_db->record->value1 = record->value1;
        local_db->record->value2 = record->value2;

    }
};

void printRecord( Record * record)
{
    printf( " | %4i | %20s | %6i | %10i | \n",
                record->key,
                record->name,
                record->value1,
                record->value2 );
};

void printDataBase( DataBase * dataBase )
{
    for( int i = 0; i<MAX_DATABASE_SIZE; i++ )
    {
        printf( " | %4i | %20s | %6i | %10i | \n",
                dataBase->record[ i ].key,
                dataBase->record[ i ].name,
                dataBase->record[ i ].value1,
                dataBase->record[ i ].value2 );
    }
};