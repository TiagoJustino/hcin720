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
int angle = 120;

void loop() {
  String command;
  int val;
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    while(Serial.available() == 0) {}
    val = Serial.parseInt();
    if (command == "forward") {
      Serial.print("Pumping water in! ");
      myStepper->step(val, FORWARD, SINGLE); 
    } else if (command == "backward") {
      Serial.print("Pumping water out! ");
      myStepper->step(val, BACKWARD, SINGLE); 
    } else if (command == "servo") {
      Serial.print("Making waves! ");
      angle = val;
    } else {
      Serial.print("Command not understood! ");
      Serial.print("command = [");
      Serial.print(command);
      Serial.print("]; ");
    }
    Serial.print("val = [");
    Serial.print(val);
    Serial.println("]");
    while(Serial.available()) {
      Serial.read();
    }

  }
  servo.write(angle);
  delay(500);
  servo.write(120);
  delay(500);
}

