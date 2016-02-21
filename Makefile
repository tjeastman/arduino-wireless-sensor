BOARD_TAG = uno
ARDUINO_PORT ?= /dev/ttyUSB0
ARDUINO_LIBS = SPI Wire RF24
USER_LIB_PATH = libraries
RADIO_ROLE ?= sender
CPPFLAGS = -DRADIO_ROLE=$(RADIO_ROLE)
include /usr/share/arduino/Arduino.mk
