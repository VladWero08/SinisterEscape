#pragma once
#ifndef GAME_H
#define GAME_H

#include <LiquidCrystal.h>
#include <LedControl.h>

#include "EEPROM.h"

#include "CustomCharacters.h"
#include "MenuDisplay.h"
#include "JoyStick.h"
#include "Player.h"
#include "Note.h"
#include "DrNocturne.h"
#include "Highscores.h"
#include "Utils.h"

// duration of transition between messages
const byte transitionTime = 100;
// duration of special messages during the game, on the display
const int gameSpecialMomentsTimeInterval = 3000;
// duration of message after the game has eneded
const int gameEndingTimeInterval = 3500;
// column position of the hearts in the live game menu
const byte heartsStartPosition = 13;
// column position of the time in the live game menu
const byte timePosition = 6;
const byte notesNeedForWin = 6;

struct Game{
  Player player;
  Note note;
  DrNocturne doctor;
  
  // time since the game started
  unsigned long time;
  // last time when the time was incremented
  unsigned long lastTimeIncrement;
  // last time a note has been found
  unsigned long lastNoteFound;
  // last time the player has died
  unsigned long lastDeath;

  bool isInPause = false;
  bool isRunning = true;
  bool isDisplayingEndMessage = false;

  unsigned long gameEndingTime = 0;
  unsigned long gameSpecialMomentsTime = 0;
  byte gameEndedMenuArrow = 0; 

  Game(LedControl &lc): time(0), player(lc){
    lastTimeIncrement = millis();
  }

  // functions to control the state of game
  void checkPlayerFoundNote(LiquidCrystal &lcd);
  void checkPlayerWasFoundByDoctor(LiquidCrystal &lcd);
  void checkPlayerWon(LedControl &lc, LiquidCrystal &lcd);
  void checkPlayerLost(LedControl &lc, LiquidCrystal &lcd);
  bool checkPlayerGotHighscore();

  // functions to display the game on the LCD
  void play(LedControl &lc, LiquidCrystal &lcd, Joystick &joystick);

  // functions to display game status while running
  void displayGameRunningMenu(LedControl &lc, LiquidCrystal &lcd);
  void displayTime(LiquidCrystal &lcd, const int line);
  void increaseTime();
  
  // function to handle pause mode
  void gamePauseHandler(LiquidCrystal &lcd, Joystick &joystick);
  void displayPauseMode(LiquidCrystal &lcd);

  // functions to handle end of the game
  void displayGameEnded(LedControl &lc, LiquidCrystal &lcd);
  void displayGameEndedMessage(LiquidCrystal &lcd);  
  void displayPlayerGotHighscore(LiquidCrystal &lcd);
  void displayPlayerEntersName(LiquidCrystal &lcd);

  // function to reset the game
  void reset(LedControl &lc);
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
  player.notes += 1;
  lastNoteFound = millis();

  // if the player already collected 3 notes,
  // make sure the note spawns in a different room 
  // from the room the player is currently in
  if (player.notes >= 3) {
    note.spawnNoteDifferentRoom(player.currentRoom);
  } 
  // otherwise, just spawn the note randomly
  else {
    note.spawnNoteRandomly();
  }

  // when the player reaches 2 / 4 notes, a special message
  // will be displayed on the LCD and the level will be increased
  if (player.notes == 2 || player.notes == 4) {
    lcd.clear();
    doctor.levelUp();
    gameSpecialMomentsTime = millis();
  }

  // if the user reached 2 notes, spawn Dr. Nocturne,
  // and until reaching 4 notes, the Dr. spawns randomly
  if (player.notes >= 2 && player.notes <= 3) {
    doctor.spawnDoctorRandomly();
    doctor.isWaiting = true;
  }

  // for the last two notes, the doctor will spawn in the
  // same room with the player
  if (player.notes >= 4 && player.notes <= 5) {
    doctor.spawnDoctorSameRoom(player);
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
    player.lives -= 1;
    // make the doctor inactive
    doctor.isWaiting = false;
    doctor.isChasing = false;
    lastDeath = millis();
  }
}

