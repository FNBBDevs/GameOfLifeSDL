CC = g++ -std=c++17
FILENAME = main
ERRORS = -Wall
FLAGS = -IC:/msys64/mingw64/include/SDL2 -Dmain=SDL_main -LC:/msys64/mingw64/lib -lmingw32 -mwindows -lSDL2main -lSDL2

ifeq ($(OS),Windows_NT)
    PATHTOMAIN = src\\
    PATHTOBIN = bin\\
    FLAG = -D WIN32 -O3
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
    FLAG = -O3
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
	@echo Building . . .
	@$(CC) -o $(PATHTOBIN)$(FILENAME) $(PATHTOMAIN)$(FILENAME).cpp $(FLAG) $(ERRORS) $(FLAGS) 
	@echo Successfully built: $(FILENAME)
	@$(PATHTOBIN)./$(FILENAME)
	@echo Terminating . . .

clean:
	@echo Deleting . . .
	@$(RM)
	@echo Successfully deleted: $(FILENAME)