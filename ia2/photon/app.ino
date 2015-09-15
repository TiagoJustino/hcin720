#include "MMA8452Q.h"
#include "pitches.h"

MMA8452Q accel;
bool nyan = false;
int eventSoundCounter = 0;
bool eventSound = false;
byte pl = LOCKOUT, prevPl = LOCKOUT;
int buzzerPin = D2;
int frequencies[] = {2000, 2400, 2800, 3200, 3600, 4000};

/*
 Nyan Cat
 
 Plays Nyan Cat
 example: http://www.youtube.com/watch?v=eFHfGA5e4n8
 circuit:
 * 8-ohm speaker on digital pin 8
  
 By: Bruce Helsen (geobruce) 
     Stan Draulans (purewantfun)
     

  Adapted by Tiago justino to work alongside with Photon and Accelerometer.

  original source from http://www.instructables.com/id/Nyan-Cat-on-Arduino/
  */

int introSize = 72;
int intro[] = {
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_E5,
  NOTE_E5,
  0,
  NOTE_FS5,
  0,
  NOTE_B5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_E5,
  NOTE_E5,
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_B5,
  NOTE_B5,
  0,
  NOTE_CS6,
  NOTE_CS6,
  0,
  NOTE_DS6,
  NOTE_DS6,
  0,
  NOTE_CS6,
  NOTE_CS6,
  0,
  NOTE_AS5,
  NOTE_AS5,
  0,
  NOTE_B5,
  0,
  NOTE_FS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_E5,
  NOTE_E5,
  0,
  NOTE_FS5,
  0,
  NOTE_B5,
  0,
  NOTE_CS6,
  NOTE_CS6,
  0,
  NOTE_AS5,
  NOTE_AS5,
  0,
  NOTE_B5,
  NOTE_B5,
  0,
  NOTE_CS6,
  NOTE_CS6,
  0,
  NOTE_E6,
  NOTE_E6,
  0,
  NOTE_DS6,
  NOTE_DS6,
  0,
  NOTE_E6,
  NOTE_E6,
  0,
  NOTE_B5, 
  NOTE_B5, 
  0
  };

int melodySize = 608;
  int melody[] = {
  NOTE_FS5,
  0,
  NOTE_GS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  REST,    
  REST,    
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_D5, 
  NOTE_D5, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  REST,    
  REST,    
  0,
  NOTE_B4, 
  0,
  NOTE_CS5,
  0,
  NOTE_D5, 
  0,
  NOTE_D5, 
  NOTE_D5, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_GS5,
  NOTE_GS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_DS5,
  0,
  NOTE_FS5,
  0,
  NOTE_GS5,
  NOTE_GS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_D5, 
  NOTE_D5, 
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_D5, 
  NOTE_D5, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_D5, 
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  0,
  NOTE_B4, 
  0,
  NOTE_CS5,
  0,
  NOTE_FS5,
  0,
  NOTE_GS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  REST,    
  REST,    
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_D5, 
  NOTE_D5, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  REST,    
  REST,    
  0,
  NOTE_B4, 
  0,
  NOTE_CS5,
  0,
  NOTE_D5, 
  0,
  NOTE_D5, 
  NOTE_D5, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_GS5,
  NOTE_GS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_DS5,
  0,
  NOTE_FS5,
  0,
  NOTE_GS5,
  NOTE_GS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_D5, 
  NOTE_D5, 
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_D5, 
  NOTE_D5, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_D5, 
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  0,
  NOTE_B4, 
  0,
  NOTE_CS5,
  0,
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_E5, 
  NOTE_E5, 
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_E5, 
  NOTE_E5, 
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_B4, 
  0,
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_E5, 
  NOTE_E5, 
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_DS4,
  NOTE_DS4,
  0,
  NOTE_E4, 
  NOTE_E4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_B4, 
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_AS4,
  NOTE_AS4,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_E4, 
  NOTE_E4, 
  0,
  NOTE_E5, 
  NOTE_E5, 
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_E5, 
  NOTE_E5, 
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_B4, 
  0,
  NOTE_AS4,
  0,
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_E5, 
  NOTE_E5, 
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_E5, 
  NOTE_E5, 
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_B4, 
  0,
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_E5, 
  NOTE_E5, 
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_DS4,
  NOTE_DS4,
  0,
  NOTE_E4, 
  NOTE_E4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_CS5,
  NOTE_CS5,
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_B4,   
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_AS4,
  NOTE_AS4,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_FS4,
  NOTE_FS4,
  0,
  NOTE_GS4,
  NOTE_GS4,
  0,
  NOTE_B4, 
  NOTE_B4, 
  0,
  NOTE_E5, 
  NOTE_E5, 
  0,
  NOTE_DS5,
  NOTE_DS5,
  0,
  NOTE_E5, 
  NOTE_E5, 
  0,
  NOTE_FS5,
  NOTE_FS5,
  0,
  NOTE_B4, 
  0,
  NOTE_CS5,
  0
  };

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

void playMelody() {
  for (int thisNote = 0; thisNote < 216; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    tone(buzzerPin, melody[thisNote], 150);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    // stop the tone playing:
    
  }
}

void playNyanSound() {
  if(nyanSoundCounter < introSize) {
    playSong(intro[nyanSoundCounter]);
  } else {
    playSong(melody[nyanSoundCounter - introSize]);
  }
  if (nyanSoundCounter == introSize + melodySize) {
    nyanSoundCounter = 0;
  } else {
    nyanSoundCounter++;
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

void loop() {
  char str[64];
  int light;
  int tap;
  String orientation;

  if(counter == 1000) {
    accel.read();
    light = analogRead(A0);
    prevPl = pl;
    pl = accel.readPL();
    orientation = getOrientation(pl);
    tap = accel.readTap();

    if(prevPl != pl && !nyan) {
      playSound(pl);
    }

    if(tap > 0) {
      nyan = !nyan;
    }

    if(!nyan && !eventSound) {
      digitalWrite(D7, LOW);
    }

    if(tap > 0 && nyan) {
      nyanSoundCounter = 0;
    }

    sprintf(str, "%d:%d:%d:%d:%s:%d", accel.x, accel.y, accel.z, light, orientation.c_str(), nyan);
    Serial.println(str);
    Spark.publish("accelData", str);
    counter = 0;
  }

  if(eventSound) {
    playEventSound();
    digitalWrite(D7, counter % 250 ? HIGH : LOW);
  }
  if(nyan && !eventSound) {
    playNyanSound();
    digitalWrite(D7, counter % 250 ? HIGH : LOW);
  }

  counter += 125;
  delay(125);
}
