### Build flags for all targets
#
CF_ALL          = $(OPTIMIZATION_LEVEL) $(FEATURE_DEFINES) -Ilibraries/Arduino/ -Ilibraries/ArduinoUnit/ -Ilibraries/ArduinoUnit/utility -Ilibraries/Ethernet -Ilibraries/Ethernet/utility -Ilibraries/HttpClient -Ilibraries/OneWire/ -Ilibraries/SD/ -Ilibraries/SPI/ -Ilibraries/TemplateParser/ -Ilibraries/Time/ -Ilibraries/Webduino/ -Ilibraries/Xively/  -I. -Os -DF_CPU=16000000L -DUSB_PID=NULL -DUSB_VID=NULL -DARDUINO=105 -D__AVR_ATmega2560__ -DDEBUG -Wall -fmessage-length=0 -mmcu=atmega2560 -ffunction-sections -fdata-sections -fno-exceptions
LF_ALL          = $(OPTIMIZATION_LEVEL) -Wl,--gc-sections,--relax -mmcu=atmega2560
LL_ALL          =

### Build tools
# 
CC              = ./build/ccd-gcc
COMP            = $(CC) $(CF_ALL) $(CF_TGT) -o $@ -c $<
LINK            = $(CC) $(LF_ALL) $(LF_TGT) -o $@ $^ $(LL_TGT) $(LL_ALL)
COMPLINK        = $(CC) $(CF_ALL) $(CF_TGT) $(LF_ALL) $(LF_TGT) -o $@ $< $(LL_TGT) $(LL_ALL)

### Standard parts
#
include Rules.mk
