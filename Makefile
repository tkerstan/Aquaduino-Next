include Makefile.rules



compile: $(foreach d, $(OBJS), $(OBJDIR)/$d)
	@for i in $(DIRS); do make -s -C $$i compile; done

depend: $(foreach d, $(DEPS), $(OBJDIR)/$d)
	@for i in $(DIRS); do make -s -C $$i depend; done

all: compile
	@echo "Linking Aquaduino.elf"
	@$(CROSSCPP) $(LDFLAGS) -o Aquaduino.elf $(shell find $(OBJDIR) -type f -name '*.o') -lm -lc >> build.log
	
clean:
	rm -rf $(OBJDIR)/*
