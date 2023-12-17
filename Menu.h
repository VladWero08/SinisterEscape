#pragma once
#ifndef MENU_H
#define MENU_h

#include <LiquidCrystal.h>
#include <LedControl.h>
#include <string.h>

#include "EEPROM.h"

#include "Game.h"
#include "JoyStick.h"
#include "Highscores.h"
#include "MenuInput.h"
#include "MenuDisplay.h"
#include "Melody.h"
#include "Rooms.h"
#include "Utils.h"

const byte mainMenuMessagesSize = 4;
const char* mainMenuMessages[mainMenuMessagesSize] = {
  "Start game", "Highscores", "Settings", "About",
};

const byte settingsMenuSize = 6;
const char* settingsMenu[settingsMenuSize] = {
  "Enter name", "LCD bright", "Matrix bright", "Reset scores", "Sound", "Back"
};

const byte gameEndedMenuSize = 2;
const char* gameEndedMenu[gameEndedMenuSize] = {
  "Play again", "Back"
};

const byte aboutMenuSize = 4;
const char* aboutMenu[4] = {
  "SinisterEscape",
  // "Run through",
  // "linked rooms,",
  // "collecting 6",
  // "notes that",
  // "hold the",
  // "key to your",
  // "escape.",
  // "Be wary of",
  // "Dr. Nocturne's",
  // "pursuit.",
  // "",
  "Creator:",
  "VladWero08",
  "Back",
};

byte usernameCompletedSize;
const byte usernameSize = 3;
char* username[usernameSize] = {"", "", ""}; 

const byte brightnessNumberSize = 3;
char* brightnessNumber[brightnessNumberSize];

const byte letterAlphabetSize = 26;
const char* letterAlphabet[letterAlphabetSize] = {
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
};

