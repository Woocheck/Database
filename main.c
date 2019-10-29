#include <stdio.h>
#include <stdlib.h>

#include "./db_library/API_db_library.h"
#include "./testData/testData.h"


int main(void)
{
    DataBase * local_db;
    local_db = newDataBase( MAX_DATABASE_SIZE );

    makeDatabase(local_db);
    
    InitDataBase( local_db );
    sleep(1);
    printDataBase( local_db );

    makeDatabase( local_db );
    printDataBase( local_db );

    Record newRecord;
    newRecord.key = 2;
    strcpy(newRecord.name, "nowy record");
    newRecord.value1 = 111111;
    newRecord.value2 = 100000;
    changeElementDataBase( local_db, &newRecord );

    newRecord.key = 3;
    strcpy(newRecord.name, "nowy record");
    newRecord.value1 = 222222;
    newRecord.value2 = 100000;
    changeElementDataBase( local_db, &newRecord );
     
    writeDataBase( local_db );
    sleep(1);
    print_list( &(local_db->listOfChanges) );
    deleteDataBase( local_db );
    return 0;
}
