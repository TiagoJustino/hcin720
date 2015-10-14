#include "MMA8452Q.h"
#include "pitches.h"
#include "nyancat.h"
#include <math.h>
#include <system_cloud.h>

#define PI 3.14159265
#define ACCEL_QUEUE_SIZE 16

MMA8452Q accel;
int ledFrequency = 2000;
volatile bool nyan = false;
volatile bool eventSound = false;
volatile byte pl = LOCKOUT, prevPl = LOCKOUT;
int buzzerPin = D2;
int frequencies[] = {2000, 2400, 2800, 3200, 3600, 4000};
int nextNyanSoundIndex;
int nextEventSoundIndex;
int accelQueuePos = 0;
struct {
  int x = 0, y = 0, z = 0;
} previousAccel, accelQueue[ACCEL_QUEUE_SIZE];
enum MODE {ORIENTATION, MUSICAL, SILENT};
int mode = ORIENTATION;
int previousCloudEvent = 0;
int currentCloudEvent = 0;

unsigned long now;
unsigned long readAccelCounter;
unsigned long sendDataCounter;
unsigned long nextNyanSoundCounter;
unsigned long ledSignalingCounter;
unsigned long nextEventSoundCounter;
unsigned long ledCounter;

double accel_angle_x;
double accel_angle_y;
int accel_center_x;
int accel_center_y;
int accel_center_z;

const float alpha = 0.5;
double fXg = 0;
double fYg = 0;
double fZg = 0;
double roll;
double pitch;

const uint32_t VIBGYOR_Colors[] = {
    0xEE82EE, 0x4B0082, 0x0000FF, 0x00FF00, 0xFFFF00, 0xFFA500, 0xFF0000
};
const int VIBGYOR_Size = sizeof (VIBGYOR_Colors) / sizeof (uint32_t);
int VIBGYOR_Index;

int eventMelodySize = 8;
int eventMelody[] = {NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_G7, NOTE_A7, NOTE_B7, NOTE_C8};

void eventHandler(const char *event, const char *data) {
  if(data) {
    previousCloudEvent = currentCloudEvent;
    currentCloudEvent = atoi(data);
    nextEventSoundCounter = now;
    nextEventSoundIndex = 0;
    eventSound = true;
  }
}

int setMode(String modeArg) {
  if(modeArg == "silent") {
    mode = SILENT;
    return 0;
  }
  else if(modeArg == "orientation") {
    mode = ORIENTATION;
    return 0;
  }
  else if(modeArg == "musical") {
    mode = MUSICAL;
    return 0;
  }
  return -1;
}

