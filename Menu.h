#ifndef MENU_H
#define MENU_h

#include <LiquidCrystal.h>
#include "JoyStick.h"
#include "MenuInput.h"
#include "MenuDisplay.h"

const byte mainMenuMessagesSize = 4;
const char* mainMenuMessages[mainMenuMessagesSize] = {
  "Start game", "Highscore", "Settings", "About",
};

const byte settingsMenuSize = 5;
const char* settingsMenu[settingsMenuSize] = {
  "Enter name", "LCD bright", "Matrix bright", "Sound", "Back"
};

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

  bool sound;
  bool soundExitBlinking;

  byte currentMenu;
  byte arrowMenuPosition;
  byte arrowMenuLine;
  byte currentMenuPosition;

  MenuInput menuInput;

  Menu(byte RS, byte EN, byte D4, byte D5, byte D6, byte D7): lcd(RS, EN, D4, D5, D6, D7){    
    sound = true;
    soundExitBlinking = false;

    currentMenu = 0;
    arrowMenuPosition = 0;
    arrowMenuLine = 0;
    currentMenuPosition = 0; 
  }

  void menuSwitch(Joystick joystick);
  void menuWatcher(int maximumMenuSize, Joystick joystick);
  void menuReset();
  void displayMenu(const char * menu[]);

  void displayWelcomeMessage();
  void welcomeMessageHandler(Joystick joystick);
  void mainMenuHandler(Joystick joystick);

  void settingsMenuHandler(Joystick joystick);
  void enterNameHandler(Joystick joystick);
  void lcdBrightnessMenuHandler(Joystick joystick);
  void matrixBrightnessMenuHandler(Joystick joystick);
  void soundToggleHandler(Joystick joystick);
};

void Menu::menuSwitch(Joystick joystick){
  switch (currentMenu) {
    case 0:
      displayWelcomeMessage();
      welcomeMessageHandler(joystick);
      break;
    case 1:
      menuWatcher(mainMenuMessagesSize, joystick);
      displayMenu(mainMenuMessages);
      mainMenuHandler(joystick);
      break;
    case 2:
      // display highscores
      break;
    case 3:
      // display settings menu
      menuWatcher(settingsMenuSize, joystick);
      displayMenu(settingsMenu);
      settingsMenuHandler(joystick);
      break;
    case 30:
      // user needs to enter its name
      enterNameHandler(joystick);
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
      // user needs to toggle the sound
      soundToggleHandler(joystick);
      break;
    case 4:
      // display about
      break;
    default:
      break;
  }
};

