#include "custom_characters.h"
// #include "joystick.h"
#include "menu.h"

// PINs connected to the joystick
// digital pin connected to joystick's switch output
const byte joystick_pin_SW = 2;
// analogue pin connected to X axis
const byte joystick_pin_X = A0;
// analogue pin connected to Y axis
const byte joystick_pin_Y = A1;

Joystick joystick(joystick_pin_SW, joystick_pin_X, joystick_pin_Y);

// PINs connected to the LCD
const byte lcd_rs = 9; 
const byte lcd_en = 8;
const byte lcd_d4 = 7;
const byte lcd_d5 = 6;
const byte lcd_d6 = 5;
const byte lcd_d7 = 4;

Menu menu(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7);

void setup() {
  // set up joystick's pins
  pinMode(joystick_pin_SW, INPUT_PULLUP);
  pinMode(joystick_pin_X, INPUT);
  pinMode(joystick_pin_Y, INPUT);
  
  initializeCustomChars(menu.lcd);
  // set up the LCD's number of columns and rows
  menu.lcd.begin(16, 2);

  Serial.begin(9600);
}

void loop() {
  joystick.switchHandler();
  joystick.movementHandler();
 
  menu.menuSwitch(joystick);
}

