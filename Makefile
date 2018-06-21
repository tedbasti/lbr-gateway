SUBDIRS = src

all: subdirs
	
clean: 
	for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done

.PHONY: clean subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
    
