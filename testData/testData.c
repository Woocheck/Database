

#include "./testData.h"

Record *  makeTestRecord( int recordNumber ) 
{
    static Record record;

    static int isFirstTimeUsed = 1;
    if(isFirstTimeUsed)
    {
        int seed = time(NULL);
        srand(seed);
        isFirstTimeUsed = 0;
    }

    char name[10][20] = { "Kompot", "Wiadro", "Parapet", 
                           "Okno", "Winda", "Taki Maly dynks", 
                           "Kaloryfer", "Zegarek", "Krzeslo", "Kubek" };
      
        record.key = recordNumber;
        int randomNamePosition = rand() % 10;
        strncpy( record.name, name[ randomNamePosition ], sizeof( name[ randomNamePosition ] )-1 );
        int pidNumber = getpid();
        record.value1 = pidNumber;
        record.value2 = pidNumber + recordNumber;

    return &record;
}

DataBase * makeTestDatabase( DataBase * local_db )
{
    for( int i = 0; i< MAX_DATABASE_SIZE; i++ )
    {
        Record * record = makeTestRecord( i ); 
        
        (local_db->record)[i].key = record->key;
        strcpy( (local_db->record)[i].name, record->name );
        (local_db->record)[i].value1 = record->value1;
        (local_db->record)[i].value2 = record->value2;
    }
    return local_db;
}

void printRecord( Record * record)
{
    printf( " | %4i | %20s | %6i | %10i | \n",
                record->key,
                record->name,
                record->value1,
                record->value2 );
}

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
    printf("\n");
}