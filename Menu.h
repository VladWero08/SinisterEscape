#include <LiquidCrystal.h>
#include "JoyStick.h"

const byte lcdBlinkingInterval = 500;

const byte mainMenuMessagesSize = 4;
const char* mainMenuMessages[mainMenuMessagesSize] = {
  "Start game", "Highscore", "Settings", "About",
};

const byte settingsMenuSize = 5;
const char* settingsMenu[settingsMenuSize] = {
  "Enter name", "LCD bright", "Matrix bright", "Sound", "Back"
};

const byte arrowPosition = 0;
const byte userInputStartPosition = 2;
const byte verifyPosition = 15;

const char* username[3] = {"", "", ""}; 
const byte letterAlphabetSize = 26;
const char* letterAlphabet[letterAlphabetSize] = {
  "A", "B", "C", "D", "E", "F", "G", "H","I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
};

struct Menu{
  LiquidCrystal lcd;

  bool displayBlinking;
  unsigned long lastBlinkingChar;

  byte currentMenu;
  byte arrowMenuPosition;
  byte arrowMenuLine;
  byte currentMenuPosition;

  byte alphabetPositionBoundary;
  byte currentAlphabetCursorPosition;
  byte currentInputCursorPosition;

  Menu(byte RS, byte EN, byte D4, byte D5, byte D6, byte D7): lcd(RS, EN, D4, D5, D6, D7){    
    displayBlinking = true;
    lastBlinkingChar = 0;

    currentMenu = 0;
    arrowMenuPosition = 0;
    arrowMenuLine = 0;
    currentMenuPosition = 0; 

    currentAlphabetCursorPosition = 0;
    currentInputCursorPosition = 0;
    alphabetPositionBoundary = 0;
  }

  void menuSwitch(Joystick joystick);
  void menuWatcher(int maximumMenuSize, Joystick joystick);
  void menuReset();

  void displayMenu(const char * menu[]);

  void displayWelcomeMessage();
  void displayMessageInCenter(const char* message,  const int line);
  void displayMessageInCenterWithSkull(const char* message,  const int line);
  void displayBlinkingInt(const int message, const int line, const int position);
  void displayBlinkingChar(const char* message, const int line, const int position);

  void welcomeMessageHandler(Joystick joystick);
  void mainMenuHandler(Joystick joystick);
  void settingsMenuHandler(Joystick joystick);

  void userInputHandler(Joystick joystick, const int maxInput, const char* userInput[], const char* userAlphabet[]);
  void userAlphabetHandler(Joystick joystick, const char* userAlphabet[], const int leftBoundary, const int rightBoundary);

  void lcdBrightnessMenuHandler(Joystick joystick);
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
  displayMessageInCenter("SinisterEscape", 0);
  displayMessageInCenterWithSkull("RUN", 1);
};

void Menu::displayMessageInCenter(const char* message,  const int line){
  int spaces = (16 - strlen(message)) / 2;
  lcd.setCursor(spaces, line);
  lcd.print(message);
};

void Menu::displayMessageInCenterWithSkull(const char* message,  const int line){
  int spaces = (16 - strlen(message)) / 2;
  int firstSkullPosition = spaces - 2;
  int secondSkullPosition = spaces + strlen(message) + 2 - 1;

  displayBlinkingInt((uint8_t) 0, line, firstSkullPosition);
  displayBlinkingInt((uint8_t) 0, line, secondSkullPosition);

  lcd.setCursor(spaces, line);
  lcd.print(message);
};

void Menu::displayBlinkingInt(const int message, const int line, const int position){
    if ((millis() - lastBlinkingChar) > lcdBlinkingInterval) {
      displayBlinking = !displayBlinking;
      lastBlinkingChar = millis();
    }

    if (displayBlinking) {
      lcd.setCursor(position, line);
      lcd.write(message);
    } else {
      lcd.setCursor(position, line);
      lcd.write(" ");
    }
};

