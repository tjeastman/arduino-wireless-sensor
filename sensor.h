struct sensor_val_t {
  int accel_X;
  int accel_Y;
  int accel_Z;
  int gyro_X;
  int gyro_Y;
  int gyro_Z;
  float temp;
};

class Sensor {
  sensor_val_t state;
 public:
  Sensor();
  const sensor_val_t * get_state();
  bool update_state();
};
