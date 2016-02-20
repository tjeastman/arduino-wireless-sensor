BOARD_TAG = uno
ARDUINO_PORT ?= /dev/ttyACM0
ARDUINO_LIBS = SPI RF24
USER_LIB_PATH = libraries
CPPFLAGS = -DRADIO_ROLE=$(RADIO_ROLE)
include /usr/share/arduino/Arduino.mk
