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
    sleep( 2 );
    printDataBase( local_db );

int licznik = 0;

while(1)
{
    
    for( int x = 6; x<10; x++)
    {
        changeElementDataBase( local_db, makeTestRecord( x ) );
    }
    writeDataBase( local_db );

    readDataBase( local_db );
    
    sleep( 1 );
    printf("licznik:  %d\n", licznik);
    
    printDataBase( local_db );
    
    licznik++;
}
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