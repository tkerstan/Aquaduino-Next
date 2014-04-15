PREFIX = ../..
include $(PREFIX)/Makefile.rules

OBJS = Dhcp Dns Ethernet EthernetClient EthernetServer EthernetUdp

all: $(foreach d, $(OBJS), $(OBJDIR)/$d.o)