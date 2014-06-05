# Standard things

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)


# Subdirectories, in random order
#dir	:= $(d)/test
#include		$(dir)/Rules.mk	

# Local variables

OBJS_$(d)	:= $(d)/DigitalInput.o $(d)/DS18S20.o \
               $(d)/SerialAtlasEC.o $(d)/SerialAtlasORP.o \
               $(d)/SerialAtlasPH.o
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

