#include <stdio.h>
#include <stdlib.h>

#include "./db_library/database.h"


int main(int argc, char const *argv[])
{
    struct DataBase * local_db;
    local_db = malloc( sizeof( struct DataBase ) );

    InitDataBase( local_db );
    
    free( local_db );
    return 0;
}
