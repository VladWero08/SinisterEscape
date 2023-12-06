#pragma once
#ifndef MENU_DISPLAY_H
#define MENU_DISPLAY_h

#include "LiquidCrystal.h"

const byte lcdBlinkingInterval = 500;
bool displayBlinking = true;
unsigned long lastBlinkingChar = 0;
  
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

void resetBlinkingVariables(){
  displayBlinking = true;
  lastBlinkingChar = millis();
};

void displayMessageInCenter(LiquidCrystal lcd, const char* message,  const int line){
  int spaces = (16 - strlen(message)) / 2;
  lcd.setCursor(spaces, line);
  lcd.print(message);
};

void displayMessageInCenterWithSkull(LiquidCrystal lcd, const char* message,  const int line){
  int spaces = (16 - strlen(message)) / 2;
  int firstSkullPosition = spaces - 2;
  int secondSkullPosition = spaces + strlen(message) + 2 - 1;

  displayBlinkingInt(lcd, (uint8_t) 0, line, firstSkullPosition);
  displayBlinkingInt(lcd, (uint8_t) 0, line, secondSkullPosition);

  lcd.setCursor(spaces, line);
  lcd.print(message);
};

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

#endif 