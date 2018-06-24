all: subdirs $(SHARED_LIB) 

include $(WORKING_DIR)/subdirs.mk

$(SHARED_LIB): $(SHARED_OBJECTS) 
	avr-ar crf $(SHARED_LIB) $(SHARED_OBJECTS)

clean: cleansubdirs
	rm -f $(SHARED_OBJECTS)
	rm -f $(SHARED_LIB)
	
.PHONY: $(SHARED_LIB) clean