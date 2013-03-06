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

# Tool invocations
Aquaduino.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	avr-g++ -L"D:\workspaces\privat\Arduino\Debug" -L"D:\workspaces\privat\TemplateParser\Debug" -L"D:\workspaces\privat\Ethernet\Debug" -L"D:\workspaces\privat\OneWire\Debug" -L"D:\workspaces\privat\SD\Debug" -L"D:\workspaces\privat\SPI\Debug" -L"D:\workspaces\privat\Time\Debug" -L"D:\workspaces\privat\Webduino\Debug" -Os -Wl,--gc-sections,--relax -mmcu=atmega2560 -o "Aquaduino.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) Aquaduino.elf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
