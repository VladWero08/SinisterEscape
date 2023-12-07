#pragma once
#ifndef ROOMS_H
#define ROOMS_H

const byte roomsSize = 4;
// array which contains the initial configuration
// for each room in the game
const uint8_t rooms[roomsSize][8] = {
{
  0b11100111,
  0b10000001,
  0b10101001,
  0b00001100,
  0b00000000,
  0b10111001,
  0b10000001,
  0b11100111
}, 
{
  0b11100111,
  0b10000001,
  0b10100101,
  0b00110000,
  0b00000100,
  0b10110101,
  0b10000001,
  0b11100111
},
{
  0b11100111,
  0b10000001,
  0b10110001,
  0b00011000,
  0b00000000,
  0b10110101,
  0b10000001,
  0b11100111
},
{
  0b11100111,
  0b10000001,
  0b10111101,
  0b00010000,
  0b00010000,
  0b10110101,
  0b10000001,
  0b11100111
}};

#endif