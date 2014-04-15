PREFIX = ..
include $(PREFIX)/Makefile.rules

OBJS = ClockTimer
OBJS += ClockTimerController
OBJS += LevelController
OBJS += TemperatureController

all: $(foreach d, $(OBJS), $(OBJDIR)/$d.o)