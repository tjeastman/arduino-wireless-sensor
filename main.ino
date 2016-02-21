#include <SPI.h>
#include "RF24.h"
#include "sensor.h"

const rf24_pa_dbm_e RADIO_POWER_LEVEL = RF24_PA_LOW;  // default is RF24_PA_MAX
const rf24_datarate_e RADIO_DATA_RATE = RF24_1MBPS; //RF24_250KBPS;

RF24 radio(7, 8);  // set up nRF24L01 radio on SPI bus plus pins 7 & 8

const unsigned long SEND_DELAY_MS = 500;

enum radio_role_e {
  sender,
  receiver
};

const radio_role_e radio_role = RADIO_ROLE;

void setup()
{
  Serial.begin(115200);

  if (radio_role == sender) {
    initialize_sensor();
  }

  radio.begin();

  /* Serial.println(F("before")); */
  /* Serial.print(F("power level: ")); */
  /* Serial.println(radio.getPALevel()); */
  /* Serial.print(F("data rate: ")); */
  /* Serial.println(radio.getDataRate()); */
  /* Serial.println(); */

  radio.setPALevel(RADIO_POWER_LEVEL);
  radio.setDataRate(RADIO_DATA_RATE);

  /* Serial.println(F("after")); */
  /* Serial.print(F("power level: ")); */
  /* Serial.println(radio.getPALevel()); */
  /* Serial.print(F("data rate: ")); */
  /* Serial.println(radio.getDataRate()); */
  /* Serial.println(); */

  // open pipes on each radio for reading and writing with opposite addresses
  byte address1[] = "node1";
  byte address2[] = "node2";
  if (radio_role == sender) {
    radio.openWritingPipe(address2);
    radio.openReadingPipe(1, address1);
  } else {
    radio.openWritingPipe(address1);
    radio.openReadingPipe(1, address2);
  }

  radio.startListening();
}

void loop()
{
  sensor_val_t sensor_val;

  if (radio_role == sender)  {
    sensor_val = read_sensor();

    radio.stopListening();
    bool success = radio.write(&sensor_val, sizeof(sensor_val_t));
    radio.startListening();

    Serial.print(F("sending sensor value: "));
    Serial.print(sensor_val.temp);
    Serial.print(F("..."));
    Serial.println(success ? F("succeeded") : F("failed"));

    delay(SEND_DELAY_MS);
  }

  if (radio_role == receiver) {
    if (radio.available()) {

      while (radio.available()) { // while there is data ready
        radio.read(&sensor_val, sizeof(sensor_val_t));
      }

      Serial.print(F("received sensor value: "));
      Serial.println(sensor_val.temp);
    }
  }
}
