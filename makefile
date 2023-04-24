#commands:  - "make" to compile/run
#           - "clean" to delete exe (required to recompile)

CC = g++ -std=c++17
FILENAME = main
FLAGS = -Wall -Werror -pedantic
WINSDL = `sdl2-config --cflags --libs`
SDLFLAG = -lSDL2 -lSDL2main -lm

ifeq ($(OS),Windows_NT)
    PATHTOMAIN = src\
    PATHTOBIN = bin\
    FLAG = -D WIN32 -lmingw32 -lSDL2main -lSDL2 -O3
    SDLFLAG += WINSDL
	RM = del $(PATHTOBIN)$(FILENAME).exe
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
    PATHTOMAIN = src/
	PATHTOBIN = bin/
	RM = rm -f $(PATHTOBIN)$(FILENAME)
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
	@echo
	@echo '>> Building . . .'
	@echo
	@$(CC) $(FLAGS) -o $(PATHTOBIN)$(FILENAME) $(PATHTOMAIN)$(FILENAME).cpp $(FLAG) $(SDLFLAG)
	@echo
	@echo '>> Successfully built: $(FILENAME)'
	@echo
	@$(PATHTOBIN)./$(FILENAME)
	@echo
	@echo ">> Terminating . . ."
	@echo

clean:
	@echo
	@echo '>> Deleting . . .'
	@echo
	@$(RM)
	@echo '>> Successfully deleted: $(FILENAME)'
	@echo