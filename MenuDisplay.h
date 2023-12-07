#pragma once
#ifndef MENU_DISPLAY_H
#define MENU_DISPLAY_h

#include "LiquidCrystal.h"

const byte lcdBlinkingInterval = 500;
bool displayBlinking = true;
unsigned long lastBlinkingChar = 0;

/*
  Display the given custom character, in the given 
  position, depending on the blinking control variable
*/
void displayBlinkingInt(LiquidCrystal lcd, const int message, const int line, const int column){
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
  position, depending on the blinking control variable
*/
void displayBlinkingChar(LiquidCrystal lcd, const char* message, const int line, const int column){
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
  
/*
  Display the given message in the middle columns
  of the LCD, knowing the LCD has 16 columns
*/
void displayMessageInCenter(LiquidCrystal lcd, const char* message,  const int line){
  int spaces = (16 - strlen(message)) / 2;
  lcd.setCursor(spaces, line);
  lcd.print(message);
};

/*
  Display the given message in the middle columns
  of the LCD, and besides the message ( left an right), a skull.
*/
void displayMessageInCenterWithSkull(LiquidCrystal lcd, const char* message,  const int line){
  int spaces = (16 - strlen(message)) / 2;
  int firstSkullPosition = spaces - 2;
  int secondSkullPosition = spaces + strlen(message) + 2 - 1;

  displayBlinkingInt(lcd, (uint8_t) 0, line, firstSkullPosition);
  displayBlinkingInt(lcd, (uint8_t) 0, line, secondSkullPosition);

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
void displaySoundSetting(LiquidCrystal lcd, bool sound, bool exitIsBlinking){
  // display the arrow at the beggining of the input
  lcd.setCursor(0, 0);
  lcd.write((uint8_t) 1);

  if (exitIsBlinking) {
    // display the exit symbol blinking
    displayBlinkingInt(lcd, 4, 0, 15);
  } else {
    // display the exit symbol
    lcd.setCursor(15, 0);
    lcd.write((uint8_t) 4);
  }

  lcd.setCursor(2, 0);
  if (sound) {
    lcd.print("Sound is ON!");
  } else {
    lcd.print("Sound is OFF!");
  }

  displayMessageInCenter(lcd, "Press to toggle!", 1);  
}

/*
  Given an array of messages of a menu, and a index, display 
  the current messages corresponding to the index. 
  
  Also, display an arrow pointing to the current option.
*/
void displayMenu(LiquidCrystal lcd, const char* menu[], int menuIndex, int arrowLinePosition){  
  lcd.setCursor(0, arrowLinePosition);
  lcd.write((uint8_t) 1);

  lcd.setCursor(2, 0);
  lcd.print(menu[menuIndex]);
  
  lcd.setCursor(2, 1);
  lcd.print(menu[menuIndex + 1]);
};

#endif 