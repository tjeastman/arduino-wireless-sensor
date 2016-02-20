#!/bin/sh

make clean
make RADIO_ROLE=receiver && make upload ARDUINO_PORT=/dev/ttyACM0
make clean
make RADIO_ROLE=sender && make upload ARDUINO_PORT=/dev/ttyUSB0
make monitor ARDUINO_PORT=/dev/ttyUSB0
