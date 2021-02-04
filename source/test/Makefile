##########################################
#
##########################################
CUR_DIR = $(shell pwd)
TOP_DIR = ../../
INCS += -I$(TOP_DIR)include
INCS += -I../include

SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SOURCES))
all:$(OBJS)
	@echo $(INCS)
%.o:%.c
	cc -c -g $(INCS) $^
clean:
	-rm  *.o
.PHONY: clean

