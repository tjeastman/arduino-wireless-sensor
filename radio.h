#ifndef RADIO_H
#define RADIO_H

class Radio {
  RF24 *device;
 public:
  Radio();
  bool available();
  bool write(const void *buffer, size_t len);
  void read(void *buffer, size_t len);
  void listen();
  void open_pipe(byte *address1, byte *address2);
};

#endif
