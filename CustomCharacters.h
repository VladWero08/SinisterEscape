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

const byte exitArrow[8] = {
	0b11110,
	0b11000,
	0b10100,
	0b10010,
	0b00001,
	0b00001,
	0b00010,
	0b01100
};

// number of custom characters that will be created
const byte numberOfCustomChars = 5;
// array with all custom characters
const byte *customChars[numberOfCustomChars] = {skull, arrow, deleteX, verify, exitArrow};

/*
  Create each character that has been declared.
*/
void initializeCustomChars(LiquidCrystal &lcd) {
  for (int i = 0; i < numberOfCustomChars; i++) {
    lcd.createChar(i, customChars[i]);
  }
}

#endif