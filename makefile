# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: splflix

# Tool invocations
splflix: bin/Main.o bin/Session.o bin/User.o bin/Watchable.o bin/Action.o bin/Tag.o
	@echo 'Building target: splflix invoking: C++ Linker'
	g++ -o bin/splflix bin/Main.o bin/Session.o bin/User.o bin/Watchable.o bin/Action.o bin/Tag.o $(LFLAGS)
	@echo 'Finished building target: splflix'

# Depends on the source and header files
bin/Main.o: src/Main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp
bin/Action.o: src/Action.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp
bin/Session.o: src/Session.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Session.o src/Session.cpp
bin/Watchable.o: src/Watchable.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Watchable.o src/Watchable.cpp
bin/User.o: src/User.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/User.o src/User.cpp
bin/Tag.o: src/Tag.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Tag.o src/Tag.cpp

#Clean the build directory
clean: 
	rm -f bin/*
