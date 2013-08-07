cmake_minimum_required(VERSION 2.8)

INCLUDE(CMakeForceCompiler)
include(ExternalProject)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_CONFIGURATION_TYPES Debug,Release)
SET(CMAKE_COLOR_MAKEFILE OFF)

set(AVR_COMPILER_PATH "c:/avr-gcc-4.7.2/" CACHE PATH "Path to the AVR GCC installation")
set(ARDUINO_SDK_PATH "c:/arduino" CACHE PATH "Path to the Arduino installation")

if (WIN32)
	SET(CMAKE_C_COMPILER ${AVR_COMPILER_PATH}/avr-gcc.exe)
	SET(CMAKE_CXX_COMPILER ${AVR_COMPILER_PATH}/avr-g++.exe)
	SET(AVRDUDE ${AVR_COMPILER_PATH}/avrdude.exe)
else()
	SET(CMAKE_C_COMPILER ${AVR_COMPILER_PATH}/avr-gcc)
	SET(CMAKE_CXX_COMPILER ${AVR_COMPILER_PATH}/avr-g++)
	SET(AVRDUDE ${AVR_COMPILER_PATH}/avrdude)
endif()

if ((EXISTS ${ARDUINO_SDK_PATH}) AND (EXISTS ${AVR_COMPILER_PATH}))

	SET(ARDUINO_CORE ${ARDUINO_SDK_PATH}/hardware/arduino/cores/arduino)
	SET(ARDUINO_LIBS ${ARDUINO_SDK_PATH}/libraries)
	SET(ARDUINO_VARIANT ${ARDUINO_SDK_PATH}/hardware/arduino/variants/mega)

	SET(CSTANDARD "")
	SET(CWARN "-Wall -Wstrict-prototypes")
	SET(CTUNING "-fmessage-length=0 -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
	SET(CMCU "-mmcu=atmega2560")

	SET(CMAKE_C_FLAGS_DEBUG  "-g3 -Os -DDEBUG")
	SET(CMAKE_CXX_FLAGS_DEBUG  "-g3 -Os -DDEBUG -fno-rtti -fno-exceptions")
	SET(LINK_FLAGS_DEBUG  "-g3 -Os -DDEBUG")

	SET(CMAKE_C_FLAGS_RELEASE "-Os")
	SET(CMAKE_CXX_FLAGS_RELEASE "-Os -fno-rtti -fno-exceptions")
	SET(LINK_FLAGS_RELEASE  "-Os")

	SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections,--relax")

	SET(CMAKE_C_FLAGS "${CMCU} ${CTUNING} ${CWARN} ${CSTANDARD}")
	SET(CMAKE_CXX_FLAGS "${CMCU} ${CTUNING}")
	add_definitions(-DF_CPU=16000000 -D__AVR__ -D__AVR_LIBC_DEPRECATED_ENABLE__=1 -DUSB_VID=NULL -DUSB_PID=NULL -DARDUINO=103 -D__AVR_ATmega2560__)


	if (NOT EXISTS ${ARDUINO_LIBS}/OneWire)
		message("OneWire not available")
		file(DOWNLOAD "http://www.pjrc.com/teensy/arduino_libraries/OneWire.zip" "./OneWire.zip")
		execute_process(COMMAND unzip OneWire.zip -d ${ARDUINO_LIBS})
	endif()

	if (NOT EXISTS ${ARDUINO_LIBS}/Time)
		message("Time not available")
		file(DOWNLOAD "http://www.pjrc.com/teensy/arduino_libraries/Time.zip" "./Time.zip")
		execute_process(COMMAND unzip Time.zip -d ${ARDUINO_LIBS})
		execute_process(COMMAND patch -p1 -d ${ARDUINO_LIBS}/Time -i ${CMAKE_CURRENT_SOURCE_DIR}/patches/time.patch)
	endif()

	if (NOT EXISTS ${ARDUINO_LIBS}/TemplateParser)
		message("TemplateParser not available")
		file(DOWNLOAD "https://github.com/tkerstan/ArduinoTemplateParser/archive/master.zip" "./TemplateParser.zip")
		execute_process(COMMAND unzip TemplateParser.zip -d ${ARDUINO_LIBS})
		file(RENAME ${ARDUINO_LIBS}/ArduinoTemplateParser-master ${ARDUINO_LIBS}/TemplateParser)
	endif()

	if (NOT EXISTS ${ARDUINO_LIBS}/Webduino)
		message("Webduino not available")
		file(DOWNLOAD "https://github.com/sirleech/Webduino/archive/master.zip" "./Webduino.zip")
		execute_process(COMMAND unzip Webduino.zip -d ${ARDUINO_LIBS})
		file(RENAME ${ARDUINO_LIBS}/Webduino-master ${ARDUINO_LIBS}/Webduino)
	endif()

	if (NOT EXISTS ${ARDUINO_LIBS}/Xively)
		message("Xively not available")
		file(DOWNLOAD "https://github.com/xively/xively_arduino/archive/master.zip" "./Xively.zip")
		execute_process(COMMAND unzip Xively.zip -d ${ARDUINO_LIBS})
		file(RENAME ${ARDUINO_LIBS}/xively_arduino-master ${ARDUINO_LIBS}/Xively)
		execute_process(COMMAND patch -p1 -d ${ARDUINO_LIBS}/Xively -i ${CMAKE_CURRENT_SOURCE_DIR}/patches/Xively-Aquaduino.patch)
	endif()

	if (NOT EXISTS ${ARDUINO_LIBS}/HttpClient)
		message("HttpClient not available")
		file(DOWNLOAD "https://github.com/amcewen/HttpClient/archive/master.zip" "./HttpClient.zip")
		execute_process(COMMAND unzip HttpClient.zip -d ${ARDUINO_LIBS})
		file(RENAME ${ARDUINO_LIBS}/HttpClient-master ${ARDUINO_LIBS}/HttpClient)
	endif()

	include_directories(${ARDUINO_CORE})
	include_directories(${ARDUINO_VARIANT})
	include_directories(${ARDUINO_LIBS}/SPI)
	include_directories(${ARDUINO_LIBS}/Ethernet)
	include_directories(${ARDUINO_LIBS}/Ethernet/utility)
	include_directories(${ARDUINO_LIBS}/SD)
	include_directories(${ARDUINO_LIBS}/SD/utility)
	include_directories(${ARDUINO_LIBS}/OneWire)
	include_directories(${ARDUINO_LIBS}/Time)
	include_directories(${ARDUINO_LIBS}/TemplateParser)
	include_directories(${ARDUINO_LIBS}/Webduino)
	include_directories(${ARDUINO_LIBS}/Xively)
	include_directories(${ARDUINO_LIBS}/HttpClient)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Actuators)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Controller)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Framework)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Sensors)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})

	project(aquaduino)

	file(GLOB ARDUINO_CORE_SRC ${ARDUINO_CORE}/*.cpp ${ARDUINO_CORE}/*.c)
	add_library(arduino_core STATIC ${ARDUINO_CORE_SRC})

	file(GLOB ARDUINO_LIB_SPI_SRC ${ARDUINO_LIBS}/SPI/*.cpp ${ARDUINO_LIBS}/SPI/*.c)
	add_library(arduino_spi STATIC ${ARDUINO_LIB_SPI_SRC})
	target_link_libraries(arduino_spi arduino_core)

	file(GLOB ARDUINO_LIB_ETHERNET_SRC ${ARDUINO_LIBS}/Ethernet/*.cpp ${ARDUINO_LIBS}/Ethernet/*.c ${ARDUINO_LIBS}/Ethernet/utility/*.cpp ${ARDUINO_LIBS}/utility/Ethernet/*.c)
	add_library(arduino_ethernet STATIC ${ARDUINO_LIB_ETHERNET_SRC})
	target_link_libraries(arduino_ethernet arduino_core arduino_spi)

	file(GLOB ARDUINO_LIB_SD_SRC ${ARDUINO_LIBS}/SD/*.cpp ${ARDUINO_LIBS}/SD/*.c ${ARDUINO_LIBS}/SD/utility/*.cpp ${ARDUINO_LIBS}/SD/utility/*.c)
	add_library(arduino_sd STATIC ${ARDUINO_LIB_SD_SRC})
	target_link_libraries(arduino_sd arduino_core arduino_spi)

	file(GLOB ARDUINO_LIB_ONEWIRE_SRC ${ARDUINO_LIBS}/OneWire/*.cpp ${ARDUINO_LIBS}/OneWire/*.c)
	add_library(arduino_onewire STATIC ${ARDUINO_LIB_ONEWIRE_SRC})
	target_link_libraries(arduino_onewire arduino_core)

	file(GLOB ARDUINO_LIB_TIME_SRC ${ARDUINO_LIBS}/Time/*.cpp ${ARDUINO_LIBS}/Time/*.c)
	add_library(arduino_time STATIC ${ARDUINO_LIB_TIME_SRC})
	target_link_libraries(arduino_time arduino_core)

	file(GLOB ARDUINO_LIB_TMPL_PARS_SRC ${ARDUINO_LIBS}/TemplateParser/*.cpp ${ARDUINO_LIBS}/TemplateParser/*.c)
	add_library(arduino_template_parser STATIC ${ARDUINO_LIB_TMPL_PARS_SRC})

	file(GLOB ARDUINO_LIB_XIVELY_SRC ${ARDUINO_LIBS}/Xively/*.cpp ${ARDUINO_LIBS}/Xively/*.c)
	add_library(arduino_xively STATIC ${ARDUINO_LIB_XIVELY_SRC})

	file(GLOB ARDUINO_LIB_HTTPCLIENT_SRC ${ARDUINO_LIBS}/HttpClient/*.cpp ${ARDUINO_LIBS}/HttpClient/*.c)
	add_library(arduino_httpclient STATIC ${ARDUINO_LIB_HTTPCLIENT_SRC})

	file(GLOB AQADUINO_SRC
			  ${CMAKE_CURRENT_SOURCE_DIR}/Actuators/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Actuators/*.c
			  ${CMAKE_CURRENT_SOURCE_DIR}/Controller/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Controller/*.c
			  ${CMAKE_CURRENT_SOURCE_DIR}/Framework/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Framework/*.c
			  ${CMAKE_CURRENT_SOURCE_DIR}/Sensors/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Sensors/*.c
			  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
	add_library(aquaduino ${AQADUINO_SRC})
	target_link_libraries(aquaduino
						  arduino_core
						  arduino_spi
						  arduino_ethernet
						  arduino_sd
						  arduino_onewire
						  arduino_time
						  arduino_template_parser
						  arduino_xively
						  arduino_httpclient)
endif()
