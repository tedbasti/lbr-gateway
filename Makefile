export WORKING_DIR := $(PWD)
export BUILD_DIR := $(WORKING_DIR)/build

SUBDIRS := src

all: createbuild subdirs

include $(WORKING_DIR)/subdirs.mk
	
clean: cleansubdirs
	rm -rf $(BUILD_DIR)
	
createbuild:
	mkdir -p $(BUILD_DIR)

.PHONY: createbuild clean

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
    
