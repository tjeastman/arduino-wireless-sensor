#include <SPI.h>
#include "RF24.h"

rf24_pa_dbm_e POWER_LEVEL = RF24_PA_LOW;  // default is RF24_PA_MAX
/* rf24_datarate_e DATA_RATE = RF24_250KBPS; */

RF24 radio(7,8);  // set up nRF24L01 radio on SPI bus plus pins 7 & 8

unsigned long SEND_DELAY_MS = 1000;

enum radio_role_e {
  sender,  // 0
  receiver // 1
};

radio_role_e radio_role = RADIO_ROLE;

typedef unsigned long sensor_val_t;

unsigned long read_sensor()
{
  return micros();
}

void setup()
{
  Serial.begin(115200);

  radio.begin();

  /* Serial.println(F("before")); */
  /* Serial.print(F("power level: ")); */
  /* Serial.println(radio.getPALevel()); */
  /* Serial.print(F("data rate: ")); */
  /* Serial.println(radio.getDataRate()); */
  /* Serial.println(); */

  radio.setPALevel(POWER_LEVEL);

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
    radio.stopListening();

    sensor_val = read_sensor();
    Serial.print(F("sending sensor value: "));
    Serial.print(sensor_val);
    Serial.print(F("..."));

    if (!radio.write(&sensor_val, sizeof(sensor_val_t))){
      Serial.println(F("failed"));
    } else {
      Serial.println(F("succeeded"));
    }

    radio.startListening();

    delay(SEND_DELAY_MS);
  }

  if (radio_role == receiver) {
    if (radio.available()) {

      while (radio.available()) { // while there is data ready
        radio.read(&sensor_val, sizeof(sensor_val_t));
      }

      Serial.print(F("received sensor value: "));
      Serial.println(sensor_val);
    }
  }
}
