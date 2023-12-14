#pragma once
#ifndef DR_NOCTURNE_H
#define DR_NOCTURNE_H

#include <LiquidCrystal.h>

#include "Rooms.h"
#include "Player.h"
#include "Utils.h"

// variables to make a blinking effect
// for how many seconds is the Dr. visible
const int drNocturneActiveBlinkingInterval = 500;
// fot how many seconds is the Dr. invisible
const byte drNocturneInactiveBlinkingInterval = 100;

// interval time between movements on level 1
const int movementCooldownLevel1 = 1000;
// interval time between movements on level 2
const int movementCooldownLevel2 = 750;

struct DrNocturne{
  byte row;
  byte column;
  byte currentRoom;
  byte level;

  // controls the Dr's visibility
  bool isDisplay;
  // controls if Dr. Nocturne needs to wait 
  // for the player to get closer
  bool isWaiting;
  // controls if Dr. Nocturne is chasing the player
  bool isChasing;

  // last time when isDisplayed changes its states, in ms
  unsigned long lastDisplayBlinking;
  unsigned long lastMovement;

  DrNocturne(){
    isWaiting = false;
    isChasing = false;
    level = 1;
    spawnDoctorRandomly();
  }

  // functions to spawn the note
  void spawnDoctorRandomly();
  void spawnDoctorSameRoom(const byte playerRoom);
  void spawnInRoom();

  void isWaitingToChase(Player player);
  void chase(LedControl &lc, Player player);
  void levelUp();

  // function to display the note in the room
  void display(LedControl &lc, Player player);
  void displayLevel(LiquidCrystal &lcd);
  void reset();
};

/*
  Spawn Dr. Nocturne in a random room.
*/
void DrNocturne::spawnDoctorRandomly(){
  currentRoom = random(0, roomsSize);  
  spawnInRoom();
};

/*
  Spawn Dr. Nocturne in the same room as the player's room.
*/
void DrNocturne::spawnDoctorSameRoom(const byte playerRoom){
  currentRoom = playerRoom;
  spawnInRoom();
};

/*
  Generate a position in the currentRoom
  until it is a valid one. Spawn Dr. in that position.
*/
void DrNocturne::spawnInRoom(){
  row = random(0, matrixSize);
  column = random(0, matrixSize);

  // generate row and column until
  // the position is different from a wall 
  while (rooms[currentRoom][row][column] == true){
    row = random(0, matrixSize);
    column = random(0, matrixSize);
  }
};


/*
  Dr. Nocturne waits for the player to come closer. 
  The Euclidean distance between the player 
  and the Dr. needs to be:

  -> level 1: Dr. Nocturne is inactive
  -> level 2: <= 5
  -> level 3: <= 3

  When the player is close enough, Dr. Nocturne exits
  the waiting mode and enters in the chasing one.
*/
void DrNocturne::isWaitingToChase(Player player){
  // if Dr. Nocturne and the player are not in 
  // the same room, exit
  if (currentRoom != player.currentRoom) {
    isChasing = false;
    return;
  }

  // calculate the euclidean distance between Dr and the player
  float distance = euclideanDistance(player.row, player.column, row, column);

  // level 2: start following the player when 
  // the distance is at most 5
  if (level == 2 && distance <= 5) {
    // deactivate the waiting state
    isWaiting = false;
    // activate the chasing state
    isChasing = true;
    lastMovement = millis();
  }

  // level 3: start following the player when 
  // the distance is at most 3, which is harder than level 1
  if (level == 3 && distance <= 3) {
    // deactivate the waiting state
    isWaiting = false;
    // activate the chasing state
    isChasing = true;
    lastMovement = millis();
  }
}

void DrNocturne::chase(LedControl &lc, Player player){
  // depending on the level, Dr. Nocturne has a cooldown
  // between consecutive movements
  switch (level) {
    case 1:
      if ((millis() - lastMovement) < movementCooldownLevel1) {
        return;
      }
      break;
    case 2:
      if ((millis() - lastMovement) < movementCooldownLevel1) {
        return;
      }
      break;
    default:
      break;
  }

  // set the old position to false, 
  // to avoid letting the old position be active 
  // in the same time with the new position
  lc.setLed(0, row, column, false);
  lastMovement = millis();

  // try to calculate the euclidean distance from each possible move
  // to the player's current position; 
  // set them as 100, in case a movement
  // is not possible, its initial value cannot influence the minimum
  float moveUp = 100, moveDown = 100, moveLeft = 100, moveRight = 100;

  // calculate the euclidean distance if the movement is a valid one,
  // aka it is not getting the doctor out of the matrix
  if (row - 1 >= 0) { 
    if (rooms[currentRoom][row - 1][column] == false) {
      moveUp = euclideanDistance(player.row, player.column, row - 1, column);
    }
  } 

  if (row + 1 <= matrixSize - 1) { 
    if (rooms[currentRoom][row + 1][column] == false) {
      moveDown = euclideanDistance(player.row, player.column, row + 1, column);
    }
  }
  
  if (column - 1 >= 0) { 
    if (rooms[currentRoom][row][column - 1] == false) {
      moveLeft = euclideanDistance(player.row, player.column, row, column - 1);
    }
  }
  
  if (column + 1 <= matrixSize - 1) {
    if (rooms[currentRoom][row ][column + 1] == false) {
      moveRight = euclideanDistance(player.row, player.column, row, column + 1);
    }
  }

  // calculate the minimum between all 4 possible movements
  float optimalMove = min(min(min(moveUp, moveDown), moveLeft), moveRight);

  // execute the movement
  if (optimalMove == moveUp) {
    row -= 1;
  } else if (optimalMove == moveDown) {
    row += 1;
  } else if (optimalMove == moveLeft) {
    column -= 1;
  } else {
    column += 1;
  }
}

void DrNocturne::levelUp(){
  level += 1;
}

/*
  Display the current position of Dr. Nocturne
  in the room, only if he is active AND in the same
  room with the player.

  It is visible for 500 ms and invisible for 100,
  to make a blinking effect that is identical with the 
  note's blinking effect.
*/
void DrNocturne::display(LedControl &lc, Player player){
  // if the Doctor is not chasing, do not display his position
  if (isWaiting == false && isChasing == false) {
    return;
  }

  // if the note and the player are placed in different rooms, exit
  if (currentRoom != player.currentRoom)
    return;

  // depending on the state of the note,
  // check if the state should be toggled 
  if (isDisplay) {
    if ((millis() - noteActiveBlinkingInterval) > lastDisplayBlinking) {
      lastDisplayBlinking = millis();
      isDisplay = !isDisplay;
      lc.setLed(0, row, column, isDisplay);
    }
  } else {
    if ((millis() - noteInactiveBlinkingInterval) > lastDisplayBlinking) {
      lastDisplayBlinking = millis();
      isDisplay = !isDisplay;
      lc.setLed(0, row, column, isDisplay);
    }
  }
};

void DrNocturne::displayLevel(LiquidCrystal &lcd){
  lcd.setCursor(0, 0);
  lcd.print("LVL");

  lcd.setCursor(3, 0);
  lcd.print(level);
}

void DrNocturne::reset(){
  isWaiting = false;
  isChasing = false;
  level = 1;
  spawnDoctorRandomly();
}



#endif