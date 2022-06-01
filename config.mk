CURRENT_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

# change these!
INTRO_PATH := $(CURRENT_DIR)../intro
INTRO := intro
INTROLIB_PATH := $(INTRO_PATH)/lib

INC_FLAGS := -I$(INTROLIB_PATH)
LDFLAGS += $(INTROLIB)
CFLAGS += -g

INTROLIB := $(CURRENT_DIR)/introlib.o
