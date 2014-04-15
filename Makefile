PREFIX = .
include $(PREFIX)/Makefile.rules

OBJS = Aquaduino
OBJ = $(wildcard $(OBJDIR)/*.o)

compile: $(foreach d, $(OBJS), $(OBJDIR)/$d.o)
	for i in $(DIRS); do make -s -C $$i; done

all: compile
	$(CROSSCPP) $(LDFLAGS) -o Aquaduino.elf $(OBJ) -lm -lc
	
clean:
	rm -f $(OBJDIR)/*
