#pragma once
#ifndef MENU_GAME_H
#define MENU_GAME_H

#include <LiquidCrystal.h>
#include "MenuDisplay.h"
#include "Utils.h"

const byte heartsStartPosition = 13;

struct MenuGame{
  // time since the game started
  unsigned int time;
  // last time when the time was incremented
  unsigned long lastTimeIncrement;
  byte notes;
  byte lives;

  MenuGame(): time(0), notes(0), lives(3) {
    lastTimeIncrement = millis();
  }

  void display(LiquidCrystal lcd);
  void displayLives(LiquidCrystal lcd);
  void displayNotes(LiquidCrystal lcd);
  void displayTime(LiquidCrystal lcd);

  void resetMenuGame();
};

/*
  Display the whole game menu
*/
void MenuGame::display(LiquidCrystal lcd){
  displayLives(lcd);
  displayNotes(lcd);
  displayTime(lcd);
};

void MenuGame::displayLives(LiquidCrystal lcd){
  for(int i = 0; i < lives; i++) {
    lcd.setCursor(heartsStartPosition + i, 0);
    lcd.write(heartIndex);
  }
};

void MenuGame::displayNotes(LiquidCrystal lcd){
  lcd.setCursor(0, 0);
  lcd.print("Notes: ");

  // 7 is the length of "Notes "
  lcd.setCursor(7, 0);
  lcd.print(notes);
};

void MenuGame::displayTime(LiquidCrystal lcd){
  // if a 1000 ms have passed, it means a second,
  // so the time will be incremented by 1
  if ((millis() - lastTimeIncrement) >= 1000) {
    lastTimeIncrement = millis();
    time += 1;
  }

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  
  displayTimeFromSeconds(lcd, time, 6, 1);
};

void MenuGame::resetMenuGame(){
  time = 0;
  notes = 0;
  lives = 3;
  lastTimeIncrement = millis();
};

#endif