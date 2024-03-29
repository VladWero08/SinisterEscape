#pragma once
#ifndef CUSTOM_CHARACTERS_H
#define CUSTOM_CHARACTERS_H

#include <LiquidCrystal.h>

const byte skull[8] = {
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b11011,
  0b11111,
  0b01110,
  0b01110
};
const uint8_t skullIndex = (uint8_t) 0;

const byte arrow[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00010,
	0b11111,
	0b00010,
	0b00000,
	0b00000
};
const uint8_t arrowIndex = (uint8_t) 1;

const byte verify[8] = {
	0b00000,
	0b00000,
	0b00001,
	0b00011,
	0b10110,
	0b11100,
	0b01000,
	0b00000
};
const uint8_t verifyIndex = (uint8_t) 2;

const byte deleteX[8] = {
	0b00000,
	0b00000,
	0b10001,
	0b01010,
	0b00100,
	0b01010,
	0b10001,
	0b00000
};
const uint8_t deleteIndex = (uint8_t) 3;

const byte exitArrow[8] = {
	0b11110,
	0b11000,
	0b10100,
	0b10010,
	0b00001,
	0b00001,
	0b00010,
	0b00000
};
const uint8_t exitIndex = (uint8_t) 4;

const byte heart[8] = {
	0b00000,
	0b01010,
	0b11111,
	0b11111,
	0b11111,
	0b01110,
	0b00100,
	0b00000
};
const uint8_t heartIndex = (uint8_t) 5;

// number of custom characters that will be created
const byte numberOfCustomChars = 6;
// array with all custom characters
const byte *customChars[numberOfCustomChars] = {
  skull, 
  arrow, 
  verify, deleteX, exitArrow, 
  heart
};

// array with each custom character's index
const uint8_t customCharsIndexes[numberOfCustomChars] = {
  skullIndex, 
  arrowIndex, 
  verifyIndex, deleteIndex, exitIndex, 
  heartIndex
};

/*
  Create each character that has been declared.
*/
void initializeCustomChars(LiquidCrystal &lcd) {
  for (int i = 0; i < numberOfCustomChars; i++) {
    lcd.createChar(customCharsIndexes[i], customChars[i]);
  }
};

#endif