const byte numbersAlphabetSize = 10;
const char* numbersAlphabet[numbersAlphabetSize] = {
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

struct Menu{
  LiquidCrystal lcd;
  LedControl lc;
  byte lcdBrightnessPin;
  byte buzzerPin;

  bool soundExitBlinking;
  bool sound;

  unsigned long highscoresResetTime;
  unsigned long gameStartTime;

  byte lcdBrightness;
  byte matrixBrightness;
  
  byte currentMenu;
  byte arrowMenuPosition;
  byte arrowMenuLinePosition;
  byte currentMenuPosition;

  Game game;
  MenuInput menuInput;

  Menu(byte RS, byte EN, byte D4, byte D5, byte D6, byte D7, 
       byte dinPin, byte clockPin, byte loadPin, 
       byte buzzerPin, byte lcdBrightnessPin): 
       lcd(RS, EN, D4, D5, D6, D7), lc(dinPin, clockPin, loadPin, 1), game(this->lc){    
    this->buzzerPin = buzzerPin;
    this->lcdBrightnessPin = lcdBrightnessPin;

    soundExitBlinking = false;
    
    currentMenu = 0;
    arrowMenuPosition = 0;
    arrowMenuLinePosition = 0;
    currentMenuPosition = 0; 

    lc.shutdown(0, false);
    lc.clearDisplay(0);
    
    loadMenuSettings();
    activateMenuSettins();
  }

  // functions to load and activate settings
  void loadMenuSettings();
  void activateMenuSettins();
  
  // functions related to the welcome message  
  void displayWelcomeMessage();
  void welcomeMessageHandler(Joystick &joystick);

  // functions related to the whole menu functionality
  void menuSwitch(Joystick &joystick);
  void menuWatcher(int maximumMenuSize, Joystick &joystick);

  // functions to handle main menu
  void mainMenuHandler(Joystick &joystick);

  // functions related to game menu
  void gameMenuHandler(Joystick &joystick);

  // funtions related to the highscores menu
  void highscoresMenuHandler(Joystick &joystick);

  // functions related to the settings menu
  void settingsMenuHandler(Joystick &joystick);
  void enterNameHandler(Joystick &joystick, byte parentMenu);
  void lcdBrightnessMenuHandler(Joystick &joystick);
  void matrixBrightnessMenuHandler(Joystick &joystick);
  void resetHighscoresHandler();
  void soundToggleHandler(Joystick &joystick);

  // functions related to about menu
  void aboutMenuHandler(Joystick &joystick);

  // reset functions 
  void resetMenu();
  void resetUserInput(char* userInput[], byte userInputSize);
};

/*
  Load brightness and sound settings, as well as the 
  highscores with their associated names from EEPROM memory.
*/
void Menu::loadMenuSettings(){
  // load the LCD brightness setting
  EEPROM.get(0, lcdBrightness);
  // load the matrix brightness setting
  EEPROM.get(1, matrixBrightness);
  // load the sound setting
  EEPROM.get(2, sound);

  loadPlayersHighschores();
};

/*
  Activate the brightness settings by writing
  the values to the PINs that control the matrix and the LCD.
*/
void Menu::activateMenuSettins(){
  // activate the LCD brightness setting
  analogWrite(lcdBrightnessPin, lcdBrightness);
  // activate the matrix brightness setting 
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
};



void Menu::displayWelcomeMessage(){
  displayMessageInCenter(lcd, "SinisterEscape", 0);
  displayMessageInCenterWithSkull(lcd, "RUN", 1);
};

/*
  After the joystick is pressed, exit from
  the welcoming message menu and enter the main menu.
*/
void Menu::welcomeMessageHandler(Joystick &joystick){
  if (joystick.currentSwitchStateChanged == HIGH) {
    currentMenu = 1;
    resetMenu();
  }
};


/*
  Acts as a central control point for managing different
  menus, depending where the user is currently at.

  Cases with 1 digit are usually "parents menu".
  Cases with 2 digits are "children menus", where the
  first digit represent the parent.

  Analyze the menu structure to have a better understanding
  of how the menus should communicate.
*/
void Menu::menuSwitch(Joystick &joystick){
  switch (currentMenu) {
    case 0:
      displayWelcomeMessage();
      welcomeMessageHandler(joystick);
      break;
    case 1:
      displayMenu(lcd, mainMenuMessages, currentMenuPosition, arrowMenuLinePosition);

      menuWatcher(mainMenuMessagesSize, joystick);
      mainMenuHandler(joystick);
      break;
    case 11:
      // display game menu
      gameMenuHandler(joystick);
      break;
    case 2:
      // display highscores
      displayHighscores(lcd, playerNames, highscores, 3, currentMenuPosition, arrowMenuLinePosition);

      menuWatcher(4, joystick);
      highscoresMenuHandler(joystick);
      break;
    case 3:
      // display settings menu
      displayMenu(lcd, settingsMenu, currentMenuPosition, arrowMenuLinePosition);

      menuWatcher(settingsMenuSize, joystick);
      settingsMenuHandler(joystick);
      break;
    case 30:
      // user needs to enter its name
      enterNameHandler(joystick, 3);
      break;
    case 31:
      // user needs to enter a number of LCD Brightness
      lcdBrightnessMenuHandler(joystick);
      break;
    case 32:
      // user needs to enter a number of matrix brightness
      matrixBrightnessMenuHandler(joystick);
      break;
    case 33:
      // highscores will be reset and user will be prompted 
      // a message upon success
      resetHighscoresHandler();
      break;
    case 34:
      // user needs to toggle the sound
      soundToggleHandler(joystick);
      break;
    case 4:
      // display about
      displayMenu(lcd, aboutMenu, currentMenuPosition, arrowMenuLinePosition);
      
      menuWatcher(aboutMenuSize, joystick);
      aboutMenuHandler(joystick);
      break;
    default:
      break;
  }

  playGameMelody(buzzerPin, sound, game);
};

/*
  For the menus with multiple options, 
  this functions listens to joystick movements and
  moves the arrow accordingly.
*/
void Menu::menuWatcher(int maximumMenuSize, Joystick &joystick){
  // joystick is pointing up
  if (joystick.direction == joystickUp) {
    lcd.clear();

    // if the arrow is on line 0 of the LCD
    if (arrowMenuLinePosition == 0) {
      // if the main menu is not pointing at 
      // the first option
      if (currentMenuPosition != 0) {
        arrowMenuPosition -= 1;
        currentMenuPosition -= 1;
      }
    } 
    // if the arrow is on line 1 of the LCD
    else {
      // move the arrow from the 
      // bottom line to the top line
      arrowMenuPosition -= 1;
      arrowMenuLinePosition = 0;
    }
  } 
  // joystick is pointing down 
  else if (joystick.direction == joystickDown) {
    lcd.clear();
    
    // if the arrow is on line 0 of the LCD
    if (arrowMenuLinePosition == 0){
      arrowMenuPosition += 1;
      arrowMenuLinePosition = 1;
    }
    // if the arrow is on line 1 of the LCD
    else {
      // if the main menu is not pointing at 
      // the last option
      if (currentMenuPosition != maximumMenuSize - 2) {
        arrowMenuPosition += 1;
        currentMenuPosition += 1;
      }
    }
  }
};



void Menu::mainMenuHandler(Joystick &joystick){
  // handle the button press when the
  // user is in the main menu
  if (joystick.currentSwitchStateChanged == HIGH) {
    // depending where the user is pointing,
    // handle each individual case
    switch (arrowMenuPosition) {
      case 0:
        // start the game
        game.reset(lc);
        gameStartTime = millis();

        currentMenu = 11;
        break;
      case 1:
        // set menu to highscores
        currentMenu = 2;
        break;
      case 2:
        // set menu to settings
        currentMenu = 3;
        break;
      case 3:
        // set menu to about section
        currentMenu = 4;
        break;
      default:
        break;
    }
    // reset the menu
    resetMenu();
  }
};


void Menu::gameMenuHandler(Joystick &joystick){
  // if the user has set its name, display a "good luck" message
  // before starting the game
  if ((millis() - gameStartTime) <= gameSpecialMomentsTimeInterval 
      && game.player.hasUserName) {
    displayGameStartedMessage(lcd, username, usernameCompletedSize);
    return;
  }

  // smooth transition between "good luck" message and actually
  // starting the game
  if ((millis() - gameStartTime) <= gameSpecialMomentsTimeInterval + transitionTime 
       && game.player.hasUserName) {
    lcd.clear();
    return;
  }

  // play the game; continue playing until the end messages
  // where all displayed to the user
  if (game.isRunning || game.isDisplayingEndMessage) {
    game.play(lc, lcd, joystick);
    return;
  }

  // if the player has a highscore but has not completed his name,
  // prompt the enter name handler
  if (game.player.hasHighscore && !game.player.hasUserName) {
    enterNameHandler(joystick, 11);
    return;
  }

  // if the player has a highscore, update the highscore array,
  // write it to EEPROM and stop displaying the "new highscore" message
  if (game.player.hasHighscore) {
    updateHighscores(game.time, username);
    writeHighscores();

    game.player.hasHighscore = false;
    lcd.clear();
    return;
  }

  displayMenu(lcd, gameEndedMenu, currentMenuPosition, arrowMenuLinePosition);
  menuWatcher(gameEndedMenuSize, joystick);
  
   if (joystick.currentSwitchStateChanged == HIGH) {
      switch (arrowMenuPosition) {
        case 0:
          // user chose to play again
          game.reset(lc);
          currentMenu = 11;
          break;
        case 1:
          // user chose to go back to the main menu
          currentMenu = 1;
          break;
        default:
          break;
      }  
      // reset the menu
      resetMenu();
   }
}



void Menu::highscoresMenuHandler(Joystick &joystick){
  // handle the button press when the
  // user is in the highscores menu
  if (joystick.currentSwitchStateChanged == HIGH) {
    // depending where the user is pointing,
    // handle each individual case
    switch (arrowMenuPosition) {
      case 3:
        // go back to the main menu
        currentMenu = 1;
        break;
      default:
        break;
    }
    // reset the menu
    resetMenu();
  }
};



void Menu::settingsMenuHandler(Joystick &joystick){
  // handle the button press when the
  // user is in the settings menu
  if (joystick.currentSwitchStateChanged == HIGH) {
    // depending where the user is pointing,
    // handle each individual case
    switch (arrowMenuPosition) {
      case 0:
        // enter the name
        menuInput.resetInputVariables();
        menuInput.currentInputCursorPosition = usernameCompletedSize;

        currentMenu = 30;
        break;
      case 1:
        // set the LCD brightnesssetByteUserInput
        menuInput.currentCursorColumnPosition = userInputStartPosition;
        menuInput.currentCursorLinePosition = 0;
        currentMenu = 31;
        break;
      case 2:
        // set the matrix brightness
        menuInput.currentCursorColumnPosition = userInputStartPosition;
        menuInput.currentCursorLinePosition = 0;

        // light up the whole matrix
        setCompleteMatrix(lc);

        currentMenu = 32;
        break;
      case 3:
        // reset the highscores
        lcd.clear();
        highscoresResetTime = millis();
        currentMenu = 33;
        break;
      case 4:
        // set the sounde ON/OFF
        currentMenu = 34;
        break;
      case 5:
        // go back to the main menu
        currentMenu = 1;
        break;
      default:
        break;
    }
    // reset the menu
    resetMenu();
  }
};

void Menu::enterNameHandler(Joystick &joystick, byte parentMenu){
  menuInput.userCursorLineHandler(lcd, joystick, letterAlphabet);

  if (joystick.currentSwitchStateChanged == HIGH && menuInput.currentCursorLinePosition == 0) {
    // if the user pressed the joystick and is pointing
    // somewhere on the first lin
    if (menuInput.currentCursorColumnPosition == deletePosition) {
      // if the user is pointing to the delete icon, 
      // delete the user input
      
      menuInput.currentInputCursorPosition = 0;
      resetUserInput(username, usernameSize);
    } else if (menuInput.currentCursorColumnPosition == verifyPosition) {
      // if the user is pointing to the exit icon,
      // set the username's completed size
      usernameCompletedSize = menuInput.currentInputCursorPosition;
      
      if (usernameCompletedSize > 0) {
        game.player.hasUserName = true;
      }

      // clear the lcd and go to the parent menu
      lcd.clear();
      currentMenu = parentMenu;
      return;
    } else if (menuInput.currentCursorColumnPosition == exitPosition) {
      // if the user is pointing to the exit icon,
      // clear the lcd and go to the parent menu
      lcd.clear();
      currentMenu = parentMenu;
      return;
    }
  }

  if (menuInput.currentCursorLinePosition == 0) {
    menuInput.userInputControlsHandler(lcd, joystick);
  } else {
    menuInput.userInputHandler(lcd, joystick, usernameSize, username, letterAlphabet);
    menuInput.userAlphabetHandler(lcd, joystick, letterAlphabet, 0, letterAlphabetSize);
  }
};

void Menu::lcdBrightnessMenuHandler(Joystick &joystick){
  byte oldBrightness = lcdBrightness;
  menuInput.userBrightnessInputHandler(lcd, joystick, lcdBrightness, 1, 255);

  // if the brightness has been changed, write
  // the new value to the LCD's pins so the user can
  // see in real time the changes 
  if (oldBrightness != lcdBrightness) {
    analogWrite(lcdBrightnessPin, lcdBrightness);
  }

  // if the user pressed the joystick
  if (joystick.currentSwitchStateChanged == HIGH) { 
    // if the user is pointing to the exit icon,
    // clear the lcd and go to the parent menu
     if (menuInput.currentCursorColumnPosition == exitPosition) {
      analogWrite(lcdBrightnessPin, lcdBrightness);
      EEPROM.put(0, lcdBrightness);

      lcd.clear();
      menuInput.resetInputVariables();
      currentMenu = 3;
      return;
    }
  }
};

void Menu::matrixBrightnessMenuHandler(Joystick &joystick){
  byte oldBrightness = matrixBrightness;
  menuInput.userBrightnessInputHandler(lcd, joystick,  matrixBrightness, 1, 15);

  // if the brightness has been changed, write
  // the new value to the matrix so the user can
  // see in real time the changes 
  if (oldBrightness != matrixBrightness) {
    lc.setIntensity(0, matrixBrightness);
  }

  // if the user pressed the joystick
  if (joystick.currentSwitchStateChanged == HIGH) { 
    // if the user is pointing to the exit icon,
    // clear the lcd and go to the parent menu
     if (menuInput.currentCursorColumnPosition == exitPosition) {
      lc.setIntensity(0, matrixBrightness);
      EEPROM.put(1, matrixBrightness);

      lcd.clear();
      menuInput.resetInputVariables();
      resetMatrix(lc);
      currentMenu = 3;
      return;
    }
  }
};

void Menu::resetHighscoresHandler(){
  // before returning to the parent menu, display
  // a message in which the user is acknowledged that
  // that the highscores were reset succesfully
  if ((millis() - highscoresResetTime) <= 2000) {
    displayMessageInCenter(lcd, "Highscores reset", 0);
    displayMessageInCenter(lcd, "successfully.", 1);
    return;
  } 

  lcd.clear();
  resetHighscores();
  // return to the parent menu
  currentMenu = 3;
}

void Menu::soundToggleHandler(Joystick &joystick){
  // listen to joystick movements to move from setting the
  // sound to the exit sign on the right of the LCD

  if (joystick.direction == joystickRight && soundExitBlinking == false) {
    soundExitBlinking = true;
  } 

  if (joystick.direction == joystickLeft && soundExitBlinking == true) {
    soundExitBlinking = false;
  }

  // listen to the joystick switch being pressed
  if (joystick.currentSwitchStateChanged == HIGH) {
    lcd.clear();
    
    // if the joystick is pressed and the user
    // is pointing towards the exit, exit from the sound menu
    if (soundExitBlinking) {
      currentMenu = 3;
      return;
    }
    // otherwise, toggle the sound buttton
    else {
      sound = !sound;
      EEPROM.put(2, sound);
    }
  }

  displaySoundSetting(lcd, sound, soundExitBlinking);
};



void Menu::aboutMenuHandler(Joystick &joystick){
  // handle the button press when the
  // user is in the about menu
  if (joystick.currentSwitchStateChanged == HIGH) {
    // depending where the user is pointing,
    // handle each individual case
    switch (arrowMenuPosition) {
      case aboutMenuSize - 1:
        // switch back to the main menu
        currentMenu = 1;
        // reset the menu
        resetMenu();
      default:
        break;
    }
  }
};

void Menu::resetMenu(){
  // reset the arrow and the variables that are 
  // holding the current state of the menu
  lcd.clear();
  arrowMenuPosition = 0;
  arrowMenuLinePosition = 0;
  currentMenuPosition = 0;
};

void Menu::resetUserInput(char* userInput[], byte userInputSize){
  for (int i = 0; i < userInputSize; i++) {
    // reset the value in the memory
    userInput[i] = " ";

    // reset the value visually, in the LCD
    lcd.setCursor(userInputStartPosition + i, 0);
    lcd.print(userInput[i]);
  }
};

#endif