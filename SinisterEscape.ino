/*
  Sinister Escape - the matrix game

  Circuit has a LCD 16x2, 1088AS LED Matrix 8 x 8 which is controlled
  with the help of MAX7219 driver, joystick, buzzer. It also contains 
  wires, resistors & capacitors as needed.

  Circuit components:
  - OUTPUT:
    - LCD 16x2:
      - (1) VSS: GND
      - (2) VCC: 5V 
      - (3) V0: DIGITAL 10 (~PWM)
      - (4) RS: DIGITAL 9
      - (5) RW: GND
      - (6) E: DIGITAL 8
      - (11) D4: DIGITAL 4
      - (12) D5: DIGITAL 6
      - (13) D6: DIGITAL 5
      - (14) D7: DIGITAL 4
      - (15) A: 5V
      - (16) K: GND (with 220+ ohm resistor)
    - Joystick:
      - GND: GND
      - 5V: 5V
      - VRx: A0
      - VRy: A1
      - SW: DIGITAL 2
    - Buzzer:
      - 5V: DIGITAL 3
      - GND: GND
    - MAX7219 Driver:
      - (4) GND: GND
      - (9) GND: GND
      - (18) ISET: 5V (with a 10k or 100k+ resistor)
      - (19) V+: 5V
      - (1) DIN: 12
      - (12) LOAD/CS: 10
      - (13) CLK: 11

  Connections between 1088AS Matrix and MAX7219 Driver:

  Row / Column   |   Matrix PIN   |   DIG/SEG Number   |   Driven PIN    
  -----------------------------------------------------------------------
  Row 5          | 1              | DIG 4              | 3          
  Row 7          | 2              | DIG 6              | 5
  Col 2          | 3              | SEG A              | 14
  Col 3          | 4              | SEG B              | 16
  Row 8          | 5              | DIG 7              | 8
  Col 5          | 6              | SEG D              | 23
  Row 6          | 7              | DIG 5              | 10
  Row 3          | 8              | DIG 2              | 6
  Row 1          | 9              | DIG 0              | 2
  Col 4          | 10             | SEG C              | 20
  Col 6          | 11             | SEG E              | 21
  Row 4          | 12             | DIG 3              | 7
  Col 1          | 13             | SEG DP             | 22
  Row 2          | 14             | DIG 1              | 11
  Col 7          | 15             | SEG F              | 15
  Col 8          | 16             | SEG G              | 17

  Created 18-12-2023
  By Olaeriu Vlad Mihai
*/

#include "CustomCharacters.h"
#include "Joystick.h"
#include "Menu.h"
#include "Game.h"

// PINs connected to the matrix
const byte dinPin = 13;
const byte clockPin = 11;
const byte loadPin = 12;

// PINs connected to the joystick
// digital pin connected to joystick's switch output
const byte joystickinSW = 2;
// analogue pin connected to X axis
const byte joystickinX = A0;
// analogue pin connected to Y axis
const byte joystickinY = A1;

Joystick joystick(joystickinSW, joystickinX, joystickinY);

// buzzer PIN to play music and sounds 
const byte buzzerPin = 3;

// PINs connected to the LCD
const byte lcdRS = 9; 
const byte lcdEN = 8;
const byte lcdD4 = 7;
const byte lcdD5 = 6;
const byte lcdD6 = 5;
const byte lcdD7 = 4;
const byte brightnessPin = 10;

Menu menu(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7, dinPin, clockPin, loadPin, buzzerPin, brightnessPin);

void setup() {
  // set up joystick's pins
  pinMode(joystickinSW, INPUT_PULLUP);
  pinMode(joystickinX, INPUT);
  pinMode(joystickinY, INPUT);
  
  // set brightness pin for LCD
  pinMode(brightnessPin, OUTPUT);
  // initialize the customized characters for the LCD
  initializeCustomChars(menu.lcd);
  // set up the LCD's number of columns and rows
  menu.lcd.begin(16, 2);

  // set the buzzer pin
  pinMode(buzzerPin, OUTPUT);

  // set the seed for randomness
  randomSeed(analogRead(0));

  Serial.begin(9600);
}

void loop() {
  // constantly listens to joystick movements
  joystick.switchHandler();
  joystick.movementHandler();
 
  menu.menuSwitch(joystick);
}


