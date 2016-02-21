// I2C address of the MPU-6050 device
//const int MPU6050_ADDRESS = 0x68;
#define MPU6050_ADDRESS 0x68

struct sensor_val_t {
  int accel_X;
  int accel_Y;
  int accel_Z;
  int gyro_X;
  int gyro_Y;
  int gyro_Z;
  float temp;
};

void initialize_sensor(void);
sensor_val_t read_sensor(void);
