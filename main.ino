#include <SPI.h>
#include <Wire.h>
#include "RF24.h"
#include "sensor.h"

const byte MPU6050_ADDRESS = 0x68; // I2C address of the MPU-6050 device

const rf24_pa_dbm_e RADIO_POWER_LEVEL = RF24_PA_LOW;  // default is RF24_PA_MAX
const rf24_datarate_e RADIO_DATA_RATE = RF24_1MBPS; //RF24_250KBPS;

RF24 radio(7, 8);  // set up nRF24L01 radio on SPI bus plus pins 7 & 8

const unsigned long SEND_DELAY_MS = 500;

enum radio_role_e {
  sender,
  receiver
};

const radio_role_e radio_role = RADIO_ROLE;

sensor_val_t read_sensor()
{
  sensor_val_t sensor_val;

  // read values from the MPU-6050 device
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(0x3B);  // start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDRESS, 14, true);  // request a total of 14 registers

  int temp;
  sensor_val.accel_X = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  sensor_val.accel_Y = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  sensor_val.accel_Z = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  temp = Wire.read() << 8 | Wire.read();                // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  sensor_val.gyro_X = Wire.read() << 8 | Wire.read();   // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  sensor_val.gyro_Y = Wire.read() << 8 | Wire.read();   // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  sensor_val.gyro_Z = Wire.read() << 8 | Wire.read();   // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  sensor_val.temp = (float)temp / 340.00 + 36.53;

  return sensor_val;
}

void setup()
{
  Serial.begin(115200);

  if (radio_role == sender) {
    Wire.begin();
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero to wake up the MPU-6050
    Wire.endTransmission(true);
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
    radio.stopListening();

    sensor_val = read_sensor();
    Serial.print(F("sending sensor value: "));
    Serial.print(sensor_val.temp);
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
      Serial.println(sensor_val.temp);
    }
  }
}
