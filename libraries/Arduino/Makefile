PREFIX = ../..
include $(PREFIX)/Makefile.rules

OBJS = CDC HardwareSerial HID IPAddress main new Print Stream Tone USBCore WInterrupts wiring_analog wiring_digital wiring_pulse wiring_shift wiring WMath WString

all: $(foreach d, $(OBJS), $(OBJDIR)/$d.o)