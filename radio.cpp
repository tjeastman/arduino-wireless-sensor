#include <SPI.h>
#include "RF24.h"
#include "radio.h"

const rf24_pa_dbm_e RADIO_POWER_LEVEL = RF24_PA_LOW;  // default is RF24_PA_MAX
const rf24_datarate_e RADIO_DATA_RATE = RF24_1MBPS; //RF24_250KBPS;

Radio::Radio()
{
  // set up nRF24L01+ radio on SPI bus plus pins 7 and 8
  device = new RF24(7, 8);

  device->begin();

  device->setPALevel(RADIO_POWER_LEVEL);
  device->setDataRate(RADIO_DATA_RATE);
}

bool Radio::available()
{
  return device->available();
}

bool Radio::write(const void *buffer, size_t len)
{
  device->stopListening();
  bool success = device->write(buffer, len);
  device->startListening();
  return success;
}

void Radio::read(void *buffer, size_t len)
{
  device->read(buffer, len);
}

void Radio::listen()
{
  device->startListening();
}

void Radio::open_pipe(byte *address1, byte *address2)
{
  device->openWritingPipe(address1);
  device->openReadingPipe(1, address2);
}
