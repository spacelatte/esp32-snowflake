#!/usr/bin/env make -f

CC      := cc
CFLAGS  := -Wall
TARGETS := convert.exe screenshot.exe
ARDUINO := open -WFnjgb cc.arduino.Arduino --args
ARDUINO := /Volumes/data/Applications/Arduino.app/Contents/MacOS/Arduino
BUILD   := $(wildcard /Volumes/tempramdisk*)/build
BOARD   := esp32:esp32:esp32thing
PORT    := /dev/cu.SLAB_USBtoUART
BAUD    := 115200
ARGS    := --verbose --preserve-temp-files \
           --pref upload.speed=921600 \
           --pref build.path=$(BUILD) \
           --pref build.flash_freq=80m \

default: u m
	# nothing

images: $(addsuffix .h, 00 01 02 03 04 05 06 07 08 09 10 11 12)

$(BUILD):
	mkdir -p $(BUILD)

wait:
	until test -e $(PORT); do echo "waiting for: $(PORT)"; sleep 1; done

c compile: $(BUILD) images
	$(ARDUINO) $(ARGS) --port $(PORT) --board $(BOARD) --verify $(wildcard *.ino)

u install: $(BUILD) wait images
	$(ARDUINO) $(ARGS) --port $(PORT) --board $(BOARD) --upload $(wildcard *.ino)

m monitor: wait
	trap 'kill %1' EXIT 0 1 2 3 6 9 15; cat $(PORT) & stty -f $(PORT) raw $(BAUD); wait

clean:
	rm -rf $(BUILD) $(TARGETS) ??.h ss.bmp

ss.bmp: screenshot.exe
	timeout 3s cat $(PORT) | ./$< > $@ & \
	stty -f $(PORT) raw $(BAUD); \
	echo | tee $(PORT); \
	true

%.h: %.bmp convert.exe
	cat $< | ./convert.exe > $@

%.exe: %.cc
	$(CC) $(CFLAGS) -o $@ $<
