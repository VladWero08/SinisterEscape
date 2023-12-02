#pragma once
#ifndef MENU_DISPLAY_H
#define MENU_DISPLAY_h

#include "LiquidCrystal.h"

const byte lcdBlinkingInterval = 500;
bool displayBlinking = true;
unsigned long lastBlinkingChar = 0;
  
void displayBlinkingInt(LiquidCrystal lcd, const int message, const int line, const int position){
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
  
void displayBlinkingChar(LiquidCrystal lcd, const char* message, const int line, const int position){
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

void resetBlinkingVariables(){
  displayBlinking = true;
  lastBlinkingChar = millis();
};

#endif 