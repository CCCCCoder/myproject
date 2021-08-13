##########################################
#
##########################################
CUR_DIR = $(shell pwd)
TOP_DIR = ../
SUB_DIR := $(CUR_DIR)/test
include $(TOP_DIR)/Makefile.define
INCS += -I$(TOP_DIR)/include  #-I 增加include目录
INCS += -I$(CUR_DIR)/include  #-I 增加include目录

SOURCES = $(wildcard *.c) #查找所有的.c，源文件
OBJS = $(patsubst %.c,%.o,$(SOURCES)) #将.c替换未.o，.o未目标文件

#TAR = main
TEST_LIB = main
all:$(TEST_LIB)
$(TEST_LIB):$(OBJS)
	@echo $@ #所有.o
	@echo $^ #所有.c
	@echo $(SUB_DIR)
	cd $(SUB_DIR) && make clean all
	$(CC) -o $@ $(SUB_DIR)/*.o $^ ../lib/libbase.a -lpthread
	mv main ../bin/test
%.o:%.c 
	$(CC) -c -g $(INCS)  $^
clean:
	-rm $(TEST_LIB) *.o
	cd $(SUB_DIR) &&make clean 
.PHONY: clean test