void Game::checkPlayerWon(LedControl &lc, LiquidCrystal &lcd){
  // check if the number of notes reached the number
  // needed for the player to win
  if (player.notes == 0) {
    gameEndingTime = millis();
    // clear the matrix
    resetMatrix(lc);
    // clear the menu LCD
    lcd.clear();
    
    isRunning = false;
    isDisplayingEndMessage = true;

    player.isWinning = true;
    // check if the player got an highscore
    player.hasHighscore = checkPlayerGotHighscore();
  }
}

void Game::checkPlayerLost(LedControl &lc, LiquidCrystal &lcd){
  // if the player has no lives left, it means that he lost
  if (player.lives == 0) {
    gameEndingTime = millis();
    // clear the matrix
    resetMatrix(lc);
    // clear the menu LCD
    lcd.clear();

    isRunning = false;
    isDisplayingEndMessage = true;
  }
}

bool Game::checkPlayerGotHighscore(){
    // if highscores have not been completed
  if (highscoresRegistered < 3) {
    highscoresRegistered += 1;
    return true;
  }

  // loop the current highscores and check if the player
  // surpassed other scores
  for (int i = 0; i < highscoresRegistered; i++) {
    // check if player surpassed ith score
    if (time < highscores[i]) {
      return true;
    }
  }

  return false;
}


/*
  If the game is running, display the game status
  on the LCD screen. Also, listen to joystick movements
  from the player, check if it has found any notes and if
  it might have won.

  Otherwise, display a game ending message.
*/
void Game::play(LedControl &lc, LiquidCrystal &lcd, Joystick &joystick){  
  // before playing the game, check if the user pressed
  // the SW in the joystick aka made a pause
  gamePauseHandler(lcd, joystick);

  if (isInPause) {
    displayPauseMode(lcd);
    return;
  }

  if (isRunning) {
    // display the menu on the LCD constantly
    displayGameRunningMenu(lc, lcd);
    // listens to the position change of the player
    player.movementWatcher(lc, joystick);

    // if the doctor waits to chase the player
    if (doctor.isWaiting == true) {
      doctor.isWaitingToChase(player);
      doctor.display(lc, player);
    } 

    // if the doctor is chasing the player
    if (doctor.isChasing == true) {
      // if the player escaped from the room where the
      // doctor was, the doctor stops chasing and becomes inactive
      if (player.currentRoom != doctor.currentRoom) {
        doctor.isChasing = false;
      }

      // doctor is chasing the player
      doctor.chase(lc, player);
      // check if the player was found by the doctor
      checkPlayerWasFoundByDoctor(lcd);

      doctor.display(lc, player);
    }

    // if the doctor is inactive, display the note and
    // check if it was found by the player
    if (doctor.isWaiting == false && doctor.isChasing == false) {
      note.display(lc, player);
      // check if the player found any notes
      checkPlayerFoundNote(lcd);
    }

    // at every step, check if 
    // the player is winning or losing
    checkPlayerWon(lc, lcd);
    checkPlayerLost(lc, lcd);

    // increase time and display player constantly
    increaseTime();
    player.display(lc);
  } 
    
  if (isDisplayingEndMessage) {
    displayGameEnded(lc, lcd);
  }
};

void Game::displayGameRunningMenu(LedControl &lc, LiquidCrystal &lcd){
  // display a special message when the player reached level 2
  if ((millis() - gameSpecialMomentsTime) < gameSpecialMomentsTimeInterval && player.notes == 2) {
    displayMessageInCenter(lcd, "Dr. Nocturne", 0);
    displayMessageInCenter(lcd, "was spawned...", 1);
    return;
  }

  // display a special message when the player reached level 3
  if ((millis() - gameSpecialMomentsTime) < gameSpecialMomentsTimeInterval && player.notes == 4) {
    displayMessageInCenter(lcd, "Dr. Nocturne", 0);
    displayMessageInCenter(lcd, "is faster...", 1);
    return;
  }

  // make a smooth transition between the special message
  // and the game menu
  if ((millis() - gameSpecialMomentsTime) >= gameSpecialMomentsTimeInterval 
  && (millis() - gameSpecialMomentsTime) <= gameSpecialMomentsTimeInterval + transitionTime) {
    lcd.clear();
  }

  // display the usual game menu
  player.displayLives(lcd, heartsStartPosition, 0);
  player.displayNotes(lcd);
  doctor.displayLevel(lcd);
  displayTime(lcd, 0);
};

