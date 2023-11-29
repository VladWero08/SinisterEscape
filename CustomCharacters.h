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

// number of custom characters that will be created
const byte numberOfCustomChars = 2;
// array with all custom characters
const byte *customChars[numberOfCustomChars] = {skull, arrow};

/*
  Create each character that has been declared.
*/
void initializeCustomChars(LiquidCrystal &lcd) {
  for (int i = 0; i < numberOfCustomChars; i++) {
    lcd.createChar(i, customChars[i]);
  }
}