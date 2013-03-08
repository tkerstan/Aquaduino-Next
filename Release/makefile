################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Sensors/subdir.mk
-include Framework/subdir.mk
-include Controller/subdir.mk
-include Actuators/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Aquaduino.elf

dependents:
	-cd D:/workspaces/privat/Arduino/Release && $(MAKE) clean all
	-cd D:/workspaces/privat/Ethernet/Release && $(MAKE) clean all
	-cd D:/workspaces/privat/OneWire/Release && $(MAKE) clean all
	-cd D:/workspaces/privat/SD/Release && $(MAKE) clean all
	-cd D:/workspaces/privat/SPI/Release && $(MAKE) clean all
	-cd D:/workspaces/privat/TemplateParser/Release && $(MAKE) clean all
	-cd D:/workspaces/privat/Time/Release && $(MAKE) clean all
	-cd D:/workspaces/privat/Webduino/Release && $(MAKE) clean all

# Tool invocations
Aquaduino.elf: $(OBJS) $(USER_OBJS) D:/workspaces/privat/Arduino/Release/libArduino.a D:/workspaces/privat/Ethernet/Release/libEthernet.a D:/workspaces/privat/OneWire/Release/libOneWire.a D:/workspaces/privat/SD/Release/libSD.a D:/workspaces/privat/SPI/Release/libSPI.a D:/workspaces/privat/TemplateParser/Release/libTemplateParser.a D:/workspaces/privat/Time/Release/libTime.a D:/workspaces/privat/Webduino/Release/libWebduino.a
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	avr-gcc -L"D:\workspaces\privat\Arduino\Release" -L"D:\workspaces\privat\TemplateParser\Release" -L"D:\workspaces\privat\Ethernet\Release" -L"D:\workspaces\privat\OneWire\Release" -L"D:\workspaces\privat\SPI\Release" -L"D:\workspaces\privat\Time\Release" -L"D:\workspaces\privat\Webduino\Release" -L"D:\workspaces\privat\SD\Release" -Os -Wl,--gc-sections,--relax -mmcu=atmega2560 -o "Aquaduino.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) Aquaduino.elf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
D:/workspaces/privat/Arduino/Release/libArduino.a:
D:/workspaces/privat/Ethernet/Release/libEthernet.a:
D:/workspaces/privat/OneWire/Release/libOneWire.a:
D:/workspaces/privat/SD/Release/libSD.a:
D:/workspaces/privat/SPI/Release/libSPI.a:
D:/workspaces/privat/TemplateParser/Release/libTemplateParser.a:
D:/workspaces/privat/Time/Release/libTime.a:
D:/workspaces/privat/Webduino/Release/libWebduino.a:

-include ../makefile.targets