void setup() {
  accel.init();
  Serial.begin(9600);
  pinMode(D7, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Particle.subscribe("cloudevent", eventHandler, MY_DEVICES);
  Particle.function("setmode", setMode);
}

String getOrientation(byte _pl) {
  switch (_pl)
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

void myTone(int frequency, int duration) {
  if(mode != SILENT) {
    tone(buzzerPin, frequency, duration);
  }
}

void playSound(byte _pl) {
  int duration = 150;
  switch (_pl)
  {
    case PORTRAIT_U:
      myTone(frequencies[0], duration);
      return;
    case PORTRAIT_D:
      myTone(frequencies[1], duration);
      return;
    case LANDSCAPE_R:
      myTone(frequencies[2], duration);
      return;
    case LANDSCAPE_L:
      myTone(frequencies[3], duration);
      return;
    case LOCKOUT:
      myTone(frequencies[4], duration);
      return;
  }
  myTone(frequencies[5], duration);
}

void sendData() {
  char str[64];
  int light;
  String orientation;

  light = analogRead(A0);
  pl = accel.readPL();
  orientation = getOrientation(pl);

  sprintf(str, "%6d : %6d : %6d : %6d : %16s : %1d : %4.0f : %4.0f", accel.x, accel.y, accel.z, light, orientation.c_str(), nyan, pitch, roll);
  Serial.println(str);
  Particle.publish("accelData", str);
}

void queueAccelRead() {
  accelQueue[accelQueuePos].x = accel.x;
  accelQueue[accelQueuePos].y = accel.y;
  accelQueue[accelQueuePos].z = accel.z;
  accelQueuePos++;
  accelQueuePos = accelQueuePos == ACCEL_QUEUE_SIZE ? 0 : accelQueuePos;
}

void readAccel() {
  int tap;

  accel.read();
  queueAccelRead();
  tap = accel.readTap();

  if(tap > 0) {
    nyan = !nyan;
    if(nyan) {
      ledFrequency = 250;
      nextNyanSoundIndex = 0;
      nextNyanSoundCounter = now - 1;
      LED_Signaling_Start();
      ledSignalingCounter = now;
    } else {
      ledFrequency = 2000;
      LED_Signaling_Stop();
    }
  }
}

void checkEventSound() {
  if(now >= nextEventSoundCounter) {
    toggleLed();
    int index = previousCloudEvent < currentCloudEvent ? nextEventSoundIndex :
      (currentCloudEvent < previousCloudEvent ? eventMelodySize - 1 - nextEventSoundIndex : 3);
    int thisNote = eventMelody[index];
    int noteDuration = 150;
    myTone(thisNote, noteDuration);
    nextEventSoundCounter += noteDuration;
    nextEventSoundIndex += 1;
    if(nextEventSoundIndex == eventMelodySize) {
      eventSound = false;
    }
  }
}

void checkNyanSound() {
  if(now >= nextNyanSoundCounter) {
    int thisNote = nyanMelody[nextNyanSoundIndex];
    int noteDuration = 880 / nyanNoteDurations[nextNyanSoundIndex];
    int pauseBetweenNotes = noteDuration * 1.30;
    myTone(thisNote, noteDuration);
    nextNyanSoundCounter += noteDuration + pauseBetweenNotes;
    nextNyanSoundIndex += 1;
    if(nextNyanSoundIndex == melodyLength) {
      nextNyanSoundIndex = 26;
    }
  }
}

void checkSound() {
  if(eventSound) {
    checkEventSound();
  } else {
    if (nyan) {
      checkNyanSound();
    }
  }
}

void toggleLed() {
  digitalWrite(D7, !digitalRead(D7));
}

void checkOrientation() {
  if(prevPl != pl) {
    prevPl = pl;
    if(!eventSound && !nyan) {
      playSound(pl);
    }
  }
}

void checkMovement() {
  if(!nyan) {
    int diff = abs(accel.x - previousAccel.x);
    diff += abs(accel.y - previousAccel.y);
    diff += abs(accel.z - previousAccel.z);

    myTone(diff, 100);
  }
}

// copied from system/src/system_cloud_internal.cpp (photon firmware code)
void LED_Signaling_Override(void)
{
  LED_SetSignalingColor(VIBGYOR_Colors[VIBGYOR_Index]);
  LED_On(LED_RGB);

  ++VIBGYOR_Index;
  if (VIBGYOR_Index >= VIBGYOR_Size)
  {
    VIBGYOR_Index = 0;
  }
}

int getXAverage() {
  int sum = 0;
  for(int i = 0; i < ACCEL_QUEUE_SIZE; i++) {
    sum += accelQueue[i].x;
  }
  return sum / ACCEL_QUEUE_SIZE;
}

int getYAverage() {
  int sum = 0;
  for(int i = 0; i < ACCEL_QUEUE_SIZE; i++) {
    sum += accelQueue[i].y;
  }
  return sum / ACCEL_QUEUE_SIZE;
}

int getZAverage() {
  int sum = 0;
  for(int i = 0; i < ACCEL_QUEUE_SIZE; i++) {
    sum += accelQueue[i].z;
  }
  return sum / ACCEL_QUEUE_SIZE;
}

// copied from http://www.hobbytronics.co.uk/accelerometer-info
void calc_xy_angles(){
  int x = getXAverage();
  int y = getYAverage();
  int z = getZAverage();
  accel_angle_x = atan(x / sqrt(y * y + z * z)) * 180 / PI;
  accel_angle_y = atan(y / sqrt(x * x + z * z)) * 180 / PI;
}


// copied from http://theccontinuum.com/2012/09/24/arduino-imu-pitch-roll-from-accelerometer/
void calc_xy_angles2(int Xg, int Yg, int Zg){
  //Low Pass Filter
  fXg = Xg * alpha + (fXg * (1.0 - alpha));
  fYg = Yg * alpha + (fYg * (1.0 - alpha));
  fZg = Zg * alpha + (fZg * (1.0 - alpha));

  //Roll & Pitch Equations
  roll  = (atan2(-fYg, fZg)*180.0)/M_PI;
  pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI;
}

void loop() {
  now = millis();
  if(nyan && ( now - ledSignalingCounter >= 100)) {
    ledSignalingCounter = now;
    LED_Signaling_Override();
  }
  if(now - readAccelCounter >= 100) {
    readAccelCounter = now;
    readAccel();
    calc_xy_angles2(accel.x, accel.y, accel.z);
    switch(mode) {
      case ORIENTATION:
        checkOrientation();
        break;
      case MUSICAL:
        checkMovement();
    }
    previousAccel.x = accel.x;
    previousAccel.y = accel.y;
    previousAccel.z = accel.z;
  }

  if(now - sendDataCounter >= 1000) {
    sendDataCounter = now;
    sendData();
  }

  if(now - ledCounter >= ledFrequency) {
    ledCounter = now;
    toggleLed();
  }

  checkSound();
}
