include ../config.mk

SRC := $(wildcard *.c)
INTRO_FILE := $(addsuffix .intro,$(SRC))
EXE := $(SRC:%.c=db_%)

all: $(EXE)

$(EXE): $(SRC) $(INTRO_FILE) $(INTROLIB)
	$(CC) $< $(CFLAGS) $(INC_FLAGS) $(LDFLAGS) -o $@

$(INTRO_FILE): $(SRC)
	$(INTRO) $(INC_FLAGS) $< -o $@
