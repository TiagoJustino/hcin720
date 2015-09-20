#include "MMA8452Q.h"
#include "pitches.h"
#include "nyancat.h"
#include <system_cloud.h>

MMA8452Q accel;
int ledFrequency = 2000;
volatile bool nyan = false;
volatile bool eventSound = false;
volatile byte pl = LOCKOUT, prevPl = LOCKOUT;
int buzzerPin = D2;
int frequencies[] = {2000, 2400, 2800, 3200, 3600, 4000};
int nextNyanSoundIndex;
int nextEventSoundIndex;
struct {
  int x = 0, y = 0, z = 0;
} previousAccel;
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
  Serial.println(modeArg);
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
  Spark.subscribe("cloudevent", eventHandler, MY_DEVICES);
  Spark.function("setmode", setMode);
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

  sprintf(str, "%d:%d:%d:%d:%s:%d", accel.x, accel.y, accel.z, light, orientation.c_str(), nyan);
  Serial.println(str);
  Spark.publish("accelData", str);
}

void readAccel() {
  int tap;

  accel.read();
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

void loop() {
  now = millis();
  if(nyan && ( now - ledSignalingCounter >= 100)) {
    ledSignalingCounter = now;
    LED_Signaling_Override();
  }
  if(now - readAccelCounter >= 100) {
    readAccelCounter = now;
    readAccel();
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
