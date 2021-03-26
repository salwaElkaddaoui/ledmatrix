#include <Arduino.h>
#include <string.h>

#include "characters.hpp"
#include "matrix.hpp"

Matrix::Matrix(uint8_t h, uint8_t w, uint8_t start_line){
  this->mat_h = h; //led matrix height in pixels
  this->mat_w = w; //led matrix width in pixels
  this->start_line = start_line; //index of matrix row where the top of the character lands
}

void Matrix::init(){
  driver.begin();
  for(uint8_t i=0; i<16; i++)
    driver.pinMode(i, OUTPUT);
}

void Matrix::set_column(uint8_t col_idx, bool matrix_col[]){

  uint16_t ba = 0; //variable holding the data to be written in registers GPIOA and GPIOB
  byte BankA = 0xff; //Cathodes are connected to Bank A of the MCP23O17
  byte BankB = 0x00; //Anodes are connected to Bank B of the MCP23O17
  ba = (BankB << 8) | (BankA);
  this->driver.writeGPIOAB(ba); //for ghost cancelling

  BankB |= 1 << col_idx;
  BankA = 0x00;
  for(uint8_t i=0; i<this->mat_h; i++)
    if(matrix_col[i]==0)
      BankA |= 1 << i;
  ba = (BankB << 8) | (BankA);
  this->driver.writeGPIOAB(ba);
}

void Matrix::set_matrix(bool** matrix, uint16_t refresh_rate){
  uint32_t time = millis();
  while(millis() - time < refresh_rate)
    for(uint8_t i=0; i<this->mat_w; i++)
      set_column(i, matrix[i]);
}

void Matrix::shift_to_left(bool** matrix, bool* newcol){
  for(uint8_t i=this->mat_w-1; i>0; i--)
    memcpy(*(matrix+i), *(matrix+i-1), this->mat_h * sizeof(bool));

  memcpy(*(matrix), newcol, this->mat_h * sizeof(bool));
}

void Matrix::get_bitmap(char c, bool* bitmap){

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
        bitmap[ i*this->mat_h + j + this->start_line ] = char_table[hash_code][i*CHAR_H + j];
  else{
    #ifdef DEBUG
      Serial.println("Unknown character in text.\nPlease reboot the system...");
    #endif
    while(true){}
  }
}

void Matrix::scroll_text(char* text, uint16_t refresh_rate){

  bool** matrix = (bool**)calloc(this->mat_w, sizeof(bool*));
  for( int i=0; i<this->mat_w; i++ ){
    *(matrix+i) = (bool*)calloc(this->mat_h, sizeof(bool));
  }
  
  //num_empty_cols is used to keep track of the matrix columns
  //that are filled with a displayable content. Once all columns are filled,
  //we shift the matrix content to the left in order to make space for new content
  uint8_t num_empty_cols = 1; 

  //for each character of the text string
  uint8_t c = 0;
  while( text[c] != '\0' ){

    //get its bitmap 
    bool bitmap[CHAR_W*this->mat_h] = {0};
    get_bitmap( tolower( text[c] ), bitmap );
  
  #ifdef DEBUG
    Serial.println("character bitmap: ");
    for(int i=0; i<CHAR_W; i++){
      for(int j=0; j<this->mat_h; j++){
        Serial.print(bitmap[i*this->mat_h+j]);
        Serial.print(" ");
      }
      Serial.println("\n");
    }
    Serial.println("end of character bitmap");
  #endif

    //for each column of the bitmap of the character
    for(uint8_t i=0; i<CHAR_W; i++){
      
      //if there's enough empty columns on the matrix, copy it to the matrix 
      if( num_empty_cols > 0 ){
        memcpy( *(matrix+num_empty_cols-1), bitmap+i*this->mat_h, this->mat_h*sizeof(bool) );
        num_empty_cols--;
    #ifdef DEBUG
        Serial.print("num_empty_cols: ");
        Serial.println(num_empty_cols);
    #endif
      }
      else{ //this means that all columns of the matrix are filled, 
            //and we need to display them
            //and make space for new content by shifting the led matrix
        set_matrix(matrix, refresh_rate);
        shift_to_left(matrix, bitmap+i*this->mat_h);
      }
    }

  #ifdef DEBUG
  Serial.println("inside scroll_text: ");
    for(int  i=0; i<this->mat_w; i++){
      for(int  j=0; j<this->mat_h; j++){
        Serial.print(matrix[i][j]);
        Serial.print(" ");
      }
      Serial.println("\n");
    }
  #endif
  c++;
  }

  //keep scrolling until the text disappears in the left direction
  bool blank[this->mat_h] = {0};
  while( num_empty_cols <= this->mat_w ){
    set_matrix(matrix, refresh_rate);
    shift_to_left(matrix, blank);
    num_empty_cols++;
  }
  for(int i=0; i<this->mat_w; i++)
    free(matrix[i]);
  free(matrix);
}
