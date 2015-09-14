#include "mma8452.h"
#include <i2c_hal.h>
#include <spark_wiring.h>
#include <spark_wiring_i2c.h>

MMA8452::MMA8452(int a) : addr(a) {
  // setup stuff.
}

void MMA8452::begin() {
  Wire.setSpeed(CLOCK_SPEED_100KHZ);
  Wire.begin();
  standby();
  scale(0);
  rate(0);
  start();
}

void MMA8452::read(int &x, int &y, int &z) {
  byte buffer[6];
  in(0x01,buffer,6); // read 6 bytes in MSB order starting with X
  x = (buffer[0] << 8 | buffer[1]) >> 4;  // 12 bits MSB
  y = (buffer[2] << 8 | buffer[3]) >> 4;
  z = (buffer[4] << 8 | buffer[5]) >> 4;
}

void MMA8452::scale(int s){
  byte ctrl;
  in(0x0e,&ctrl,1);   // 0x0e is xyz config register
  ctrl &= 0xfc;       // mask out scale
  ctrl |= s;          // 0 = 2G, 1 = 4G, 2 = 8G
  out(0x0e,&ctrl,1);
}

void MMA8452::rate(int r){
  byte ctrl;
  in(0x2a,&ctrl,1);   // 0x2a is CTRL1 register
  ctrl &= 0xfc;       // mask out the rate bits
  ctrl |= (r << 3);   // 0 800, 1 400, 2 200, 3 100, 4 50, 5 12, 6 6, 7 1
  out(0x2a,&ctrl,1);
}

bool MMA8452::available(){
  byte a;
  in(0,&a,1);               // 0 is STATUS register.
  return ((a & 0x08) >> 3); // bit 3 is the data available register
}

// private methods
void MMA8452::start() {
  byte ctrl;
  in(0x2a,&ctrl,1);
  ctrl |= 1;
  out(0x2a,&ctrl,1);
}


void MMA8452::standby() {
  byte ctrl;
  in(0x2a,&ctrl,1);       // 0x2a is CTRL1 register
  ctrl = ctrl & ~(0x01);  // set low bit 0
  out(0x2a,&ctrl,1);      // update CTRL1
}

void MMA8452::out(int reg, byte* data, int l) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(data,l);
  Wire.endTransmission();
}

void MMA8452::in(int reg, byte* data, int l) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(addr,l);
  while (Wire.available() < l);
  for (int i = 0; i < l; ++i) {
    data[i] = Wire.read();
  }
}
