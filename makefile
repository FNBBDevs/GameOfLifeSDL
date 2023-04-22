#This makefile checks for OS to compile, run, and delete executable. 
#commands:  - "make" to compile/run
#           - "clean" to delete exe (required to recompile)
#- Nolan

CC = g++
FILENAME = main

ifeq ($(OS),Windows_NT)
    FLAG = -Isrc/Include -Lsrc/lib -D WIN32 -lmingw32 -lSDL2main -lSDL2 -O2
	RM = del $(FILENAME).exe
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        FLAG += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            FLAG += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            FLAG += -D IA32
        endif
    endif
else
	RM = rm -f $(FILENAME)
	PATH = $1 
    FLAG = -std=c++17 -lSDL2 -O3
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        FLAG += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        FLAG += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        FLAG += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        FLAG += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        FLAG += -D ARM
    endif
endif

make:
	$(CC) -o main main.cpp $(FLAG)
	./main

clean:
	$(RM)