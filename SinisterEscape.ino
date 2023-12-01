#include "CustomCharacters.h"
// #include "joystick.h"
#include "Menu.h"

// PINs connected to the joystick
// digital pin connected to joystick's switch output
const byte joystickinSW = 2;
// analogue pin connected to X axis
const byte joystickinX = A0;
// analogue pin connected to Y axis
const byte joystickinY = A1;

Joystick joystick(joystickinSW, joystickinX, joystickinY);

// PINs connected to the LCD
const byte lcdRS = 9; 
const byte lcdEN = 8;
const byte lcdD4 = 7;
const byte lcdD5 = 6;
const byte lcdD6 = 5;
const byte lcdD7 = 4;
const byte birgt = 10;

Menu menu(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7);

void setup() {
  // set up joystick's pins
  pinMode(joystickinSW, INPUT_PULLUP);
  pinMode(joystickinX, INPUT);
  pinMode(joystickinY, INPUT);
  
  pinMode(birgt, OUTPUT);
  analogWrite(birgt, 100);

  initializeCustomChars(menu.lcd);
  // set up the LCD's number of columns and rows
  menu.lcd.begin(16, 2);

  Serial.begin(9600);
}

void loop() {
  joystick.switchHandler();
  joystick.movementHandler();
 
  menu.lcdBrightnessMenuHandler(joystick);
}

