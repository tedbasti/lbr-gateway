export WORKING_DIR := $(PWD)

SUBDIRS := src

all: subdirs

include $(WORKING_DIR)/subdirs.mk
	
clean: cleansubdirs

.PHONY: createbuild clean