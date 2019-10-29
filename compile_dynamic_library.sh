#!/bin/bash

g++ -fPIC -c -Wall -ansi -pedantic ./db_library/API_db_library.c -o ./Relase/db_library/API_db_library.o
g++ -fPIC -c -Wall -ansi -pedantic ./db_library/db_changelist.c -o ./Relase/db_library/db_changelist.o
g++ -fPIC -c -Wall -ansi -pedantic ./db_library/db_synchronization.c -o ./Relase/db_library/db_synchronization.o
g++ -fPIC -c -Wall -ansi -pedantic ./db_library/db_threads.c -o ./Relase/db_library/db_threads.o
g++ -fPIC -c -Wall -ansi -pedantic ./db_library/db_utility.c -o ./Relase/db_library/db_utility.o

g++ -shared -Wl,-soname,libdb_library.so.1 -o ./Relase/libdb_library.so.1.0.1 \
                                            ./Relase/db_library/API_db_library.o \
                                            ./Relase/db_library/db_changelist.o \
                                            ./Relase/db_library/db_synchronization.o \
                                            ./Relase/db_library/db_threads.o \
                                            ./Relase/db_library/db_utility.o
                                           
ln -sf ./Relase/libdb_library.so.1.0.1 libdb_library.so.1
ln -sf ./Relase/libdb_library.so.1.0.1 libdb_library.so
cp ./Relase/libdb_library.so* /usr/lib

g++ main.c ./testData/testData.c -o main -L/usr/lib -ldb_library -pthread -lrt -Wall -pedantic 