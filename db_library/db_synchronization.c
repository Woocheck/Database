#include <stdio.h>
#include <stdlib.h>    // EXIT_SUCCESS
#include <sys/mman.h>  // PROT_*, MAP_SHARED 
#include <fcntl.h>     // O_* constants
#include <unistd.h>    // ftruncate, close
#include <sys/wait.h>  // wait
#include <string.h>    // strcpy
#include <semaphore.h>


#include "./database.h"
#include "./db_synchronization.h"

SharedMemoryStruct * initSynchronisatoin( DataBase * local_db )
{
    SharedMemoryStruct * shm_ptr   = NULL;

    int  shm_fd = shm_open( "/buffer_shm", O_CREAT | O_EXCL | O_RDWR, 0600 );
    ftruncate( shm_fd, sizeof( SharedMemoryStruct ) );

	shm_ptr = mmap(NULL, 	              
				  sizeof( SharedMemoryStruct ),      
				  PROT_READ | PROT_WRITE, 
				  MAP_SHARED,             
				  shm_fd, 
				  0);

    return shm_ptr;
};
void writeDataSynchronisation( Record * record )
{
    
};