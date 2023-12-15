#pragma once
#ifndef MELODY_H
#define MELODY_H

#include "Game.h"
#include "pitches.h"

int melodyNBC[] = {
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_GS4, NOTE_F4, NOTE_F4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_GS4, NOTE_G4,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_GS4, NOTE_G4, NOTE_DS4, NOTE_D4, NOTE_G4,
};

byte durationsNBC[] = {
  125, 125, 125, 125, 62, 62, 125, 250,
  62, 62, 125, 62, 62, 125, 62, 62, 125, 250,
  125, 125, 125, 62, 62, 125, 62, 62, 250,
  125, 125, 62, 62, 125, 125, 125, 125, 125,
};

int noteMelody[] = {
  REST, NOTE_G4, NOTE_B4, NOTE_D5, NOTE_G4, NOTE_B4, NOTE_D5, REST
};

byte noteDurations[] = {
  100, 100, 100, 100, 100, 100, 100, 100
};

unsigned long previousNoteTime = 0;
unsigned long currentNote = 0;
bool playingMelody = false;

/*
  Given the pin to which the buzzer is connected, a melody and
  its notes durations, use the tone function to reproduce 
  the melody, with a small delay between notes
*/
void playMelody(const byte buzzerPin, const int melody[], const byte durations[], const byte sizeOfMelody, float duration){
  unsigned long currentNoteTime = millis();
  
  if (playingMelody) {
    if ((currentNoteTime - previousNoteTime) >= durations[currentNote] * duration) {
      previousNoteTime = currentNoteTime;
      tone(buzzerPin, melody[currentNote], durations[currentNote]);
      playingMelody = false;

      currentNote += 1;
      if (currentNote >= sizeOfMelody) {
        currentNote = 0;
      }
    }
  } else {
    if ((currentNoteTime - previousNoteTime) >= durations[currentNote] * 1.3) {
      previousNoteTime = currentNoteTime;
      noTone(buzzerPin);
      playingMelody = true;
    }
  }
};

/*
  Depending on the sound settings set by the user
  and state of the game, play different melodies.

  A special melody will be played when the user has found a note.

  The game melody will play the entire time, if the sound
  setting is set to on, obviously.
*/
void playGameMelody(const byte buzzerPin, bool soundIsOn, Game game){
  // if sound is off, exit imediately
  if (!soundIsOn) {
    return;
  }

  // prioritize player foundin a note by first checking if he found a note recently enough
  if ((millis() - game.lastNoteFound) <= 2000 && game.player.notes > 0) {
    playMelody(buzzerPin, noteMelody, noteDurations, sizeof(noteMelody) / sizeof(noteMelody[0]), 1.00);
  }
  // otherwise, if the game is running and doctor has level 2, speed up
  // the game's melody
  else if (game.isRunning && game.doctor.level == 2) {
    playMelody(buzzerPin, melodyNBC, durationsNBC, sizeof(melodyNBC) / sizeof(melodyNBC[0]), 0.75);
  } 
  // if the game is running and doctor has level 3, speed up 
  // even more, because its the hardest level
  else if (game.isRunning && game.doctor.level == 3) {
    playMelody(buzzerPin, melodyNBC, durationsNBC, sizeof(melodyNBC) / sizeof(melodyNBC[0]), 0.5);
  } 
  // if no special case is happening, just play the normal melody
  else {
    playMelody(buzzerPin, melodyNBC, durationsNBC, sizeof(melodyNBC) / sizeof(melodyNBC[0]), 1.00);
  }
}


#endif