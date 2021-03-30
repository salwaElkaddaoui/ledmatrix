//#define DEBUG 1

#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <SoftwareSerial.h>

class Matrix{
  public:
    Matrix(uint8_t h, uint8_t w, uint8_t start_line, const char content[], byte BT_rxPin, byte BT_txPin);
    void init();
    void scroll(uint16_t refresh_rate);
    
  private:
    Adafruit_MCP23017 driver;
    SoftwareSerial BT;

    uint8_t mat_w, mat_h, start_line;
    volatile char content[50];
    
    void set_column(uint8_t col_idx, bool matrix_col[]);
    void display_matrix(bool** matrix, uint16_t refresh_rate);
    void shift_to_left(bool** matrix, bool* newcol);
    void get_bitmap(char c, bool* bitmap);
    void catch_BT_data();
  };
