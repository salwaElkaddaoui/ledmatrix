//#define DEBUG 1

#define MAT_H 8 // number of pixels
#define MAT_W 8

#define CHAR_H 5
#define CHAR_W 3

#define NUM_CHARS 10 //number of characters with a defined bitmap

#define TEXT_SCROLL_SPEED 2000 // milliseconds
#define MAX_TEXT_LENGTH 50 // number of characters in text string
#define START_LINE 2 //index of matrix row where the top of the character lands

bool zero[CHAR_W*CHAR_H] = {0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  1, 1, 1, 1, 1};
bool one[CHAR_W*CHAR_H]  = {0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  1, 1, 1, 1, 1};
bool two[CHAR_W*CHAR_H]  = {1, 0, 1, 1, 1,  1, 0, 1, 0, 1,  1, 1, 1, 0, 1};
bool three[CHAR_W*CHAR_H] = {1, 0, 1, 0, 1,  1, 0, 1, 0, 1,  1, 1, 1, 1, 1};
bool four[CHAR_W*CHAR_H] = {1, 1, 1, 0, 0,  0, 0, 1, 0, 0,  1, 1, 1, 1, 1};
bool five[CHAR_W*CHAR_H] = {1, 1, 1, 0, 1,  1, 0, 1, 0, 1,  1, 0, 1, 1, 1};
bool six[CHAR_W*CHAR_H] = {1, 1, 1, 1, 1,  1, 0, 1, 0, 1, 1, 0, 1, 1, 1};
bool seven[CHAR_W*CHAR_H] = {1, 0, 0, 0, 0,  1, 0, 0, 0, 0,  1, 1, 1, 1, 1};
bool eight[CHAR_W*CHAR_H] = {1, 1, 1, 1, 1,  1, 0, 1, 0, 1,  1, 1, 1, 1, 1};
bool nine[CHAR_W*CHAR_H] = {1, 1, 1, 0, 1,  1, 0, 1, 0, 1,  1, 1, 1, 1, 1};
bool* char_table[CHAR_W*CHAR_H] = { zero, one, two, three, four, five, six, seven, eight, nine};
