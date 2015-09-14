#include "mma8452.h"

MMA8452 accel;

void setup() {
  accel.begin();
  Serial.begin(9600);
  pinMode(D7, OUTPUT);
}

void loop() {
  char str[64];
  int x, y, z, light;

  accel.read(x,y,z);
  light = analogRead(A0);

  sprintf(str, "%d:%d:%d:%d", x, y, z, light);

  Serial.println(str);

  digitalWrite(D7, HIGH);
  delay(1000);
  digitalWrite(D7, LOW);
  delay(1000);
}
