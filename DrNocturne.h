#pragma once
#ifndef DR_NOCTURNE_H
#define DR_NOCTURNE_H

#include "Rooms.h"
#include "Player.h"
#include "Utils.h"

const int drNocturneActiveBlinkingInterval = 500;
const byte drNocturneInactiveBlinkingInterval = 100;
const int movementCooldownLevel1 = 1000;
const int movementCooldownLevel2 = 750;

struct DrNocturne{
  byte row;
  byte column;
  byte currentRoom;
  byte level;

  // controls the note's visibility
  bool isDisplay;
  // controls if Dr. Nocturne needs to wait for the player
  // to get closer
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

  // while the position generated is invalid,
  // generate new positions and check their validity
  while (rooms[currentRoom][row][column] == true){
    row = random(0, matrixSize);
    column = random(0, matrixSize);
  }
};



void DrNocturne::isWaitingToChase(Player player){
  // if Dr. Nocturne and the player are not in 
  // the same room, exit
  if (currentRoom != player.currentRoom) {
    isChasing = false;
    return;
  }

  // calculate the euclidean distance between Dr and the player
  float distance = euclideanDistance(player.row, player.column, row, column);

  // level 1: start following the player when 
  // the distance is at most 5
  if (level == 1 && distance <= 5) {
    isWaiting = false;
    isChasing = true;
    lastMovement = millis();
  }

  // level 2: start following the player when 
  // the distance is at most 3, which is harder than level 1
  if (level == 2 && distance <= 3) {
    isWaiting = false;
    isChasing = true;
    lastMovement = millis();
  }
}

void DrNocturne::chase(LedControl &lc, Player player){
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

  // set the current old position to false, 
  // to avoid letting the old position be active 
  // simultaneously with the new position
  lc.setLed(0, row, column, false);
  lastMovement = millis();

  // calculate the euclidean distance from each possible move
  // to the players current position
  float moveUp = 100, moveDown = 100, moveLeft = 100, moveRight = 100;

  // only generate the euclidean distance if the movement is a valid one,
  // aka it is not getting the doctor out of the matrix
  if (row - 1 >= 0) { 
    if (rooms[currentRoom][row - 1][column] == false) {
      moveUp = euclideanDistance(player.row, player.column, row - 1, column);
    }
  } 

  if (row + 1 <= 7) { 
    if (rooms[currentRoom][row + 1][column] == false) {
      moveDown = euclideanDistance(player.row, player.column, row + 1, column);
    }
  }
  
  if (column - 1 >= 0) { 
    if (rooms[currentRoom][row][column - 1] == false) {
      moveLeft = euclideanDistance(player.row, player.column, row, column - 1);
    }
  }
  
  if (column + 1 <= 7) {
    if (rooms[currentRoom][row ][column + 1] == false) {
      moveRight = euclideanDistance(player.row, player.column, row, column + 1);
    }
  }

  float optimalMove = min(min(min(moveUp, moveDown), moveLeft), moveRight);

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
  in the room.
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

void DrNocturne::reset(){
  isWaiting = false;
  isChasing = false;
  level = 1;
  spawnDoctorRandomly();
}



#endif