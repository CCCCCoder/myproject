LIB_TARGET=libbase.a

CUR_DIR = $(shell pwd)
TOP_DIR = ../

include $(TOP_DIR)/Makefile.define
INCS   += -I$(CUR_DIR)/include
INCS   += -I$(TOP_DIR)/include
SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SOURCES))

lib:$(LIB_TARGET)
$(LIB_TARGET):$(OBJS)
	@echo "lib"
	$(AR) cq $@ $^
%.o:%.c 
	$(CC) -c -g $(INCS)  $^
.PHONY : clean
clean:
	-rm *.o $(LIB_TARGET)
