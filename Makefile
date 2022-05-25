include config.mk

EXAMPLE_DIRS := $(shell ls -d */)

define \n


endef

.PHONY: all
all: $(INTROLIB)
	$(foreach d,$(EXAMPLE_DIRS),@$(MAKE) -C $d$(\n))

$(INTROLIB) introlib.d: $(INTROLIB_PATH)/introlib.c
	$(CC) -c $< $(CFLAGS) -MMD -o $(INTROLIB)

-include introlib.d
