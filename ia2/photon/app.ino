// #include <Wire.h>
#include "MMA8452Q.h"

MMA8452Q accel;
bool nyan = false;

void setup() {
  accel.init();
  Serial.begin(9600);
  pinMode(D7, OUTPUT);
}

String getOrientation(byte pl) {
  switch (pl)
  {
    case PORTRAIT_U:
      return "Portrait Up";
    case PORTRAIT_D:
      return "Portrait Down";
    case LANDSCAPE_R:
      return "Landscape Right";
    case LANDSCAPE_L:
      return "Landscape Left";
    case LOCKOUT:
      return "Flat";
  }
  return "Unknown";
}

void switchNyan(bool on) {
  if (on) {
    LED_Signaling_Start();
    LED_Spark_Signal = 1;
  } else {
    LED_Signaling_Stop();
    LED_Spark_Signal = 0;
  }
}

void loop() {
  char str[64];
  int light;
  int tap;
  String orientation;

  accel.read();
  light = analogRead(A0);
  orientation = getOrientation(accel.readPL());
  tap = accel.readTap();

  sprintf(str, "%d:%d:%d:%d:%s:%d", accel.x, accel.y, accel.z, light, orientation.c_str(), tap);
  Serial.println(str);
  Spark.publish("accelData", str);
  delay(1000);
}
