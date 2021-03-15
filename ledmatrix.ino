/*
MCP23O17 pin numbers:
pin 0 is bit 0 of port A
pin 7 is bit 7 of port A
pin 8 is bit 0 of port B
pin 15 is bit 7 of port B

For an 8x8 ledmatrix where the pixel is 4cmx4cm large,
use a frequency of 400k for the I2C bus, by changing the file:
arduino-1.8.13/hardware/arduino/avr/libraries/Wire/src/utility/twi.h
from #define TWI_FREQ 100000L to #define TWI_FREQ 400000L
*/

#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include "characters.h"

Adafruit_MCP23017 mcp;

void setup() {
  mcp.begin();
  for(int i=0; i<16; i++){
    mcp.pinMode(i, OUTPUT);
    }
}

 /*  void set_column(uint8_t col_idx, bool pixels[]):
   *
   * Lights up a column of the led matrix
   *
   * uint8_t col_idx : the index of the column to light up
   *
   * bool pixels[]:  the states of the pixels of a column "col_idx"
   * ordered from the top of the led matrix to its bottom

   * Step 0: clear all LEDs
   * Step 1: set the column "col_idx" and clear the rest of the columns:
   * A column of the matrix connects the anodes of the leds.
   * So setting a column consists of setting to HIGH
   * the corresponding expander pin
   * Step 2: put ON or OFF the leds of column col_idx depending
   * on the values contained in input argument pixels[]
   */
void set_column_eachpin(uint8_t col_idx, bool pixels[]){

  for(uint8_t i=0; i<8; i++)
    mcp.digitalWrite(i, HIGH);

  for(uint8_t i=8; i<16; i++) //Anodes are connected to Bank B of the MCP23O17
    if(i!=col_idx+8)
      mcp.digitalWrite(i, LOW);
    else
      mcp.digitalWrite(i, HIGH);

  for(uint8_t i=0; i<8; i++) //Cathodes are connected to Bank A of the MCP23O17
    if(pixels[i])
      mcp.digitalWrite(i, LOW);
}


/*
Set a column of the led matrix
All banks of the GPIO expander are set in a single transmission
*/
void set_column(uint8_t col_idx, bool pixels[]){

  uint16_t ba = 0;
  byte BankA = 0xff; //Cathodes are connected to Bank A of the MCP23O17
  byte BankB = 0x00; //Anodes are connected to Bank B of the MCP23O17
  //concatenate
  ba = (BankB << 8) | (BankA);
  mcp.writeGPIOAB(ba); //to get rid of ghost lighting

  BankB |= 1 << col_idx;
  BankA = 0x00;
  for(uint8_t i=0; i<8; i++)
    if(pixels[i]==0)
      BankA |= 1 << i;
  ba = (BankB << 8) | (BankA);
  mcp.writeGPIOAB(ba);
}

/*  void display_character(bool character[3][5]):
  *
  * Display character on led matrix by lighting up one column after another
  *
  * bool character[3][5] : the bitmap of the character to display
  *
  * for each column of the character's bitmap:
  * Step 0: fill up the column of the ledmatrix accordingly
  * Step 1: display the column
  */
void display_character(bool character[3][5]){
  bool pixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  for(int col=0; col<3; col++){
    for(int i=0; i<5; i++){
      pixels[i+3] = character[col][i];
      }
    set_column(7-col, pixels);
    }
}

void loop() {
  uint32_t time = millis();
  while(millis() - time < 1000)
    display_character(zero);

  time = millis();
  while(millis() - time < 1000)
    display_character(one);

  time = millis();
  while(millis() - time < 1000)
    display_character(two);

  time = millis();
  while(millis() - time < 1000)
    display_character(three);

  time = millis();
  while(millis() - time < 1000)
    display_character(four);

  time = millis();
  while(millis() - time < 1000)
    display_character(five);

  time = millis();
  while(millis() - time < 1000)
    display_character(six);

  time = millis();
  while(millis() - time < 1000)
    display_character(seven);

  time = millis();
  while(millis() - time < 1000)
    display_character(eight);

  time = millis();
  while(millis() - time < 1000)
    display_character(nine);

}
