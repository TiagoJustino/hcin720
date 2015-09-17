#include "MMA8452Q.h"
#include "pitches.h"
#include "nyancat.h"

MMA8452Q accel;
int ledFrequency = 1000;
volatile bool nyan = false;
int eventSoundCounter = 0;
bool eventSound = false;
byte pl = LOCKOUT, prevPl = LOCKOUT;
int buzzerPin = D2;
int frequencies[] = {2000, 2400, 2800, 3200, 3600, 4000};
int nextNyanSoundIndex;
unsigned long nextNyanSoundTime;
unsigned long now;

int eventHandler(String data) {
  eventSoundCounter = 0;
  eventSound = true;
  return 0;
}

void setup() {
  accel.init();
  Serial.begin(9600);
  pinMode(D7, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Spark.function("cloudevent", eventHandler);
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

void playSound(byte _pl) {
  int duration = 150;
  switch (_pl)
  {
    case PORTRAIT_U:
      tone(buzzerPin, frequencies[0], duration);
      return;
    case PORTRAIT_D:
      tone(buzzerPin, frequencies[1], duration);
      return;
    case LANDSCAPE_R:
      tone(buzzerPin, frequencies[2], duration);
      return;
    case LANDSCAPE_L:
      tone(buzzerPin, frequencies[3], duration);
      return;
    case LOCKOUT:
      tone(buzzerPin, frequencies[4], duration);
      return;
  }
  tone(buzzerPin, frequencies[5], duration);
}

int counter = 0;
int nyanSoundCounter = 0;

void playSong(int frequency) {
  if(frequency > 0) {
    tone(buzzerPin, frequency, 150);
  } else {
    noTone(buzzerPin);
  }
}

int eventMelodySize = 13;
int eventMelody[] = {NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7, NOTE_C8};

void playEventSound() {
  if(eventSoundCounter == eventMelodySize) {
    eventSound = false;
    nyan = false;
    eventSoundCounter = 0;
  } else {
    playSong(eventMelody[eventSoundCounter]);
    eventSoundCounter++;
  }
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
      nextNyanSoundTime = now - 1;
    } else {
      ledFrequency = 1000;
    }
  }
}

void checkEventSound() {
}

void checkNyanSound() {
  if(now > nextNyanSoundTime) {
    int thisNote = nyanMelody[nextNyanSoundIndex];
    int noteDuration = 1000 / nyanNoteDurations[nextNyanSoundIndex];
    int pauseBetweenNotes = noteDuration * 1.30;
    tone(buzzerPin, thisNote, noteDuration);
    nextNyanSoundIndex += 1;
    nextNyanSoundTime += noteDuration + pauseBetweenNotes;
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

void loop() {
  now = millis();
  if(now % 250 == 0) {
    readAccel();
    checkOrientation();
  }

  if(now % 1000 == 0) {
    sendData();
  }

  if(now % ledFrequency == 0) {
    toggleLed();
  }

  if(nyan) {
    checkSound();
  }
}
