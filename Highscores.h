#pragma once
#ifndef HIGHSCORES_H
#define HIGHSCORES_H

const byte playerNamesStartAddr = 3;
const byte highscoreStartAddr = 6;
const byte highscoresRegisteredAddr = 24;
const byte maximumHighscores = 3;

byte highscoresRegistered;
unsigned long highscores[maximumHighscores];
char* playerNames[maximumHighscores];

void loadPlayersHighschores(){
  EEPROM.get(highscoresRegisteredAddr, highscoresRegistered);

  for (int score = 0; score < maximumHighscores; score++) {
    // read the ith score and the player's name who 
    // made that score

    char playerName[4];
    // player names are made of 3 characters
    for (int letter = 0; letter < 3; letter++) {
      EEPROM.get(playerNamesStartAddr + score * 7 + letter, playerName[letter]);
    }
    // add the null character at the end
    playerName[3] = '\0';
    // copy the value into the array of player names
    playerNames[score] = strdup(playerName);
    
    EEPROM.get(highscoreStartAddr + score * 7, highscores[score]);
  }  
};

void updateHighscores(unsigned long newHighscore, char* username[]){
  for (int i = 0; i < highscoresRegistered; i++) {
    if (newHighscore < highscores[i]) {
      for (int j = highscoresRegistered - 1; j >= i + 1; j--) {
        highscores[j] = highscores[j - 1]; 
        playerNames[j] = strdup(playerNames[j - 1]);
      }

      highscores[i] = newHighscore;

      playerNames[i] = "";
      for (int j = 0; j < 3; j++) {
        strcat(playerNames[i], username[j]);
      }
      break;
    }
  }
};

void writeHighscores(){
  EEPROM.put(highscoresRegisteredAddr, highscoresRegistered);

  for (int i = 0; i < highscoresRegistered; i++) {
    EEPROM.put(highscoreStartAddr + i * 7, highscores[i]);

    for (int letter = 0; letter < 3; letter++) {
      EEPROM.put(playerNamesStartAddr + i * 7 + letter, playerNames[i][letter]);
    }
  }
};

void resetHighscores(){
  for (int i = 0; i < maximumHighscores; i++) {
    highscores[i] = 900;

    for (int letter = 0; letter < 3; letter++) {
      playerNames[i][letter] = "-";
    }
  }

  highscoresRegistered = 3;
  writeHighscores();
  highscoresRegistered = 0;
};

#endif