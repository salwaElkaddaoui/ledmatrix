#define MAT_H 8 // number of pixels
#define MAT_W 8

#define CHAR_H 5
#define CHAR_W 3

#define NUM_CHARS 36 //number of characters with a defined bitmap

#define TEXT_SCROLL_SPEED 1000 // milliseconds
#define MAX_TEXT_LENGTH 50 // max number of characters in text string
#define START_LINE 2 //index of matrix row where the top of the character lands

bool zero[CHAR_W*CHAR_H]  = {1, 1, 1, 1, 1,   1, 0, 0, 0, 1,    1, 1, 1, 1, 1};
bool one[CHAR_W*CHAR_H]   = {0, 0, 1, 0, 0,   0, 1, 0, 0, 0,    1, 1, 1, 1, 1};
bool two[CHAR_W*CHAR_H]   = {1, 0, 1, 1, 1,   1, 0, 1, 0, 1,    1, 1, 1, 0, 1};
bool three[CHAR_W*CHAR_H] = {1, 0, 1, 0, 1,   1, 0, 1, 0, 1,    1, 1, 1, 1, 1};
bool four[CHAR_W*CHAR_H]  = {1, 1, 1, 0, 0,   0, 0, 1, 0, 0,    1, 1, 1, 1, 1};
bool five[CHAR_W*CHAR_H]  = {1, 1, 1, 0, 1,   1, 0, 1, 0, 1,    1, 0, 1, 1, 1};
bool six[CHAR_W*CHAR_H]   = {1, 1, 1, 1, 1,   1, 0, 1, 0, 1,    1, 0, 1, 1, 1};
bool seven[CHAR_W*CHAR_H] = {1, 0, 0, 0, 0,   1, 0, 0, 0, 0,    1, 1, 1, 1, 1};
bool eight[CHAR_W*CHAR_H] = {1, 1, 1, 1, 1,   1, 0, 1, 0, 1,    1, 1, 1, 1, 1};
bool nine[CHAR_W*CHAR_H]  = {1, 1, 1, 0, 1,   1, 0, 1, 0, 1,    1, 1, 1, 1, 1};

bool A[CHAR_W*CHAR_H]     = {0, 1, 1, 1, 1,   1, 0, 1, 0, 0,    0, 1, 1, 1, 1};
bool B[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   1, 0, 1, 0, 1,    0, 1, 0, 1, 0};
bool C[CHAR_W*CHAR_H]     = {0, 1, 1, 1, 0,   1, 0, 0, 0, 1,    1, 0, 0, 0, 1};
bool D[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   1, 0, 0, 0, 1,    0, 1, 1, 1, 0};
bool E[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   1, 0, 1, 0, 1,    1, 0, 0, 0, 1};
bool F[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   1, 0, 1, 0, 0,    1, 0, 0, 0, 0};
bool G[CHAR_W*CHAR_H]     = {0, 1, 1, 1, 0,   1, 0, 0, 0, 1,    1, 0, 1, 1, 1};
bool H[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   0, 0, 1, 0, 0,    1, 1, 1, 1, 1};
bool I[CHAR_W*CHAR_H]     = {1, 0, 0, 0, 1,   1, 1, 1, 1, 1,    1, 0, 0, 0, 1};
bool J[CHAR_W*CHAR_H]     = {1, 0, 0, 1, 0,   1, 0, 0, 0, 1,    1, 1, 1, 1, 0};
bool K[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   0, 1, 0, 1, 0,    0, 0, 1, 0, 0};
bool L[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   0, 0, 0, 0, 1,    0, 0, 0, 0, 1};
bool M[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   1, 0, 0, 0, 0,    1, 1, 1, 1, 1};
bool N[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   0, 1, 0, 0, 0,    0, 0, 1, 1, 1};
bool O[CHAR_W*CHAR_H]     = {0, 1, 1, 1, 0,   1, 0, 0, 0, 1,    0, 1, 1, 1, 0};
bool P[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   1, 0, 1, 0, 0,    0, 1, 0, 0, 0};
bool Q[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 0,   1, 0, 0, 1, 1,    1, 1, 1, 1, 0};
bool R[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   1, 0, 1, 1, 0,    0, 1, 1, 0, 1};
bool S[CHAR_W*CHAR_H]     = {0, 1, 1, 0, 1,   1, 0, 1, 0, 1,    1, 0, 1, 1, 0};
bool T[CHAR_W*CHAR_H]     = {1, 0, 0, 0, 0,   1, 1, 1, 1, 1,    1, 0, 0, 0, 0};
bool U[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 1,   0, 0, 0, 0, 1,    1, 1, 1, 1, 1};
bool V[CHAR_W*CHAR_H]     = {1, 1, 1, 1, 0,   0, 0, 0, 0, 1,    1, 1, 1, 1, 0};
bool W[CHAR_W*CHAR_H]     = {0, 1, 1, 1, 0,   1, 0, 0, 0, 1,    0, 1, 1, 1, 0};
bool X[CHAR_W*CHAR_H]     = {1, 1, 0, 1, 1,   0, 0, 1, 0, 0,    1, 1, 0, 1, 1};
bool Y[CHAR_W*CHAR_H]     = {1, 1, 0, 0, 0,   0, 0, 1, 1, 1,    1, 1, 0, 0, 0};
bool Z[CHAR_W*CHAR_H]     = {1, 0, 0, 1, 1,   1, 0, 1, 0, 1,    1, 1, 0, 0, 1};

bool* char_table[NUM_CHARS] = { zero, one, two, three, four, five, six, seven, eight, nine, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z};
