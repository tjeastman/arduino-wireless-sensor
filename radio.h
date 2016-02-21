#ifndef RADIO_H
#define RADIO_H

class Radio {
  RF24 *device;
 public:
  Radio();
  bool available();
  bool write(const void *bytes, size_t num_bytes);
  void read(void *bytes, size_t num_bytes);
  void listen();
  void open_pipe(byte *address1, byte *address2);
};

#endif