void Menu::menuWatcher(int maximumMenuSize, Joystick joystick){
  // joystick is pointing up
  if (joystick.direction == 0) {
    lcd.clear();

    // if the arrow is on line 0 of the LCD
    if (arrowMenuLine == 0) {
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
      arrowMenuLine = 0;
    }
  } 
  // joystick is pointing down 
  else if (joystick.direction == 1) {
    lcd.clear();
    
    // if the arrow is on line 0 of the LCD
    if (arrowMenuLine == 0){
      arrowMenuPosition += 1;
      arrowMenuLine = 1;
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

void Menu::menuReset(){
  // reset the arrow and the variables that are 
  // holding the current state of the menu
  lcd.clear();
  arrowMenuPosition = 0;
  arrowMenuLine = 0;
  currentMenuPosition = 0;
};

void Menu::displayMenu(const char* menu[]){  
  lcd.setCursor(0, arrowMenuLine);
  lcd.write((uint8_t) 1);

  lcd.setCursor(2, 0);
  lcd.print(menu[currentMenuPosition]);
  
  lcd.setCursor(2, 1);
  lcd.print(menu[currentMenuPosition + 1]);
};

void Menu::displayWelcomeMessage(){
  displayMessageInCenter(lcd, "SinisterEscape", 0);
  displayMessageInCenterWithSkull(lcd, "RUN", 1);
};

void Menu::welcomeMessageHandler(Joystick joystick){
  if (joystick.currentSwitchStateChanged == HIGH) {
    currentMenu = 1;
    menuReset();
  }
};

void Menu::mainMenuHandler(Joystick joystick){
  // handle the button press when the
  // user is in the main menu
  if (joystick.currentSwitchStateChanged == HIGH) {
    // depending where the user is pointing,
    // handle each individual case
    switch (arrowMenuPosition) {
      case 0:
        // start the game
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
    menuReset();
  }
};

void Menu::settingsMenuHandler(Joystick joystick){
  // handle the button press when the
  // user is in the main menu
  if (joystick.currentSwitchStateChanged == HIGH) {
    // depending where the user is pointing,
    // handle each individual case
    switch (arrowMenuPosition) {
      case 0:
        // enter the name
        menuInput.resetInputVariables();
        currentMenu = 30;
        break;
      case 1:
        // set the LCD brightness
        menuInput.resetInputVariables();
        currentMenu = 31;
        break;
      case 2:
        // set the matrix brightness
        menuInput.resetInputVariables();
        currentMenu = 32;
        break;
      case 3:
        // set the sounde ON/OFF
        currentMenu = 33;
        break;
      case 4:
        // go back to the main menu
        currentMenu = 1;
        break;
      default:
        break;
    }
    // reset the menu
    menuReset();
  }
};

void Menu::enterNameHandler(Joystick joystick){
  menuInput.userCursorLineHandler(lcd, joystick, letterAlphabet);

  int returnToParentMenu = menuInput.joystickPressControlsHandler(lcd, joystick, usernameSize, username);

  if (returnToParentMenu == 1) {
    menuInput.resetInputVariables();
    currentMenu = 3;
    return;
  }


  if (menuInput.currentCursorLinePosition == 0) {
    menuInput.userInputControlsHandler(lcd, joystick);
  } else {
    menuInput.userInputHandler(lcd, joystick, usernameSize, username, letterAlphabet);
    menuInput.userAlphabetHandler(lcd, joystick, letterAlphabet, 0, letterAlphabetSize);
  }
}

void Menu::lcdBrightnessMenuHandler(Joystick joystick){
  menuInput.userCursorLineHandler(lcd, joystick, numbersAlphabet);

  int returnToParentMenu = menuInput.joystickPressControlsHandler(lcd, joystick, brightnessNumberSize, brightnessNumber);

  if (returnToParentMenu == 1) {
    menuInput.resetInputVariables();
    currentMenu = 3;
    return;
  }

  if (menuInput.currentCursorLinePosition == 0) {
    menuInput.userInputControlsHandler(lcd, joystick);
  } else {
    menuInput.userInputHandler(lcd, joystick, brightnessNumberSize, brightnessNumber, numbersAlphabet);
    menuInput.userAlphabetHandler(lcd, joystick, numbersAlphabet, 0, numbersAlphabetSize);
  }
};

void Menu::matrixBrightnessMenuHandler(Joystick joystick){
  menuInput.userCursorLineHandler(lcd, joystick, numbersAlphabet);

  int returnToParentMenu = menuInput.joystickPressControlsHandler(lcd, joystick, brightnessNumberSize, brightnessNumber);

  if (returnToParentMenu == 1) {
    menuInput.resetInputVariables();
    currentMenu = 3;
    return;
  }

  if (menuInput.currentCursorLinePosition == 0) {
    menuInput.userInputControlsHandler(lcd, joystick);
  } else {
    menuInput.userInputHandler(lcd, joystick, brightnessNumberSize, brightnessNumber, numbersAlphabet);
    menuInput.userAlphabetHandler(lcd, joystick, numbersAlphabet, 0, numbersAlphabetSize);
  }
};

void Menu::soundToggleHandler(Joystick joystick){
  if (joystick.direction == joystickRight && soundExitBlinking == false) {
    soundExitBlinking = true;
  } 

  if (joystick.direction == joystickLeft && soundExitBlinking == true) {
    soundExitBlinking = false;
  }

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
    }
  }

  displaySoundSetting(lcd, sound, soundExitBlinking);
}

#endif