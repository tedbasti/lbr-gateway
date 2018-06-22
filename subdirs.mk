include $(WORKING_DIR)/basemake.mk

cleansubdirs: 
	for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
	
.PHONY: cleansubdirs subdirs $(SUBDIRS)