include $(WORKING_DIR)/basemake.mk

all: $(SHARED_LIB) 

$(SHARED_LIB): $(SHARED_OBJECTS) 
	avr-ar crf $(SHARED_LIB) $(SHARED_OBJECTS)

clean: 
	rm $(SHARED_OBJECTS)
	rm $(SHARED_LIB)
	
.PHONY: $(SHARED_LIB) clean