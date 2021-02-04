export SUB_TAR = buid-in.o
TAR = main
CUR_DIR = $(shell pwd)
SOURCE := $(CUR_DIR)/source
LIB_DIR:= $(CUR_DIR)/lib
all:
	@echo $(SOURCE)
	cd $(LIB_DIR) && make clean lib
	cd $(SOURCE) && make clean all
clean:
	cd $(SOURCE) && make clean
	cd $(LIB_DIR) && make clean 
	-rm $(CUR_DIR)/bin/test
.PHONY: clean
