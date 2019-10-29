#ifndef _TEST_DATA_
#define _TEST_DATA_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <string.h>

#include "../db_library/API_db_library.h"

Record * makeTestRecord( int recordNumber );
DataBase * makeTestDatabase( DataBase * local_db );

void printRecord( Record * record);
void printDataBase( DataBase * dataBase );

#endif 
