#include <stdio.h>
#include <stdlib.h>

#include "./db_library/database.h"
#include "./testData/testData.h"

int main(void)
{
    DataBase * local_db;
    local_db = newDataBase( MAX_DATABASE_SIZE );

    InitDataBase( local_db );
    sleep(3);
    printDataBase( local_db );
    makeDatabase( local_db );
    printDataBase( local_db );

    writeDataBase( local_db );
    
    makeDatabase( local_db );
    printDataBase( local_db );
    sleep(3);
    readDataBase( local_db);

    printDataBase( local_db );

    deleteDataBase( local_db );
    return 0;
}
