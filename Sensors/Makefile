PREFIX = ..
include $(PREFIX)/Makefile.rules

OBJS = DigitalInput DS18S20 SerialInput

all: $(foreach d, $(OBJS), $(OBJDIR)/$d.o)