	# Standard things

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)


# Subdirectories, in random order
#dir	:= $(d)/test
#include		$(dir)/Rules.mk


# Local variables

OBJS_$(d)	:= $(d)/Actuator.o $(d)/Controller.o \
		       $(d)/GUIServer.o $(d)/NTPSync.o $(d)/Object.o \
		       $(d)/OneWireHandler.o $(d)/SDConfigManager.o \
		       $(d)/Sensor.o $(d)/util.o $(d)/Aquaduino.o
DEPS_$(d)	:= $(OBJS_$(d):%=%.d)
CLEAN		:= $(CLEAN) $(OBJS_$(d)) $(DEPS_$(d))

#TGTS_$(d)	:= ArrayMap_test.elf
#TGT_BIN		:= $(TGT_BIN) ArrayMap_test.elf
AQ_OBJS_ALL	:= $(AQ_OBJS_ALL) $(OBJS_$(d))
CLEAN		:= $(CLEAN) $(TGTS_$(d)) $(DEPS_$(d))

# Local rules
$(OBJS_$(d)):	CF_TGT := -I$(d)

#ArrayMap_test.elf: CF_TGT := -I$(d)
#ArrayMap_test.elf: $(d)/ArrayMap_test.o libraries/ArduinoUnit/utility/ArduinoUnit.o \
#                   libraries/ArduinoUnit/utility/FakeStream.o libraries/ArduinoUnit/utility/FakeStreamBuffer.o \
#                   libraries/ArduinoUnit/utility/FreeMemory.o libraries/Arduino/HardwareSerial.o \
#                   libraries/Arduino/Print.o libraries/Arduino/new.o libraries/Arduino/main.o \
#                   libraries/Arduino/wiring.o
#	@echo "Linking $@"
#	$(LINK)

# Standard things

-include	$(DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