void Menu::displayBlinkingChar(const char* message, const int line, const int position){
    if ((millis() - lastBlinkingChar) > lcdBlinkingInterval) {
      displayBlinking = !displayBlinking;
      lastBlinkingChar = millis();
    }

    if (displayBlinking) {
      lcd.setCursor(position, line);
      lcd.print(message);
    } else {
      lcd.setCursor(position, line);
      lcd.print(" ");
    }
};

void Menu::welcomeMessageHandler(Joystick joystick){
  if (joystick.currentSwitchState == HIGH) {
    currentMenu = 1;
    menuReset();
  }
};

void Menu::mainMenuHandler(Joystick joystick){
  // handle the button press when the
  // user is in the main menu
  if (joystick.currentSwitchState == HIGH) {
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
  if (joystick.currentSwitchState == HIGH) {
    // depending where the user is pointing,
    // handle each individual case
    switch (arrowMenuPosition) {
      case 0:
        // enter the name
        break;
      case 1:
        // set the LCD brightness
        break;
      case 2:
        // set the matrix brightness
        break;
      case 3:
        // set the sounde ON/OFF
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

void Menu::userInputHandler(Joystick joystick, const int maxInput, const char* userInput[], const char* userAlphabet[]) {
  // display the arrow at the beggining of the input
  lcd.setCursor(arrowPosition, 0);
  lcd.write((uint8_t) 1);

  // display the verify symbol the end of the input
  lcd.setCursor(verifyPosition, 0);
  lcd.write((uint8_t) 2);

  // if the input has not reached the maximum size
  if (currentInputCursorPosition < maxInput) {
    // check if the joystick switch was pressed, and if
    // it was, add the letter the user is pointing at in the input
    if (joystick.currentSwitchStateChanged == HIGH) {
      username[currentInputCursorPosition] = userAlphabet[currentAlphabetCursorPosition];
      currentInputCursorPosition += 1;
    }
  }
  
  // display the user input
  for (int i = 0; i < currentInputCursorPosition; i++) {
    lcd.setCursor(userInputStartPosition + i, 0);
    lcd.print(userInput[i]);
  }
};

void Menu::userAlphabetHandler(Joystick joystick, const char* userAlphabet[], const int leftBoundary, const int rightBoundary) {
  // if the joystick moved to the left and the cursor position
  // will not move out of the left boundary of the alphabet
  if (joystick.direction == 2 && currentAlphabetCursorPosition > leftBoundary) {
    currentAlphabetCursorPosition -= 1;

    // if the cursor position moved beyond the right boundary of the LCD 
    if (currentAlphabetCursorPosition < alphabetPositionBoundary) {
      alphabetPositionBoundary -= 1;
    }
  } 

  // if the joystick moved to the right and the cursor position
  // will not move out of the right boundary of the alphabet
  if (joystick.direction == 3 && currentAlphabetCursorPosition < rightBoundary) {
    currentAlphabetCursorPosition += 1;

    // if the cursor position moved beyond the left boundary of the LCD 
    if (currentAlphabetCursorPosition >= alphabetPositionBoundary + 16) {
      alphabetPositionBoundary += 1;
    }
  }

  // display the alphabet and make the element positioned at
  // the cursor position blink
  for (int i = 0; i < 16; i++) {
    if (i + alphabetPositionBoundary == currentAlphabetCursorPosition) {
      // blink the cursor
      displayBlinkingChar(userAlphabet[currentAlphabetCursorPosition], 1, i);
    } else {
      // display the characters of the alphabet
      lcd.setCursor(i, 1);
      lcd.print(userAlphabet[i + alphabetPositionBoundary]);
    }   
  }
};

void Menu::lcdBrightnessMenuHandler(Joystick joystick){
  userInputHandler(joystick, 3, username, letterAlphabet);
  userAlphabetHandler(joystick, letterAlphabet, 0, letterAlphabetSize);
};



