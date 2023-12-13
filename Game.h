#pragma once
#ifndef GAME_H
#define GAME_H

#include <LiquidCrystal.h>
#include <LedControl.h>

#include "CustomCharacters.h"
#include "MenuDisplay.h"
#include "JoyStick.h"
#include "Player.h"
#include "Note.h"
#include "DrNocturne.h"
#include "Utils.h"

const int gameSpecialMomentsInterval = 2000;
const byte heartsStartPosition = 13;
const byte notesNeedForWin = 6;

struct Game{
  // time since the game started
  unsigned int time;
  // last time when the time was incremented
  unsigned long lastTimeIncrement;
  byte notes;
  byte lives;

  bool gameIsRunning = true;
  unsigned long gameEndingTime = 0;
  unsigned long gameSpecialMoments = 0;
  byte gameEndedMenuArrow = 0; 

  Player player;
  Note note;
  DrNocturne doctor;

  Game(LedControl &lc): time(0), notes(0), lives(3), player(lc){
    lastTimeIncrement = millis();
  }

  // functions to control the state of game
  void checkPlayerFoundNote(LiquidCrystal &lcd);
  void checkPlayerWasFoundByDoctor(LiquidCrystal &lcd);
  void checkPlayerWon(LedControl &lc, LiquidCrystal &lcd);
  void checkPlayerLost(LedControl &lc, LiquidCrystal &lcd);

  // functions to display the game on the LCD
  int play(LedControl &lc, LiquidCrystal &lcd, Joystick &joystick);

  // functions to display game status while running
  void displayGameRunningMenu(LedControl &lc, LiquidCrystal &lcd);
  void displayLives(LiquidCrystal &lcd);
  void displayNotes(LiquidCrystal &lcd);
  void displayLevel(LiquidCrystal &lcd);
  void increaseTime();
  void displayTime(LiquidCrystal &lcd);
  
  // functions to handle end of the game
  int displayGameEndedMenu(LedControl &lc, LiquidCrystal &lcd, Joystick &joystick);
  void displayGameEndedMessage(LiquidCrystal &lcd);
  int gameEndedMenuHandler(LedControl &lc, LiquidCrystal &lcd, Joystick &joystick);
  
  // function to reset the game
  void resetGame(LedControl &lc);
};

void Game::checkPlayerFoundNote(LiquidCrystal &lcd){
  // if the player and the note are in different rooms, exit
  if (player.currentRoom != note.currentRoom)
    return;

  // if the player and the note are not on the same position, exit
  if (player.row != note.row || player.column != note.column) 
    return;

  // if this section of the function was reached, it means
  // that the player has found the note
  notes += 1;

  // if the player already collected 3 notes,
  // make sure the note spawns in a different room 
  // from the room the player is currently in
  if (notes >= 3) {
    note.spawnNoteDifferentRoom(player.currentRoom);
  } 
  // otherwise, just spawn the note randomly
  else {
    note.spawnNoteRandomly();
  }

  // when the player reaches 2 / 4 notes, a special message
  // will be displayed on the LCD
  if (notes == 2 || notes == 4) {
    lcd.clear();
    gameSpecialMoments = millis();
  }

  // if the user reached 4 notes, level up Dr. Nocturne
  // and spawn him randomly
  if (notes == 4) {
    doctor.levelUp();
  }

  // if the user reached 2 notes, spawn Dr. Nocturne,
  // and until reaching 4 notes, the Dr. spawns randomly
  if (notes >= 2 && notes <= 3) {
    doctor.spawnDoctorRandomly();
    doctor.isWaiting = true;
  }

  // for the last two notes, the doctor will spawn in the
  // same room with the player
  if (notes >= 4 && notes <= 5) {
    doctor.spawnDoctorSameRoom(player.currentRoom);
    doctor.isWaiting = true;
  }
}

void Game::checkPlayerWasFoundByDoctor(LiquidCrystal &lcd){
  // if the player and the doctor are in the same room,
  // in the same position, it means the player was found
  if (player.currentRoom == doctor.currentRoom && player.row == doctor.row && player.column == doctor.column) {
    // firstly clear the lcd, than
    // decrease the number of lives
    lcd.clear();
    lives -= 1;
    // make the doctor inactive
    doctor.isWaiting = false;
    doctor.isChasing = false;
  }
}

void Game::checkPlayerWon(LedControl &lc, LiquidCrystal &lcd){
  // if the number of notes reached 5, it means the player won
  if (notes == notesNeedForWin) {
    // clear the matrix
    resetMatrix(lc);
    // clear the menu LCD
    lcd.clear();

    gameEndingTime = millis();
    gameIsRunning = false;
    player.isWinning = true;
  }
}

void Game::checkPlayerLost(LedControl &lc, LiquidCrystal &lcd){
  // if the player has no lives left, that means he lost
  if (lives == 0) {
    // clear the matrix
    resetMatrix(lc);
    // clear the menu LCD
    lcd.clear();

    gameEndingTime = millis();
    gameIsRunning = false;
  }
}

/*
  If the game is running, display the game status
  on the LCD screen. Also, listen to joystick movements
  from the player, check if it has found any notes and if
  it might have won.

  Otherwise, display a game ending message.
*/
int Game::play(LedControl &lc, LiquidCrystal &lcd, Joystick &joystick){
  // increase time constantly
  increaseTime();
  
  if (gameIsRunning) {
    displayGameRunningMenu(lc, lcd);
    
    // listents to the position change of the player
    player.positionWatcher(lc, joystick);

    if (doctor.isWaiting == true) {
      doctor.isWaitingToChase(player);
      doctor.display(lc, player);
    } 

    if (doctor.isChasing == true) {
      // if the player escaped from the room where the
      // doctor was, the doctor stops chasing
      if (player.currentRoom != doctor.currentRoom) {
        doctor.isChasing = false;
      }

      // doctor is chasing the player
      doctor.chase(lc, player);
      // check if the player was found by the doctor
      checkPlayerWasFoundByDoctor(lcd);
      
      doctor.display(lc, player);
    }

    if (doctor.isWaiting == false && doctor.isChasing == false) {
      note.display(lc, player);
      // check if the player found any notes
      checkPlayerFoundNote(lcd);
    }

    // check if the player found enough notes
    checkPlayerWon(lc, lcd);
    // check if the player lost
    checkPlayerLost(lc, lcd);

    return -1;
  } else {
    return displayGameEndedMenu(lc, lcd, joystick);
  }
};

