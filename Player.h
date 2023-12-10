#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "JoyStick.h"
#include <LedControl.h>

const byte matrixSize = 8;

struct Player{
  byte line;
  byte column;

  Player(): line(0), column(0){};

  void positionWatcher(Joystick joystick);
  void display(LedControl lc);
};

void Player::positionWatcher(Joystick joystick){
  if (joystick.direction == joystickUp && line > 0) {
    line -= 1;
  }

  if (joystick.direction == joystickDown && line < matrixSize - 1) {
    line += 1;
  }

  if (joystick.direction == joystickLeft && column > 0) {
    column -= 1;
  }

  if (joystick.direction == joystickRight && column < matrixSize - 1) {
    column += 1;
  }
}

void Player::display(LedControl lc){
  lc.setLed(0, line, column, true);
}

#endif