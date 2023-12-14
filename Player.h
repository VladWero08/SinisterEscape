#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <LedControl.h>
#include <LiquidCrystal.h>

#include "JoyStick.h"
#include "Rooms.h"

// interval in ms between player's blinking position 
const byte playerBlinkingInterval = 50;

struct Player{
  byte row;
  byte column;
  byte currentRoom;

  byte notes;
  byte lives;

  // controls the player's visibility
  bool isDisplayed;
  // controls the winning state of the player
  bool isWinning;
  // controls if the player had a highscore
  bool hasHighscore;
  bool hasUserName;
  // last time when isDisplayed changed its state, in ms
  unsigned long lastDisplayBlinking;

  Player(LedControl &lc): notes(0), lives(3){
    // start from position 1, 1 in the room
    row = 1;
    column = 1;

    // choose the room randomly and  
    // display the room on the matrix
    currentRoom = random(0, roomsSize);
    setRoom(lc, currentRoom);

    // the player starts from a losing state
    isWinning = false;
    // the player is being displayed on the matrix
    isDisplayed = true;
    // the player needs to earn the highscores, so its false at start
    hasHighscore = false;
    hasUserName = false;
  };

  // functions to handle player's movement
  void movementWatcher(LedControl &lc, Joystick &joystick);
  void movementUpHandler(LedControl &lc);
  void movementDownHandler(LedControl &lc);
  void movementLeftHandler(LedControl &lc);
  void movementRightHandler(LedControl &lc);

  // function to display the player in the room
  void display(LedControl &lc);
  void displayNotes(LiquidCrystal &lcd);
  void displayLives(LiquidCrystal &lcd, byte heartsStartPosition);

  // function to initate the players position;
  void reset(LedControl &lc);
};

/*
  Listens to the joystick movement, and
  depending on the direction, handles that movement.
*/
void Player::movementWatcher(LedControl &lc, Joystick &joystick){
  if (joystick.direction == joystickUp) {
    movementUpHandler(lc);
  }

  if (joystick.direction == joystickDown) {
    movementDownHandler(lc);
  }

  if (joystick.direction == joystickLeft) {
    movementLeftHandler(lc);
  }

  if (joystick.direction == joystickRight) {
    movementRightHandler(lc);
  }
};

void Player::movementUpHandler(LedControl &lc){
  if (row > 0) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row - 1][column] == true) {
      return;
    }

    // otherwise, unset the current position
    lc.setLed(0, row, column, false);
    // move up
    row -= 1;
  } 
  // if the player is already on the first row
  // and tries to move up, it means it wants to access a door
  else if (row == 0) {  
    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickUp];
    setRoom(lc, currentRoom);

    // if the player moved up through a door, that means
    // it is now currently on the last row of the new room
    row = matrixSize - 1;
  }
};

void Player::movementDownHandler(LedControl &lc){
  if (row < matrixSize - 1) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row + 1][column] == true) {
      return;
    }

    // otherwise, unset the current position
    lc.setLed(0, row, column, false);
    // move down
    row += 1;
  } 
  // if the player is already on the last row
  // and tries to move down, it means it wants to access a door
  else if(row == matrixSize - 1) {

    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickDown];
    setRoom(lc, currentRoom);
    
    // if the player moved down through a door, that means
    // it is now currently on the first row of the new room
    row = 0;
  }
};

void Player::movementLeftHandler(LedControl &lc){
  if (column > 0) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row][column - 1] == true) {
      return;
    }
    
    // otherwise, unset the current position
    lc.setLed(0, row, column, false);
    // move to the left
    column -= 1;
  } 
  // if the player is already on the first column
  // and tries to move to the left, it means it wants to access a door
  else if (column == 0) {
    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickLeft];
    setRoom(lc, currentRoom);
  
    // if the player moved to the left through a door, that means
    // it is now currently on the last column of the new room
    column = matrixSize - 1;
  }
};

void Player::movementRightHandler(LedControl &lc){
  if (column < matrixSize - 1) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row][column + 1] == true) {
      return;
    }

    // otherwise, unset the current position
    lc.setLed(0, row, column, false);
    // move to the right
    column += 1;
  } 
  // if the player is already on the first column
  // and tries to move to the left, it means it wants to access a door
  else if (column == matrixSize - 1) {

    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickRight];
    setRoom(lc, currentRoom);

    // if the player moved to the right through a door, that means
    // it is now currently on the first column of the new room
    column = 0;
  }
};

/*
  Display the current position of the player in
  a blinking mode. Once 50 ms. make the player blink
  so it is easily distinguishable.
*/
void Player::display(LedControl &lc){
  if ((millis() - lastDisplayBlinking) > playerBlinkingInterval) {
    lastDisplayBlinking = millis();
    isDisplayed = !isDisplayed;
    lc.setLed(0, row, column, isDisplayed);
  }
};

void Player::displayNotes(LiquidCrystal &lcd){
  lcd.setCursor(0, 1);
  lcd.print("Notes: ");

  // 7 is the length of "Notes "
  lcd.setCursor(7, 1);
  lcd.print(notes);
};

void Player::displayLives(LiquidCrystal &lcd, byte heartsStartPosition){
  for(int i = 0; i < lives; i++) {
    lcd.setCursor(heartsStartPosition + i, 0);
    lcd.write(skullIndex);
  }
};

void Player::reset(LedControl &lc){
  row = 1;
  column = 1;

  currentRoom = random(0, roomsSize);
  setRoom(lc, currentRoom);

  notes = 0;
  lives = 3;
  
  isWinning = false;
  isDisplayed = true;
  hasHighscore = false;
}

#endif