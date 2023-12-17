#pragma once
#ifndef HIGHSCORES_H
#define HIGHSCORES_H

// addressed in EEPROM memory where
// highscores information is stored
const byte playerNamesStartAddr = 3;
const byte highscoreStartAddr = 6;
const byte highscoresRegisteredAddr = 24;
// the name of the player is 3 bytes, and the score
// is 4 bytes, so for a highscore 7 bytes are needed to store it
const byte highscoreSize = 7;
const unsigned int highscoreDefaultValue = 900;

// the maximum number of highscores that can be stored
const byte maximumHighscores = 3;
// the actual number of highscores that is stored in EPPROM
byte highscoresRegistered;

unsigned long highscores[maximumHighscores];
char* playerNames[maximumHighscores];

/*
  Load the number of highscores, the highscores and
  their associated player name from EEPROM.
*/
void loadPlayersHighschores(){
  // read the number of highscores registered
  EEPROM.get(highscoresRegisteredAddr, highscoresRegistered);

  for (int score = 0; score < maximumHighscores; score++) {
    // read the ith score and the player's name who 
    // made that score
    char playerName[4];

    // player names are made of 3 characters
    for (int letter = 0; letter < 3; letter++) {
      EEPROM.get(playerNamesStartAddr + score * highscoreSize + letter, playerName[letter]);
    }
    // add the null character at the end
    playerName[3] = '\0';
    // copy the value into the array of player names
    playerNames[score] = strdup(playerName);
    
    EEPROM.get(highscoreStartAddr + score * highscoreSize, highscores[score]);
  }  
};

/*
  Given a new highscores and its associated player name, insert
  the score into the list of highscores.

  Update the rest of the highscores as well (if possible), by
  copying them to a lower rank in the list.
*/
void updateHighscores(unsigned long newHighscore, char* playerName[]){
  for (int i = 0; i < highscoresRegistered; i++) {
    // search for the position where the new highscores
    // should ne inserted
    if (newHighscore < highscores[i]) {
      // copy all the values from the position where the new score
      // will be inserted into lower positions, to keep the top valid
      for (int j = highscoresRegistered - 1; j >= i + 1; j--) {
        highscores[j] = highscores[j - 1]; 
        playerNames[j] = strdup(playerNames[j - 1]);
      }

      // insert the new highscore
      highscores[i] = newHighscore;
      // also, insert the name associated with the score
      playerNames[i] = "";
      for (int j = 0; j < 3; j++) {
        strcat(playerNames[i], playerName[j]);
      }
      break;
    }
  }
};

/*
  Write the number of highscores, the highscores and
  their associated player name into EEPROM.

  This function will be called whenever the highscores
  will be updated, in order for the EEPROM memory to keep up
  with the changes.
*/
void writeHighscores(){
  EEPROM.put(highscoresRegisteredAddr, highscoresRegistered);

  for (int i = 0; i < highscoresRegistered; i++) {
    EEPROM.put(highscoreStartAddr + i * highscoreSize, highscores[i]);

    for (int letter = 0; letter < 3; letter++) {
      EEPROM.put(playerNamesStartAddr + i * highscoreSize + letter, playerNames[i][letter]);
    }
  }
};

/*
  Reset the highscores, seting the scores to a high
  value (15:00 minutes, 900 seconds). 
  
  Also, the number of highscores registered will become 0. 
*/
void resetHighscores(){
  for (int i = 0; i < maximumHighscores; i++) {
    highscores[i] = highscoreDefaultValue;

    for (int letter = 0; letter < 3; letter++) {
      playerNames[i][letter] = "-";
    }
  }

  highscoresRegistered = 3;
  writeHighscores();
  highscoresRegistered = 0;
};

#endif