PREFIX = ..
include $(PREFIX)/Makefile.rules

OBJS = Actuator
OBJS += Controller
OBJS += Flashvars
OBJS += GUIServer
OBJS += NTPSync
OBJS += Object
OBJS += OneWireHandler
OBJS += SDConfigManager
OBJS += Sensor
OBJS += util

all: $(foreach d, $(OBJS), $(OBJDIR)/$d.o)