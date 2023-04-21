#This makefile checks for OS to compile, run, and delete executable. 
#commands:  - "make" to compile/run
#           - "clean" to delete exe (required to recompile)
#- Nolan

CC = g++
FLAG = -std=c++17 -lSDL2 -O3
ifeq ($(OS),Windows_NT)
    FLAG += -D WIN32
	RM = del /Q
	PATH = $(subst /,\,$1)
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
	RM = rm -f
	PATH = $1
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
	$(CC) $(FLAG) -o main main.cpp
	./main

clean:
	$(RM) main