#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <Servo.h> 

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myStepper = AFMS.getStepper(200, 2);
Servo servo;

void setup() {
  Serial.begin(9600);
  Serial.println("MMMMotor party!");

  AFMS.begin();
  servo.attach(10);
  myStepper->setSpeed(255);
}

/*
void loop() {
  //myStepper->step(100, FORWARD, SINGLE); 
  servo.write(0);
  delay(500);
  myStepper->step(100, FORWARD, SINGLE); 
  servo.write(90);
  delay(500);
  myStepper->step(100, FORWARD, SINGLE); 
  servo.write(180);
  delay(500);
  myStepper->step(100, FORWARD, SINGLE); 
  servo.write(-90);
  delay(500);
}
*/

int n = 0;

void loop() {
  int a, b, c;
  if (Serial.available() > 0) {
    a = Serial.read();
    if (a == 'a') {
      myStepper->step(1000, FORWARD, SINGLE);
    }
    if (a == 's') {
      myStepper->step(1000, BACKWARD, SINGLE);
    }

  }
}

