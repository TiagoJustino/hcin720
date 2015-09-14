// mma8452.h
//
// copyright 2015 wot.io Inc. <dev@wot.io>
//



typedef unsigned char byte;

class MMA8452 {
public:
  MMA8452(int a = 0x1d); // I2C address
  void begin();
  void read(int &x, int &y, int &z);
  void scale(int s); // 2 4 or 8
  void rate(int r); // 0 = 800, 1 = 400, 2 = 200, 100, 50, 12, 6, 1
  bool available();

private:
  void start();
  void standby();
  void out(int r, byte* data, int l);
  void in(int r, byte* data, int l);

  int addr;

};
