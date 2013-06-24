#!/bin/sh

export ARDUINO_CC=avr-gcc
export ARDUINO_CXX=avr-g++

cmake -i -DCMAKE_BUILD_TYPE=Release -DARDUINO_SDK_PATH=/d/IDE/arduino-1.0.3 -G"MSYS Makefiles" $1