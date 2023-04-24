#OBJS specifies which files to compile as part of the project
OBJS = b_main.c r_render.c i_event.c f_save.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -std=gnu99 -Wall -Wno-missing-braces -Wunused-result

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ../bin/bittens-adventure

DISCORD ?= 0
ifeq ($(DISCORD),TRUE)
	LINKER_FLAGS += -ldiscord-rpc -lstdc++
	COMPILER_FLAGS += -DDISCORD
endif
#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
