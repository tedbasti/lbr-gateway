include $(WORKING_DIR)/basemake.mk

all: $(SHARED_LIB) 

$(SHARED_LIB): $(SHARED_OBJECTS) 
	avr-ar crf $(BUILD_DIR)/$(SHARED_LIB) $(SHARED_OBJECTS)

clean: 
	rm $(SHARED_OBJECTS)
	rm $(BUILD_DIR)/$(SHARED_LIB)
	
.PHONY: $(SHARED_LIB) clean