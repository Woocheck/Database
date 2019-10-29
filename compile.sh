#!/bin/bash
g++ main.c  ./testData/testData.c \
            ./db_library/API_db_library.c \
            ./db_library/db_changelist.c \
            ./db_library/db_synchronization.c \
            ./db_library/db_threads.c \
            ./db_library/db_utility.c \
            -o main -pthread -lrt -Wall -pedantic 