void Game::displayTime(LiquidCrystal &lcd, const int line){
  displayTimeFromSeconds(lcd, time, timePosition, line);
};

void Game::increaseTime(){
  // if 1000 ms have passed, it means a second,
  // so the time will be incremented by 1
  if ((millis() - lastTimeIncrement) >= 1000) {
    lastTimeIncrement = millis();
    time += 1;
  }
}


void Game::gamePauseHandler(LiquidCrystal &lcd, Joystick &joystick){
  // if the SW button was pressed, toggle the pause variable
  if (joystick.currentSwitchStateChanged == HIGH) {
    lcd.clear();
    // reset the last time when the time was incremented because it does
    // not need to take into account the MS during the pause mode
    lastTimeIncrement = millis();

    // toggle the pause variable
    isInPause = !isInPause;
  } 
}

void Game::displayPauseMode(LiquidCrystal &lcd){
  displayMessageInCenter(lcd, " PAUSE", 1);
  displayTime(lcd, 0);

  player.displayLives(lcd, heartsStartPosition, 0);
  doctor.displayLevel(lcd);
}


/*
  After the game has ended, display an ending message
  with the result for 3 seconds.

  Afterwards, a transition of 0.5s will occur.

  Finally, the user will be prompted a intermediate menu
  to choose from: to play again OR to return to the main menu.
*/
void Game::displayGameEnded(LedControl &lc,  LiquidCrystal &lcd){
  // for 3 seconds the game endings message will be displayed
  if ((millis() - gameEndingTime) < gameEndingTimeInterval) {
    displayGameEndedMessage(lcd);
    return;
  } 
  // after that, a smooth 500 ms transition will be made,
  // in which the LCD will be cleared
  else if ((millis() - gameEndingTime) >= gameEndingTimeInterval 
          && (millis() - gameEndingTime) <= gameEndingTimeInterval + transitionTime) {
    lcd.clear();
    return;
  } 

  if ((millis() - gameEndingTime) < (gameEndingTimeInterval * 2 + transitionTime)
      && player.hasHighscore) { 
      displayPlayerGotHighscore(lcd);
      return;
  } 
  // after that, a smooth 500 ms transition will be made,
  // in which the LCD will be cleared
  else if ((millis() - gameEndingTime) >= (gameEndingTimeInterval * 2 + transitionTime)
          && (millis() - gameEndingTime) <= 2 * (gameEndingTimeInterval + transitionTime)) {
    lcd.clear();
    return;
  } 

  if ((millis() - gameEndingTime) < (gameEndingTimeInterval * 3 + transitionTime)
      && player.hasHighscore
      && !player.hasUserName) { 
      displayPlayerEntersName(lcd);
      return;
  } 

  lcd.clear();
  isDisplayingEndMessage = false;
}

/*
  Display this message when the game has ended,
  related to the state of the game: win / lose, & time.
*/
void Game::displayGameEndedMessage(LiquidCrystal &lcd){
  if (player.isWinning) {
    displayMessageInCenter(lcd, "You escaped!", 0);
  } else {
    displayMessageInCenter(lcd, "You died!", 0);
  }
  displayTimeFromSeconds(lcd, time, 5, 1);
}

void Game::displayPlayerGotHighscore(LiquidCrystal &lcd){
  displayMessageInCenter(lcd, "New highscore!", 0);
  displayTimeFromSeconds(lcd, time, 5, 1);
}

void Game::displayPlayerEntersName(LiquidCrystal &lcd){
  displayMessageInCenter(lcd, "Who defeated", 0);
  displayMessageInCenter(lcd, "Dr.Nocturne?", 1);
}

/*
  Reset all the game variables.
*/
void Game::reset(LedControl &lc){
  time = 0;
  lastTimeIncrement = millis();
  
  isRunning = true;

  doctor.reset();
  player.reset(lc);
  note.spawnNoteRandomly();
};

#endif