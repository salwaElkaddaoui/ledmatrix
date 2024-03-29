#include <Arduino.h>
#include <string.h>
#include <ctype.h>

#include "characters.hpp"
#include "matrix.hpp"

/**
 * Matrix class constructor:
  
    h         : height of the ledmatrix in pixels
    w         : width of the ledmatrix in pixels
    start_line: row of the ledmatrix on which the top of the character lands
    content_  : intial content to scroll on the ledmatrix
    BT_rxPin  : pin on which to receive bluetooth serial data
    BT_txPin  : pin on which to transmit bluetooth serial data
*/
Matrix::Matrix(uint8_t h, uint8_t w, uint8_t start_line, const char content_[], byte BT_rxPin, byte BT_txPin)
: mat_h(h), mat_w(w), start_line(start_line), BT(BT_rxPin, BT_txPin){

  memcpy(this->content, content_, 8);
}

/**
 * Initializer of MCP23017 pins, bluetooth serial connection
 */
void Matrix::init(){
  driver.begin();
  for(uint8_t i=0; i<16; i++)
    driver.pinMode(i, OUTPUT);
  BT.begin(9600);
}

/**
 * Set (to ON or OFF) the pixels of a column of the ledmatrix
 */
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

/**
 * Set all columns of the ledmatrix according to the content to display
 */
void Matrix::display_matrix(bool** matrix, uint16_t refresh_rate){
  uint32_t time = millis();
  while(millis() - time < refresh_rate){
    for(uint8_t i=0; i<this->mat_w; i++)
      set_column(i, matrix[i]);
    catch_BT_data();
  }
}

/**
 * shift the content of the ledmatrix one step to the left
 */
void Matrix::shift_to_left(bool** matrix, bool* newcol){
  for(uint8_t i=this->mat_w-1; i>0; i--)
    memcpy(*(matrix+i), *(matrix+i-1), this->mat_h * sizeof(bool));

  memcpy(*(matrix), newcol, this->mat_h * sizeof(bool));
}

/**
 * given a character, get its bitmap (that we defined in characters.hpp)
 */
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

/**
 * scroll a content (usually a text string) on the matrix
 */
void Matrix::scroll(uint16_t refresh_rate){

  bool** matrix = (bool**)calloc(this->mat_w, sizeof(bool*));
  for( int i=0; i<this->mat_w; i++ ){
    *(matrix+i) = (bool*)calloc(this->mat_h, sizeof(bool));
  }
  
  //num_empty_cols is used to keep track of the matrix columns
  //that are filled with a displayable content. 
  //Once all columns are filled, we shift the matrix content to the left
  //in order to make space for new content
  uint8_t num_empty_cols = 1; 

  //for each character of the text string
  uint8_t c = 0;
  while( this->content[c] != '\0' ){
    uint8_t bitmap_width = 0; //number of columns of the bitmap
    bool* bitmap;
    if( isalnum(this->content[c]) ){
      //get its bitmap 
      bitmap = (bool*)calloc((CHAR_W+1)*this->mat_h, sizeof(bool));
      get_bitmap( tolower( this->content[c] ), bitmap );
      bitmap_width = CHAR_W+1; //I add one for the space after the character
    }
    else{
      if( this->content[c] == ' '){
        bitmap = (bool*)calloc(2*this->mat_h, sizeof(bool));
        bitmap_width = 2;
      }
      else{
        bitmap = (bool*)calloc(this->mat_h, sizeof(bool));
        bitmap_width = 1;
        }
      }
    
    //for each column of the bitmap of the character
    for(uint8_t i=0; i<bitmap_width; i++){
      
      //if there's enough empty columns on the matrix, copy it to the matrix 
      if( num_empty_cols > 0 ){
        memcpy( *(matrix+num_empty_cols-1), bitmap+i*this->mat_h, this->mat_h*sizeof(bool) );
        num_empty_cols--;
      }
      else{ //this means that all columns of the matrix are filled, 
            //and we need to display them
            //and make space for new content by shifting the led matrix
        display_matrix(matrix, refresh_rate);
        shift_to_left(matrix, bitmap+i*this->mat_h);
      }
    }
  c++;
  free(bitmap);
  }

  //keep scrolling until the text disappears in the left direction
  bool blank[this->mat_h] = {0};
  while( num_empty_cols <= this->mat_w ){
    display_matrix(matrix, refresh_rate);
    shift_to_left(matrix, blank);
    num_empty_cols++;
  }
  for(int i=0; i<this->mat_w; i++)
    free(matrix[i]);
  free(matrix);
}

/**
 * listen for text string to scroll on ledmatrix received via bluetooth 
 * (sent from a phone having the Bluetooth Terminal App)
 */
void Matrix::catch_BT_data(){
  
  if( this->BT.available() ){
    uint8_t i= 0;
    while( this->BT.available() && i<49 ){
      char c = BT.read();
      if( isalnum(c) || c == ' '){
        this->content[i] = c;
        i++;
      }
    }
    this->content[i] = '\0';
  }
}
