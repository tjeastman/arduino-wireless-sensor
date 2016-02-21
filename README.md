arduino-wireless-sensor
=======================

Read sensor information from an MPU-6050 and send it from one Arduino
to another using nRF24L01+ 2.4GHz radio transceivers to facilitate
wireless communication between them.

This assumes that there are two Arduino devices attached to the host
machine at ports /dev/ttyACM0 and /dev/ttyUSB0.  The port /dev/ttyUSB0
corresponds to the sending device and /dev/ttyACM0 to the receiving
device.  Modify the script deploy.sh to use different ports.

Run this command to build and upload the firmware to the two devices:
```
$ ./deploy.sh
```
When complete, it automatically connects to the serial port of the
sending device.  To view the serial output of the receiving device,
run the command
```
$ make monitor ARDUINO_PORT=/dev/ttyACM0
```
in another terminal window.
