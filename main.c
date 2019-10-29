#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "./db_library/API_db_library.h"
#include "./testData/testData.h"

void sig_handler(int signo);
void initSignal();

DataBase * local_db;

int main(void)
{
    initSignal();
    
    local_db = newDataBase( MAX_DATABASE_SIZE );
    InitDataBase( local_db );

    makeTestDatabase( local_db );
    printDataBase( local_db );
    while(1)
    {
        for( int i = 0; i < MAX_DATABASE_SIZE; i++ )
        {
            changeElementDataBase( local_db, makeTestRecord( i ) );
        }
        sleep(1);
        writeDataBase( local_db );
        printDataBase( local_db );
        
    }
    
    deleteDataBase( local_db );
    return 0;
}

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
    {
        printf("received SIGUSR1\n");
        upgradeLocalDatabase( local_db );
    }
}

void initSignal()
{
    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGUSR1\n");
}