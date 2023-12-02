#pragma once
#ifndef MENU_INPUT_H
#define MENU_INPUT_h

#include "LiquidCrystal.h"
#include "JoyStick.h"
#include "MenuDisplay.h"

const byte arrowPosition = 0;
const byte userInputStartPosition = 2;
const byte verifyPosition = 15;

struct MenuInput{
  byte alphabetPositionBoundary;
  byte currentAlphabetCursorPosition;
  byte currentInputCursorPosition;

  MenuInput(){
    currentAlphabetCursorPosition = 0;
    currentInputCursorPosition = 0;
    alphabetPositionBoundary = 0;
  } 

  void userInputHandler(LiquidCrystal lcd, Joystick joystick, const int maxInput, const char* userInput[], const char* userAlphabet[]);
  void userAlphabetHandler(LiquidCrystal lcd, Joystick joystick, const char* userAlphabet[], const int leftBoundary, const int rightBoundary);
  void resetInputVariables();
};

/*
  Listen to the switch presses from the joystick.
  
  If the joystick switch was pressed, look into the
  current letter the user is pointing at in the alphabet,
  append it to the user input only if the maximum input size
  was not reached yet.

  Display the user input on line 1 of the LCD.
*/
void MenuInput::userInputHandler(LiquidCrystal lcd, Joystick joystick, const int maxInput, const char* userInput[], const char* userAlphabet[]) {
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
      userInput[currentInputCursorPosition] = userAlphabet[currentAlphabetCursorPosition];
      currentInputCursorPosition += 1;
    }
  }
  
  // display the user input
  for (int i = 0; i < currentInputCursorPosition; i++) {
    lcd.setCursor(userInputStartPosition + i, 0);
    lcd.print(userInput[i]);
  }
};

/*
  Listen to the joystick movement. Depending on
  the direction it is pointing at (left or right), 
  move from the current letter to the pointed letter.

  The current letter of the alphabet is blinking.

  If the movement goes beyond boundaries, shift the
  alphabet one position (to left or right).
*/
void MenuInput::userAlphabetHandler(LiquidCrystal lcd, Joystick joystick, const char* userAlphabet[], const int leftBoundary, const int rightBoundary) {
  // if the joystick moved to the left and the cursor position
  // will not move out of the left boundary of the alphabet
  if (joystick.direction == joystickLeft && currentAlphabetCursorPosition > leftBoundary) {
    currentAlphabetCursorPosition -= 1;

    // if the cursor position moved beyond the right boundary of the LCD 
    if (currentAlphabetCursorPosition < alphabetPositionBoundary) {
      alphabetPositionBoundary -= 1;
    }
  } 

  // if the joystick moved to the right and the cursor position
  // will not move out of the right boundary of the alphabet
  if (joystick.direction == joystickRight && currentAlphabetCursorPosition < rightBoundary) {
    currentAlphabetCursorPosition += 1;

    // if the cursor position moved beyond the left boundary of the LCD 
    if (currentAlphabetCursorPosition >= alphabetPositionBoundary + 16) {
      alphabetPositionBoundary += 1;
    }
  }

  // display the alphabet and make the element positioned at
  // the cursor position blink
  for (int i = 0; i < min(rightBoundary, 16); i++) {
    if (i + alphabetPositionBoundary == currentAlphabetCursorPosition) {
      // blink the cursor
      displayBlinkingChar(lcd, userAlphabet[currentAlphabetCursorPosition], 1, i);
    } else {
      // display the characters of the alphabet
      lcd.setCursor(i, 1);
      lcd.print(userAlphabet[i + alphabetPositionBoundary]);
    }   
  }
};

/*
  Reset the variables to be reusable for new
  alphabets or user inputs.
*/
void MenuInput::resetInputVariables(){
  currentAlphabetCursorPosition = 0;
  currentInputCursorPosition = 0;
  alphabetPositionBoundary = 0;
};

#endif