void Game::displayGameRunningMenu(LedControl &lc, LiquidCrystal &lcd){
  if ((millis() - gameSpecialMoments) < gameSpecialMomentsInterval && notes == 2) {
    displayMessageInCenter(lcd, "Dr. Nocturne", 0);
    displayMessageInCenter(lcd, "was spawned...", 1);
    return;
  }

  if ((millis() - gameSpecialMoments) < gameSpecialMomentsInterval && notes == 4) {
    displayMessageInCenter(lcd, "Dr. Nocturne", 0);
    displayMessageInCenter(lcd, "is faster...", 1);
    return;
  }

  if ((millis() - gameSpecialMoments) >= gameSpecialMomentsInterval && (millis() - gameSpecialMoments) <= gameSpecialMomentsInterval + 100) {
    lcd.clear();
  }

  displayLives(lcd);
  displayNotes(lcd);
  displayLevel(lcd);
  displayTime(lcd);

  player.display(lc);
};

void Game::displayLives(LiquidCrystal &lcd){
  for(int i = 0; i < lives; i++) {
    lcd.setCursor(heartsStartPosition + i, 0);
    lcd.write(skullIndex);
  }
};

void Game::displayLevel(LiquidCrystal &lcd){
  lcd.setCursor(0, 0);
  lcd.print("LVL");

  lcd.setCursor(3, 0);
  lcd.print(doctor.level);
}

void Game::displayNotes(LiquidCrystal &lcd){
  lcd.setCursor(0, 1);
  lcd.print("Notes: ");

  // 7 is the length of "Notes "
  lcd.setCursor(7, 1);
  lcd.print(notes);
};

void Game::displayTime(LiquidCrystal &lcd){
  Serial.println(time);
  displayTimeFromSeconds(lcd, time, 6, 0);
};

void Game::increaseTime(){
  // if a 1000 ms have passed, it means a second,
  // so the time will be incremented by 1
  if ((millis() - lastTimeIncrement) >= 1000) {
    lastTimeIncrement = millis();
    time += 1;
  }
}


/*
  After the game has ended, display an ending message
  with the result for 3 seconds.

  Afterwards, a transition of 0.5s will occur.

  Finally, the user will be prompted a intermediate menu
  to choose from: to play again OR to return to the main menu.
*/
int Game::displayGameEndedMenu(LedControl &lc,  LiquidCrystal &lcd, Joystick &joystick){
  // for 3 seconds the game endings message will be displayed
  if ((millis() - gameEndingTime) < 3000) {
    displayGameEndedMessage(lcd);
  } 
  // after that, a smooth 500 ms transition will be made,
  // in which the LCD will be cleared
  else if ((millis() - gameEndingTime) >= 3000 && (millis() - gameEndingTime) <= 3500) {
    lcd.clear();
  } 
  // an intermediate menu will be displayed, in case
  // the user wants to play again
  else {
    lcd.setCursor(0, gameEndedMenuArrow);
    lcd.write(arrowIndex);

    lcd.setCursor(2, 0);
    lcd.print("Play again");

    lcd.setCursor(2, 1);
    lcd.print("Back");

    return gameEndedMenuHandler(lc, lcd, joystick);
  }

  return -1;
}

/*
  Display this message when the game has ended,
  related to the state of the game: win & time / lose
*/
void Game::displayGameEndedMessage(LiquidCrystal &lcd){
  if (player.isWinning) {
    displayMessageInCenter(lcd, "You escaped!", 0);
  } else {
    displayMessageInCenter(lcd, "You died!", 0);
  }
  displayTimeFromSeconds(lcd, time, 5, 1);
}

/*
  Handle the intermediate menu: if the user
  chooses to play again or to return to the main menu
*/
int Game::gameEndedMenuHandler(LedControl &lc, LiquidCrystal &lcd, Joystick &joystick){
  // handle up movement of the joystick
  if (joystick.direction == joystickUp && gameEndedMenuArrow == 1) {
    lcd.setCursor(0, gameEndedMenuArrow);
    lcd.write(" ");
    
    gameEndedMenuArrow -= 1;
  }

  // handle down movement of the joystick
  if (joystick.direction == joystickDown && gameEndedMenuArrow == 0) {
    lcd.setCursor(0, gameEndedMenuArrow);
    lcd.write(" ");

    gameEndedMenuArrow += 1;
  }

  // handle the button press for the joystick
  if (joystick.currentSwitchStateChanged == HIGH) {
    // no matter where the user is pointing, clear the LCD
    // and reset the game variables
    lcd.clear();
    resetGame(lc);

    if (gameEndedMenuArrow == 0) {
      // start the game again and enter the game menu
      gameIsRunning = true;
      return 11;
    } else {
      // go back to the main menu
      return 1;
    }
  }

  return -1;
}


/*
  Reset all the game variables
*/
void Game::resetGame(LedControl &lc){
  time = 0;
  notes = 0;
  lives = 3;
  lastTimeIncrement = millis();

  player.initPlayer(lc);
  note.spawnNoteRandomly();
};

#endif