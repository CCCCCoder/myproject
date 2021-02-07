##########################################
#
##########################################
CUR_DIR = $(shell pwd)
TOP_DIR = ../../
include $(TOP_DIR)/Makefile.define
INCS += -I$(TOP_DIR)include
INCS += -I../include

SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SOURCES))
all:$(OBJS)
	@echo $(INCS)
%.o:%.c
	$(CC) -c -g $(INCS) $^
clean:
	-rm  *.o
.PHONY: clean

