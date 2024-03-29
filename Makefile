#Output directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

#TARGET
TARGET := $(BIN_DIR)/pswd-gen

#Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
#Objects derived from Sources
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

#Compiler
CC := gcc

#Flags
CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := -Wall
#LDFLAGS  := -Llib
LDLIBS   := -lm #/usr/lib/ 

.PHONY: all clean

all: $(TARGET)

#Make binary
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

#Make objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ 

#Create obj and bin directory if they don't exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

#Remove objects and binary
clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
