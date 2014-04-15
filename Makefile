include Makefile.rules

compile: $(foreach d, $(OBJS), $(OBJDIR)/$d)
	for i in $(DIRS); do make -s -C $$i compile; done

all: compile
	@echo "Linking Aquaduino.elf"
	@$(CROSSCPP) $(LDFLAGS) -o Aquaduino.elf $(wildcard $(OBJDIR)/*.o) -lm -lc >> build.log
	
clean:
	rm -f $(OBJDIR)/*
