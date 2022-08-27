include config.mk

EXAMPLE_DIRS := $(filter-out ext/,$(shell ls -d */))

define \n


endef

.PHONY: all
all: $(INTROLIB)
	$(foreach d,$(EXAMPLE_DIRS),@$(MAKE) --no-print-directory -C $d$(\n))

$(INTROLIB) introlib.d: ext/intro_impl.cpp $(INTROLIB_PATH)/intro.h
	$(CC) -std=c++11 -c $< $(CFLAGS) -MMD -o $(INTROLIB) -I$(INTROLIB_PATH)

clean_intro_files:
	$(foreach d,$(EXAMPLE_DIRS),rm $d/*.intro$(\n))

-include introlib.d
