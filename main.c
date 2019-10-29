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
    

    deleteDataBase( local_db );
    return 0;
}

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
    {
        upgradeLocalDatabase( local_db );
        printf("received SIGUSR1\n");
    }
}

void initSignal()
{
    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGUSR1\n");
}