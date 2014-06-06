# Standard things

.ONESHELL:

all: targets


# Subdirectories, in random order

dir	:= Actuators
include		$(dir)/Rules.mk

dir	:= Controller
include		$(dir)/Rules.mk

dir	:= Framework
include		$(dir)/Rules.mk

dir	:= Sensors
include		$(dir)/Rules.mk

dir	:= libraries
include		$(dir)/Rules.mk

# General directory independent rules

%.o: %.c
	@echo "Compiling $<"
	$(COMP)

%.o: %.cpp
	@echo "Compiling $<"
	$(COMP)

%: %.o
	@echo "Linking $@"
	$(LINK)

%: %.c
	@echo "Compiling $< & Linking $@"
	$(COMPLINK)
	
# Local variables

OBJS_$(d)	:= 
DEPS_$(d)	:= $(OBJS_$(d):%=%.d)
CLEAN		:= $(CLEAN) $(OBJS_$(d)) $(DEPS_$(d))

TGTS_$(d)	:= Aquaduino.elf
TGT_BIN		:= $(TGT_BIN) Aquaduino.elf
AQ_OBJS_ALL	:= $(AQ_OBJS_ALL) $(OBJS_$(d))
CLEAN		:= $(CLEAN) $(TGTS_$(d)) $(DEPS_$(d))

# Local rules
$(OBJS_$(d)):	CF_TGT := -I$(d)

Aquaduino.elf : Framework/AquaduinoMain.o $(AQ_OBJS_ALL)
	@echo "Linking $@"
	$(LINK)
	
%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom $< $@

objdump: Aquaduino.elf
	avr-objdump -S Aquaduino.elf > Aquduino.dump

size: Aquaduino.elf
	avr-size Aquaduino.elf

upload: Aquaduino.hex
	avrdude -C$(AVRDUDECONF) -patmega2560 -cwiring -P$(ARDUINOCOM) -b115200 -D -Uflash:w:Aquaduino.hex:i 

upload_test: ArrayMap_test.hex
	avrdude -C$(AVRDUDECONF) -patmega2560 -cwiring -P$(ARDUINOCOM) -b115200 -D -Uflash:w:ArrayMap_test.hex:i 

# Standard things

-include	$(DEPS_$(d))


# The variables TGT_*, CLEAN and CMD_INST* may be added to by the Makefile
# fragments in the various subdirectories.

TGT_BIN	:= $(TGT_BIN)

.PHONY:		targets
targets:	$(TGT_BIN) $(TGT_SBIN) $(TGT_ETC) $(TGT_LIB)
	
.PHONY:		clean
clean:
		rm -f $(CLEAN)


# Prevent make from removing any build targets, including intermediate ones

.SECONDARY:	$(CLEAN)

