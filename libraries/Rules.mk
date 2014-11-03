# Standard things

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)


# Subdirectories, in random order
dir	:= $(d)/Arduino
include		$(dir)/Rules.mk

dir	:= $(d)/ArduinoUnit
include		$(dir)/Rules.mk	
	
dir	:= $(d)/Ethernet
include		$(dir)/Rules.mk	

dir	:= $(d)/HttpClient
include		$(dir)/Rules.mk	

dir	:= $(d)/OneWire
include		$(dir)/Rules.mk	

dir	:= $(d)/SD
include		$(dir)/Rules.mk	

dir	:= $(d)/SPI
include		$(dir)/Rules.mk	

dir	:= $(d)/Time
include		$(dir)/Rules.mk	

dir	:= $(d)/Xively
include		$(dir)/Rules.mk

# Local variables

OBJS_$(d)	:=
DEPS_$(d)	:= $(OBJS_$(d):%=%.d)
CLEAN		:= $(CLEAN) $(OBJS_$(d)) $(DEPS_$(d))

TGTS_$(d)	:=
AQ_OBJS_ALL	:= $(AQ_OBJS_ALL) $(OBJS_$(d))
CLEAN		:= $(CLEAN) $(TGTS_$(d)) $(DEPS_$(d))

# Local rules
$(OBJS_$(d)):	CF_TGT := -I$(d)

# Standard things

-include	$(DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

