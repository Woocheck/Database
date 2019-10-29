
#include "./db_utility.h"


void copyData(  int direction, Record * file, DataBase * local_db )
{
    for( int i = 0; i< MAX_DATABASE_SIZE; i++ )
    {
        if( direction == 1)
        {
            file[i].key = (local_db->record)[i].key;
            strcpy( file[i].name, (local_db->record)[i].name );
            file[i].value1 = (local_db->record)[i].value1;
            file[i].value2 = (local_db->record)[i].value2;
        }
        else
        {
            (local_db->record)[i].key = file[i].key;
            strcpy( (local_db->record)[i].name, file[i].name );
            (local_db->record)[i].value1 = file[i].value1;
            (local_db->record)[i].value2 = file[i].value2;
        }
        
    }
}

int lock( FILE * file)
{
    if( flock( fileno( file ), LOCK_EX ) < 0 )
    {
        printf("File locking failed.");
        return -1;
    }
    return 0;
}

void writeFileDatabase( DataBase * local_db )
{
    FILE * file = fopen( DATA_BASE_FILE, "wb" );
    if (file != NULL) 
    {     
        Record tempDataBase[ MAX_DATABASE_SIZE ];
        int toFile = 1;
        copyData( toFile, tempDataBase, local_db );
        
        lock( file );
        fwrite( tempDataBase, sizeof( Record ), MAX_DATABASE_SIZE, file );
        fclose(file);
    }
}

void readFileDatabase( DataBase * local_db )
{
    FILE * file = fopen( DATA_BASE_FILE, "rb" );
    if (file != NULL) 
    {   
        Record tempDataBase[ MAX_DATABASE_SIZE ];     
        
        lock( file );
        fread( tempDataBase, sizeof( Record ), MAX_DATABASE_SIZE, file );
        fclose(file);
        
        int toDatabase = 0;
        copyData( toDatabase, tempDataBase, local_db);
        local_db->timeStamp = time( NULL );
    }
}

void errorInfo( const char * elementName, int threadResult )
{
    fprintf(stderr,"Error - create() in %s - code: %d\n", elementName,threadResult);
    exit(EXIT_FAILURE);
}

void changeRecord( Record * oldRecord , Record * newRecord )
{
    oldRecord->key = newRecord->key;
    strcpy( oldRecord->name, newRecord->name );
    oldRecord->value1 = newRecord->value1;
    oldRecord->value2 = newRecord->value2;
}

