#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "JoyStick.h"
#include "Rooms.h"
#include <LedControl.h>

// interval in ms between player's position blinking
const byte playerBlinkingInterval = 50;

struct Player{
  byte row;
  byte column;
  byte currentRoom;

  // controls if the player's position is visible
  byte isActive;
  // last time when isActive changes its states, in ms
  unsigned long lastBlinking;

  Player(): row(1), column(1), isActive(true){
    currentRoom = random(0, roomsSize);
  };

  // functions to handle player's movement
  void positionWatcher(LedControl, Joystick joystick);
  void movementUpHandler(LedControl lc);
  void movementDownHandler(LedControl lc);
  void movementLeftHandler(LedControl lc);
  void movementRightHandler(LedControl lc);

  // functions to display on the matrix
  void setPlayerPosition(LedControl lc);
};

/*
  Listens to the joystick movement, and
  depending on the direction, handle that movement.
*/
void Player::positionWatcher(LedControl lc, Joystick joystick){
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
}

void Player::movementUpHandler(LedControl lc){
  if (row > 0) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row - 1][column] == true) {
      return;
    }

    // unset the current position
    lc.setLed(0, row, column, false);
    // move up
    row -= 1;
  } else if (row == 0) {
    // if the player is already on the first row
    // and tries to move up, it means it wants to access a door
    
    // reset the matrix first
    resetMatrix(lc);

    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickUp];
    row = matrixSize - 1;
  }
}

void Player::movementDownHandler(LedControl lc){
  if (row < matrixSize - 1) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row + 1][column] == true) {
      return;
    }

    // unset the current position
    lc.setLed(0, row, column, false);
    // move down
    row += 1;
  } else if(row == matrixSize - 1) {
    // if the player is already on the last row
    // and tries to move down, it means it wants to access a door

    // reset the matrix first
    resetMatrix(lc);

    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickDown];
    row = 0;
  }
}

void Player::movementLeftHandler(LedControl lc){
  if (column > 0) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row][column - 1] == true) {
      return;
    }
    
    // unset the current position
    lc.setLed(0, row, column, false);
    // move to the left
    column -= 1;
  } else if (column == 0) {
    // if the player is already on the first column
    // and tries to move to the left, it means it wants to access a door

    // reset the matrix first
    resetMatrix(lc);

    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickLeft];
    column = matrixSize - 1;
  }
}

void Player::movementRightHandler(LedControl lc){
  if (column < matrixSize - 1) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row][column + 1] == true) {
      return;
    }

    // unset the current position
    lc.setLed(0, row, column, false);
    // move to the right
    column += 1;
  } else if (column == matrixSize - 1) {
    // if the player is already on the first column
    // and tries to move to the left, it means it wants to access a door

    // reset the matrix first
    resetMatrix(lc);

    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickRight];
    column = 0;
  }
}

/*
  Display the current position of the player.
*/
void Player::setPlayerPosition(LedControl lc){
  if ((millis() - lastBlinking) > playerBlinkingInterval) {
    lastBlinking = millis();
    isActive = !isActive;
    lc.setLed(0, row, column, isActive);
  }
}

#endif