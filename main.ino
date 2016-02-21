#include "RF24.h"
#include "radio.h"
#include "sensor.h"

Radio *radio;

const unsigned long SEND_DELAY_MS = 500;

enum radio_role_e {
  sender,
  receiver
};

const radio_role_e radio_role = RADIO_ROLE;

Sensor *sensor;

void setup()
{
  Serial.begin(115200);

  if (radio_role == sender) {
    sensor = new Sensor();
  }

  radio = new Radio();

  // open pipes on each radio for reading and writing with opposite addresses
  byte address1[] = "node1";
  byte address2[] = "node2";
  if (radio_role == sender) {
    radio->open_pipe(address1, address2);
  } else {
    radio->open_pipe(address2, address1);
  }

  radio->listen();
}

void loop()
{
  if (radio_role == sender)  {
    if (!sensor->update_state()) {
      Serial.println(F("failed to update state"));
    }
    const sensor_state_t * sensor_state = sensor->get_state();

    bool success = radio->write(sensor_state, sizeof(sensor_state_t));

    Serial.print(F("sent sensor value: "));
    Serial.print(sensor_state->temp);
    Serial.print(F("..."));
    Serial.println(success ? F("succeeded") : F("failed"));

    delay(SEND_DELAY_MS);
  }

  if (radio_role == receiver) {
    sensor_state_t sensor_state;
    if (radio->available()) {

      while (radio->available()) { // while there is data ready
        radio->read(&sensor_state, sizeof(sensor_state_t));
      }

      Serial.print(F("received sensor value: "));
      Serial.println(sensor_state.temp);
    }
  }
}
