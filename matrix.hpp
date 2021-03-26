//#define DEBUG 1

#include <Wire.h>
#include <Adafruit_MCP23017.h>

class Matrix{
  public:
  Matrix(uint8_t h, uint8_t w);
  void init();
  void scroll_text(char* text, uint16_t refresh_rate);

  private:
  uint8_t mat_w, mat_h;
  Adafruit_MCP23017 driver;
  
  void set_column(uint8_t col_idx, bool matrix_col[]);
  void set_matrix(bool** matrix, uint16_t refresh_rate);
  void shift_to_left(bool** matrix, bool* newcol);
  void get_bitmap(char c, bool* bitmap);
  };
