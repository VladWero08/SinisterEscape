#pragma once
#ifndef MENU_DISPLAY_H
#define MENU_DISPLAY_h

#include "LiquidCrystal.h"
#include "CustomCharacters.h"
#include "ConstantsHighscore.h"

const byte lcdBlinkingInterval = 500;
bool displayBlinking = true;
unsigned long lastBlinkingChar = 0;

/*
  Display the given custom character, in the given 
  position, depending on the blinking control variable
*/
void displayBlinkingInt(LiquidCrystal &lcd, const int message, const int line, const int column){
    if ((millis() - lastBlinkingChar) > lcdBlinkingInterval) {
      displayBlinking = !displayBlinking;
      lastBlinkingChar = millis();
    }

    if (displayBlinking) {
      lcd.setCursor(column, line);
      lcd.write(message);
    } else {
      lcd.setCursor(column, line);
      lcd.write(" ");
    }
};
  
/*
  Display the given message, in the given 
  position, depending on the blinking control variable.
*/
void displayBlinkingChar(LiquidCrystal &lcd, const char message, const int line, const int column){
    if ((millis() - lastBlinkingChar) > lcdBlinkingInterval) {
      displayBlinking = !displayBlinking;
      lastBlinkingChar = millis();
    }

    if (displayBlinking) {
      lcd.setCursor(column, line);
      lcd.print(message);
    } else {
      lcd.setCursor(column, line);
      lcd.print(" ");
    }
};
  
void displayBlinkingByte(LiquidCrystal &lcd, const byte message, const int line, const int column){
    if ((millis() - lastBlinkingChar) > lcdBlinkingInterval) {
      displayBlinking = !displayBlinking;
      lastBlinkingChar = millis();
    }

    if (displayBlinking) {
      lcd.setCursor(column, line);
      lcd.print(message);
    } else {
      lcd.setCursor(column, line);
      lcd.print(" ");
    }
}

/*
  Display the given message in the middle columns
  of the LCD, knowing the LCD has 16 columns
*/
void displayMessageInCenter(LiquidCrystal &lcd, const char* message,  const int line){
  int spaces = (16 - strlen(message)) / 2;
  lcd.setCursor(spaces, line);
  lcd.print(message);
};

/*
  Display the given message in the middle columns
  of the LCD, and besides the message ( left an right), a skull.
*/
void displayMessageInCenterWithSkull(LiquidCrystal &lcd, const char* message,  const int line){
  int spaces = (16 - strlen(message)) / 2;
  int firstSkullPosition = spaces - 2;
  int secondSkullPosition = spaces + strlen(message) + 2 - 1;

  displayBlinkingInt(lcd, skullIndex, line, firstSkullPosition);
  displayBlinkingInt(lcd, skullIndex, line, secondSkullPosition);

  lcd.setCursor(spaces, line);
  lcd.print(message);
};

/*
  Reset the variables the variables that handle
  the blinking of the character the cursor is pointing at
*/
void resetBlinkingVariables(){
  displayBlinking = true;
  lastBlinkingChar = millis();
};

/*
  Display the sound settings menu, which shows 
  the state of the sound (ON/OFF), an instruction about
  how to toggle the setting and an exit symbol.

  If the user is pointing to the exit symbol, 
  it will be blinking, otherwise is just static.
*/
void displaySoundSetting(LiquidCrystal &lcd, bool sound, bool exitIsBlinking){
  // display the arrow at the beggining of the input
  lcd.setCursor(0, 0);
  lcd.write(arrowIndex);

  if (exitIsBlinking) {
    // display the exit symbol blinking
    displayBlinkingInt(lcd, 4, 0, 15);
  } else {
    // display the exit symbol
    lcd.setCursor(15, 0);
    lcd.write(exitIndex);
  }

  lcd.setCursor(2, 0);
  if (sound) {
    lcd.print("Sound ON!");
  } else {
    lcd.print("Sound OFF!");
  }

  displayMessageInCenter(lcd, "Press to toggle!", 1);  
}

/*
  Given an array of messages of a menu, and a index, display 
  the current messages corresponding to the index. 
  
  Also, display an arrow pointing to the current option.
*/
void displayMenu(LiquidCrystal &lcd, const char* menu[], int menuIndex, int arrowLinePosition){  
  lcd.setCursor(0, arrowLinePosition);
  lcd.write(arrowIndex);

  lcd.setCursor(2, 0);
  lcd.print(menu[menuIndex]);
  
  lcd.setCursor(2, 1);
  lcd.print(menu[menuIndex + 1]);
};

/*
  Given the position in an LCD, 
  display the time as <minutes>:<seconds>.
*/
void displayTimeFromSeconds(LiquidCrystal &lcd, const unsigned int time, const byte column, const byte line) {
  unsigned int minutes = time / 60;
  unsigned int seconds = time % 60;

  // if the minutes are smaller than 10,
  // also display a 0 before the minute
  if (minutes < 10) {
    lcd.setCursor(column, line);
    lcd.print("0");

    lcd.setCursor(column + 1, line);
    lcd.print(minutes);
  } else {
    lcd.setCursor(column, line);
    lcd.print(minutes);
  }

  // display the separation column
  lcd.setCursor(column + 2, line);
  lcd.print(":");

  // if the seconds are smaller than 10,
  // also display a 0 before the second
  if (seconds < 10) {
    lcd.setCursor(column + 3, line);
    lcd.print("0");

    lcd.setCursor(column + 4, line);
    lcd.print(seconds);
  } else {
    lcd.setCursor(column + 3, line);
    lcd.print(seconds);
  }
};

void displayPlayerAndScore(LiquidCrystal &lcd, char playerName[playerNameSize], const unsigned long score, const byte line){
  if (score == 900) {
    lcd.setCursor(4, line);
    lcd.print("none");
  } else {
      // display the name of the player
      for (int i = 0; i < 3; i++) {
        lcd.setCursor(4 + i, line);
        lcd.print(playerName[i]);
      }
      
      // display the score of the player
      displayTimeFromSeconds(lcd, score, 8, line);
  }
}

/*
  Given an array of player names, their scores, and a index,
  display the ranking symbol, the name and the score of the players
  on each line of the LCD.
*/
void displayHighscores(LiquidCrystal &lcd, char playerNames[maximumHighscores][playerNameSize], const unsigned long scores[], int maxPlayers, int menuIndex, int arrowLinePosition) {
  lcd.setCursor(0, arrowLinePosition);
  lcd.write(arrowIndex);

  if (menuIndex < maxPlayers - 1) {
    for (int i = 0; i < 2; i++) {
      // display the symbol for the podium
      lcd.setCursor(2, i);
      lcd.print(menuIndex + i + 1);

      displayPlayerAndScore(lcd, playerNames[menuIndex + i], scores[menuIndex + i], i);
    }
  } else {
      // display the symbol for the podium
      lcd.setCursor(2, 0);
      lcd.print(menuIndex + 1);

      displayPlayerAndScore(lcd, playerNames[menuIndex], scores[menuIndex], 0);

      lcd.setCursor(2, 1);
      lcd.print("Back");
  }
};


void displayGameStartedMessage(LiquidCrystal &lcd, char username[], byte usernameSize) {
  displayMessageInCenter(lcd, "Good luck,", 0);

  for (int i = 0; i < usernameSize; i++) {
    lcd.setCursor(6 + i, 1);
    lcd.print(username[i]);
  }
}

#endif 