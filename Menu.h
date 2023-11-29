#include <LiquidCrystal.h>
#include "JoyStick.h"

const byte lcd_blinking_interval = 500;

const byte mainMenuMessagesSize = 4;
const char* mainMenuMessages[] = {
  "Start game", "Highscore", "Settings", "About",
};

const byte settingsMenuSize = 5;
const char* settingsMenu[] = {
  "Enter name", "LCD bright", "Matrix bright", "Sound", "Back"
};

struct Menu{
  LiquidCrystal lcd;

  bool display_skull;
  unsigned long last_skull_display;

  byte currentMenu;
  byte arrowMenuPosition;
  byte arrowMenuLine;
  byte currentMenuPosition;

  Menu(byte RS, byte EN, byte D4, byte D5, byte D6, byte D7): lcd(RS, EN, D4, D5, D6, D7){    
    display_skull = true;
    last_skull_display = 0;

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
  void displayMessageInCenter(const char* message,  const int line);
  void displayMessageInCenterWithSkull(const char* message,  const int line);

  void welcomeMessageHandler(Joystick joystick);
  void mainMenuHandler(Joystick joystick);
  void settingsMenuHandler(Joystick joystick);
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
  int first_skull_position = spaces - 2;
  int second_skull_position = spaces + strlen(message) + 2 - 1;

  if ((millis() - last_skull_display) > lcd_blinking_interval) {
    display_skull = !display_skull;
    last_skull_display = millis();
  }

  if (display_skull) {
    lcd.setCursor(first_skull_position, line);
    lcd.write((uint8_t) 0);

    lcd.setCursor(second_skull_position, line);
    lcd.write((uint8_t) 0);
  } else {
    lcd.setCursor(first_skull_position, line);
    lcd.write(" ");

    lcd.setCursor(second_skull_position, line);
    lcd.write(" ");
  }

  lcd.setCursor(spaces, line);
  lcd.print(message);
};

void Menu::welcomeMessageHandler(Joystick joystick){
  if (joystick.current_switch_state == HIGH) {
    currentMenu = 1;
    menuReset();
  }
};

void Menu::mainMenuHandler(Joystick joystick){
  // handle the button press when the
  // user is in the main menu
  if (joystick.current_switch_state == HIGH) {
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
  if (joystick.current_switch_state == HIGH) {
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