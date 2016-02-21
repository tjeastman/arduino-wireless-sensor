#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"

// I2C address of the MPU-6050 device
const int MPU6050_ADDRESS = 0x68;

Sensor::Sensor()
{
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero to wake up the MPU-6050
  Wire.endTransmission(true);
}

bool Sensor::update_state()
{
  // read values from the MPU-6050 device
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(0x3B);  // start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDRESS, 14, true);  // request a total of 14 registers
  if (Wire.available() != 14) {
    return false;
  }

  int temp;
  state.accel_X = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  state.accel_Y = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  state.accel_Z = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  temp = Wire.read() << 8 | Wire.read();           // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  state.gyro_X = Wire.read() << 8 | Wire.read();   // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  state.gyro_Y = Wire.read() << 8 | Wire.read();   // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  state.gyro_Z = Wire.read() << 8 | Wire.read();   // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // convert temperature into degrees C
  state.temp = (float)temp / 340.00 + 36.53;
  return true;
}

const sensor_val_t * Sensor::get_state()
{
  return &state;
}
