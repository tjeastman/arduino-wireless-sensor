#!/bin/sh

export ARDUINO_PORT1=/dev/ttyACM0
export ARDUINO_PORT2=/dev/ttyUSB0

make clean
make RADIO_ROLE=receiver && make upload ARDUINO_PORT=$ARDUINO_PORT1
make clean
make RADIO_ROLE=sender && make upload ARDUINO_PORT=$ARDUINO_PORT2
make monitor ARDUINO_PORT=/dev/ttyUSB0
