#include "pitches.h"

const int melodyNBC[] = {
  NOTE_C4, NOTE_C4,
  NOTE_G4, NOTE_G4,
  NOTE_D4, NOTE_C4,
  NOTE_B4,
  NOTE_C4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_GS4, NOTE_F4, NOTE_F4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_GS4, NOTE_G4,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_GS4, NOTE_G4, NOTE_DS4, NOTE_D4, NOTE_G4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_G4,
  NOTE_AS4, NOTE_F4, NOTE_F4, NOTE_DS4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_A4,
  NOTE_B4, NOTE_B4, NOTE_B4, NOTE_AS4, NOTE_GS4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_AS4, NOTE_GS4,
  NOTE_B4, NOTE_G4, NOTE_G4, NOTE_FS4, NOTE_E4, NOTE_CS4, NOTE_CS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4,
  NOTE_FS4, NOTE_FS4, NOTE_G4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_G4, NOTE_FS4,
  NOTE_A3, NOTE_F4, NOTE_F4, NOTE_A3, NOTE_GS3, NOTE_A3, NOTE_B3, NOTE_CS4,
  NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4,
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4,
  NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4,
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4,
  NOTE_DS4, NOTE_DS4, NOTE_DS4, NOTE_D4, NOTE_C4, NOTE_DS4, NOTE_DS4, NOTE_DS4, NOTE_D4, NOTE_C4,
  NOTE_B4, NOTE_AS4, NOTE_GS4, NOTE_B4, NOTE_AS4, NOTE_GS4, NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_GS4, NOTE_G4, NOTE_F4,
  NOTE_F4, REST, REST
};

const int durationsNBC[] = {
  2, 2,
  2, 2,
  2, 2,
  1,
  2, 2,
  8, 8, 8, 8, 16, 16, 8, 4,
  16, 16, 8, 16, 16, 8, 16, 16, 8, 4,
  8, 8, 8, 16, 16, 8, 16, 16, 4,
  8, 8, 16, 16, 8, 8, 8, 8, 8,
  8, 8, 16, 16, 8, 8, 8, 16, 16, 8,
  8, 8, 8, 16, 16, 8, 8, 4,
  8, 8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 8,
  8, 8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 8,
  8, 8, 4, 16, 16, 16, 16, 8, 8,
  8, 8, 8, 8, 8, 16, 16, 4,
  8, 16, 16, 8, 16, 16, 16, 16, 8, 4,
  8, 8, 8, 8, 8, 16, 16, 4,
  8, 16, 16, 8, 16, 16, 16, 16, 8, 4,
  16, 16, 8, 8, 8, 16, 16, 16, 16, 4,
  8, 8, 16, 16, 8, 8, 8, 16, 16, 8,
  16, 16, 8, 16, 16, 8, 16, 16, 8, 16, 16, 8,
  4, 4, 2
};

void playMelody(const byte buzzerPin, const int melody[], const int durations[]){
  int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    int duration = 1000 / durations[note];
    tone(buzzerPin, melody[note], duration);
    
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    noTone(buzzerPin);
  }
}
