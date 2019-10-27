#ifndef _TEST_DATA_
#define _TEST_DATA_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <string.h>

#include "../db_library/database.h"

Record * makeRecord( int recordNumber );
DataBase * makeDatabase( DataBase * local_db );

void printRecord( Record * record);
void printDataBase( DataBase * dataBase );

#endif 
