# Standard things

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)


# Subdirectories, in random order
#dir	:= $(d)/test
#include		$(dir)/Rules.mk	

# Local variables

OBJS_$(d)	:= $(d)/CDC.o $(d)/HardwareSerial.o \
               $(d)/HID.o $(d)/IPAddress.o \
               $(d)/main.o $(d)/new.o $(d)/Print.o \
               $(d)/Stream.o $(d)/Tone.o $(d)/USBCore.o \
               $(d)/WInterrupts.o $(d)/wiring_analog.o $(d)/wiring_digital.o \
               $(d)/wiring_pulse.o $(d)/wiring_shift.o $(d)/wiring.o \
               $(d)/WMath.o $(d)/WString.o
DEPS_$(d)	:= $(OBJS_$(d):%=%.d)
CLEAN		:= $(CLEAN) $(OBJS_$(d)) $(DEPS_$(d))

TGTS_$(d)	:=
LL_ALL		:= $(LL_ALL) $(OBJS_$(d))
CLEAN		:= $(CLEAN) $(TGTS_$(d)) $(DEPS_$(d))

# Local rules
$(OBJS_$(d)):	CF_TGT := -I$(d)

# Standard things

-include	$(DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

