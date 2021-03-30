#include "matrix.hpp"

Matrix ledmatrix(8, 8, 2, "xxxxxxx", 10, 11);

void setup() {
  Serial.begin(9600);
  ledmatrix.init();
}

void loop() {
 ledmatrix.scroll(300);
}
