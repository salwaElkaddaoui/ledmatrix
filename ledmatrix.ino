#include <Wire.h>
#include <Adafruit_MCP23017.h>

#include <string.h>
#include "characters.h"

Adafruit_MCP23017 mcp;

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  
  mcp.begin();
  for(uint8_t i=0; i<16; i++)
    mcp.pinMode(i, OUTPUT);
}


void set_column(uint8_t col_idx, bool matrix_col[]){

  uint16_t ba = 0; //variable holding the data to be written in registers GPIOA and GPIOB
  byte BankA = 0xff; //Cathodes are connected to Bank A of the MCP23O17
  byte BankB = 0x00; //Anodes are connected to Bank B of the MCP23O17
  ba = (BankB << 8) | (BankA);
  mcp.writeGPIOAB(ba); //for ghost cancelling

  BankB |= 1 << col_idx;
  BankA = 0x00;
  for(uint8_t i=0; i<MAT_H; i++)
    if(matrix_col[i]==0)
      BankA |= 1 << i;
  ba = (BankB << 8) | (BankA);
  mcp.writeGPIOAB(ba);
}

void set_matrix(bool** matrix){
  uint32_t time = millis();
  while(millis() - time < TEXT_SCROLL_SPEED)
    for(uint8_t i=0; i<MAT_W; i++)
      set_column(i, matrix[i]);
}

void shift_to_left(bool** matrix, bool* newcol){
  for(uint8_t i=0; i<MAT_W-1; i++)
    memcpy(*(matrix+i), *(matrix+i+1), MAT_H * sizeof(bool));

  memcpy(*(matrix+MAT_W-1), newcol, MAT_H * sizeof(bool));
}

void get_bitmap(char c, bool* bitmap){

  uint8_t ascii_code = c;
  int hash_code = -1;
  if( ascii_code >= 48 && ascii_code <= 57 ) //0-9
    hash_code = ascii_code - 48;
  if(ascii_code >= 97 && ascii_code <= 122) //a-z
    hash_code = ascii_code - 97+10;
  
  #ifdef DEBUG
    Serial.print("ascii_code: "); Serial.println(ascii_code);
    Serial.print("hash_code: "); Serial.println(hash_code);
  #endif

  if(hash_code >= 0)
    for(uint8_t i=0; i<CHAR_W; i++)//for each column of the bitmap  
      for(uint8_t j=0; j<CHAR_H; j++)//for each cell of the column
        bitmap[ (CHAR_W-1-i)*MAT_H + j + START_LINE ] = char_table[hash_code][i*CHAR_H + j];
  else{
    #ifdef DEBUG
      Serial.println("Unknown character in text.\nPlease reboot the system...");
    #endif
    while(true){}
  }

}

void display_text(char* text){

  bool** matrix = (bool**)calloc(MAT_W, sizeof(bool*));
  for( int i=0; i<MAT_W; i++ ){
    *(matrix+i) = (bool*)calloc(MAT_H, sizeof(bool));
  }
  
  //num_filled_cols is used to keep track of the matrix columns
  //that are filled with a displayable content. Once all columns are filled,
  //we shift the matrix content to the left in order to make space for new content
  uint8_t num_filled_cols = 0; 

  //for each character of the text string
  uint8_t c = 0;
  while( text[c] != '\0' ){

    //get its bitmap 
    bool bitmap[CHAR_W*MAT_H] = {0};
    get_bitmap( tolower( text[c] ), bitmap );
  #ifdef DEBUG
    Serial.println("inside display_text: ");
    for(int  i=0; i<CHAR_W; i++){
      for(int  j=0; j<MAT_H; j++){
        Serial.print(bitmap[i*MAT_H+j]);
        Serial.print(" ");
      }
      Serial.println("\n");
    }
    Serial.println("end of display_text");
  #endif

    //for each column of the bitmap of the character
    for(uint8_t i=0; i<CHAR_W; i++){
      
      //if there's enough still unfilled columns on the matrix, copy it to the matrix 
      if( num_filled_cols < MAT_W ){
        memcpy( *(matrix+num_filled_cols), bitmap+i*MAT_H, MAT_H*sizeof(bool) );
        num_filled_cols++;
    #ifdef DEBUG
        Serial.print("num_filled_cols: ");
        Serial.println(num_filled_cols);
    #endif
      }
      else{ //this means that all columns of the matrix are filled, 
            //and we need to display them
            //and make space for new content by shifting the led matrix
        set_matrix(matrix);
        shift_to_left(matrix, bitmap+i*MAT_H);
      }
    }

  #ifdef DEBUG
  Serial.println("inside display_text: ");
    for(int  i=0; i<MAT_W; i++){
      for(int  j=0; j<MAT_H; j++){
        Serial.print(matrix[i][j]);
        Serial.print(" ");
      }
      Serial.println("\n");
    }
  #endif
  c++;
  }

  //keep scrolling until the text disappears in the left direction
  bool blank[MAT_H] = {0};
  while( num_filled_cols > 0 ){
    set_matrix(matrix);
    shift_to_left(matrix, blank);
    num_filled_cols--;
  }
  for(int i=0; i<MAT_W; i++)
    free(matrix[i]);
  free(matrix);
}

int matrixState = 1;
unsigned long previousMillis = millis();
unsigned long interval = 1000;

void loop() {

  bool col_0[8] = {false, false, true, false, true, true, false, false};
  bool col_1[8] = {false, true, true, false, false, true, true, false};
  bool col_2[8] = {false, false, true, false, true, true, false, false};
  bool col_3[8] = {false, false, false, false, false, true, false, false};
  bool col_4[8] = {false, false, true, true, false, false, true, false};
  bool col_5[8] = {false, true, true, false, true, true, true, false};
  bool col_6[8] = {false, true, false, false, false, false, true, false};
  bool col_7[8] = {false, false, true, false, true, true, false, false};
  bool none[8] = {false, false, false, false, false, false, false, false};
  bool* matrix[8] = { col_0, col_1, col_2, col_3, col_4, col_5, col_6, col_7 }; 
  bool* nones[8] = { none, none, none, none, none, none, none, none };

 /* 
  set_matrix(matrix);

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (matrixState == 1) {
      matrixState = 0;
    } else {
      matrixState = 0;
    }

    // set the LED with the ledState of the variable:
    if( matrixState==0 )
      set_matrix(nones);
    else
      set_matrix(matrix);
  }
  */

  static int counter = 0;
 

  //char c = '1';
  char text[4] = {'1', '2', '9', '\0'};
  
//  if(counter == 0){
    display_text(text);
  /*
    get_bitmap(c, bitmap);
    for(int  i=0; i<CHAR_W; i++){
      for(int  j=0; j<MAT_H; j++){
        Serial.print(bitmap[i*MAT_H+j]);
        Serial.print(" ");
      }
      Serial.println("\n");
    }*/
//  counter++;
//  }
}
