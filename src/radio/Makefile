SHARED_OBJECTS := layer1/layer1.o layer2/layer2.o layer3/layer3.o
SHARED_LIB := radio.a
export WORKING_DIR ?= $(PWD)/../..
SUBDIRS := layer1 layer2 layer3

include $(WORKING_DIR)/component.mk

$(SHARED_OBJECTS): subdirs