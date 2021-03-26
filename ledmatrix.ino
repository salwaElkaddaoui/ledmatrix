#include "matrix.hpp"

Matrix ledmatrix(8, 8);

void setup() {

  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  
  ledmatrix.init();
}

void loop() {
  //static int counter = 0;
  char text[7] = {'7', '0', '3', '5', '2', '1', '\0'};
  //if(counter == 0){
    ledmatrix.scroll_text(text, 1000);
  //  counter++;
  //}
}
