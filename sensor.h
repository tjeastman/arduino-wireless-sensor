#ifndef SENSOR_H
#define SENSOR_H

struct sensor_state_t {
  int accel_X;
  int accel_Y;
  int accel_Z;
  int gyro_X;
  int gyro_Y;
  int gyro_Z;
  float temp;
};

class Sensor {
  sensor_state_t state;
 public:
  Sensor();
  const sensor_state_t * get_state();
  bool update_state();
};

#endif
