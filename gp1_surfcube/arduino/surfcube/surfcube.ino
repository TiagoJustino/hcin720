#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <Servo.h> 

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myStepper = AFMS.getStepper(200, 2);
Servo servo;

int now;
int serialNextCheck;
int stepperDirection;
int stepperSteps;
int stepperNextCheck;
int servoUserTargetAngle;
int servoTargetAngle;
int servoCurrentAngle;
int servoNextCheck;
int servoFrequency;

void setup() {
  Serial.begin(9600);
  Serial.println("MMMMotor party!");

  AFMS.begin();
  servo.attach(10);
  myStepper->setSpeed(255);

  servoCurrentAngle = 120;
  servo.write(servoCurrentAngle);
}

void checkSerial() {
  String command;
  int val;
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    while(Serial.available() == 0) {}
    val = Serial.parseInt();
    if (command == "forward") {
      stepperDirection = FORWARD;
      stepperSteps = val;
    } else if (command == "backward") {
      stepperDirection = BACKWARD;
      stepperSteps = val;
    } else if (command == "servo") {
      servoTargetAngle = val % 121;
    } else if (command == "frequency") {
      servoFrequency = val;
    } else {
      Serial.print("Command not understood! ");
      Serial.print("command = [");
      Serial.print(command);
      Serial.print("]; ");
    }
    while(Serial.available()) {
      Serial.read();
    }
  }
  serialNextCheck = now + 1;
}

void checkServo() {
  if(servoCurrentAngle != servoTargetAngle) {
    if(servoCurrentAngle < servoTargetAngle) {
      ++servoCurrentAngle;
    }
    if(servoCurrentAngle > servoTargetAngle) {
      --servoCurrentAngle;
    }
    servo.write(servoCurrentAngle);
    servoNextCheck = now + 1;
    if(servoCurrentAngle == servoTargetAngle) {
      if(servoTargetAngle == 120) {
        servoNextCheck = now + servoFrequency;
      }
      servoTargetAngle = servoTargetAngle == 120 ? servoUserTargetAngle : 120;
    }
  }
}

void checkStepper() {
  if(stepperSteps) {
    myStepper->step(1, stepperDirection, SINGLE);
    --stepperSteps;
    stepperNextCheck = now + 1;
  }
}

void loop() {
  now = millis();
  if(now >= serialNextCheck) {
    checkSerial();
  }
  if(now >= servoNextCheck) {
    checkServo();
  }
  if(now >= stepperNextCheck) {
    checkStepper();
  }
}

