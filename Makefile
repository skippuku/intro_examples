include config.mk

EXAMPLE_DIRS := $(filter-out ext/,$(shell ls -d */))

define \n


endef

.PHONY: all
all: $(INTROLIB)
	$(foreach d,$(EXAMPLE_DIRS),@$(MAKE) --no-print-directory -C $d$(\n))

$(INTROLIB) introlib.d: $(INTROLIB_PATH)/introlib.c
	$(CC) -c $< $(CFLAGS) -MMD -o $(INTROLIB)

clean_intro_files:
	$(foreach d,$(EXAMPLE_DIRS),rm $d/*.intro$(\n))

-include introlib.d
