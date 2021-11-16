#include "game.h"
#include "Random.h"

#define TRUE 	      1
#define FALSE 	    0

#define EMPTY		    0
#define TILE_GREEN	4
#define TILE_RED	  8
#define TILE_BLUE	  12
#define TILE_PURPLE	16
#define TILE_YELLOW	20

#define REMOVING_FLAG	0b10000000


//조각. 총 7개
const int shape_bar[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0} };

const int shape_square[4][4] = {
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0} };

const int shape_j[4][4] = {
    {0, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0} };

const int shape_l[4][4] = {
    {0, 0, 1, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0} };

const int shape_s[4][4] = {
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0} };

const int shape_z[4][4] = {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0} };

const int shape_t[4][4] = {
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0} };

const int(*shapes[7])[4][4] = {
    &shape_bar,
    &shape_square,
    &shape_l,
    &shape_j,
    &shape_s,
    &shape_z,
    &shape_t,
};

//조각 정보 구조체
typedef struct
{
    int shape[4][4];
    int id;
} Piece;


const byte pieces[5] = {
  TILE_GREEN,
  TILE_RED,
  TILE_BLUE,
  TILE_PURPLE,
  TILE_YELLOW
};


//#define SPEED			60 // 15 // 30 // 60

#define LINES_PLAYFIELD 	18
#define COLS_PLAYFIELD 		10
#define INITIAL_LINE		0
#define INITIAL_COL		2
byte playfield[LINES_PLAYFIELD][COLS_PLAYFIELD];
byte playfieldTemp[LINES_PLAYFIELD][COLS_PLAYFIELD];

bool gameOver = FALSE;
byte g_col = 0, g_line = 0;
byte counter = 0;
//byte topPiece, midPiece, bottomPiece;
//byte nextTopPiece, nextMidPiece, nextBottomPiece;

Piece g_piece = { {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
                
                 0 };

Piece g_next_piece = { {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
                     
                      0 };

//word score = 0;
word blocksRemoved = 0;
byte level = 1;
bool newLevel = FALSE;
byte speed, rndSeed = 0;


// Debug variables
// byte d_col = 0, d_line = 0, d_value = 0;




#include "Data\Fonts.h"
#include "Data\TilePatterns.h"
#include "Data\TileColors.h"
#include "Data\Sprites.h"

#include "Graphics\GraphicsConstants.h"
#include "Graphics\SpritesLogic.h"
#include "Graphics\Countdown.h"
#include "Graphics\Graphics.h"




void Wait(word numberOfFrames) {
    do {
        word lastJiffy = JIFFY;
        while (lastJiffy == JIFFY) {
        }
    } while (numberOfFrames-- > 0);
}

word Power(byte base, byte expoent) {
    word power = base;

    if (expoent == 0) return 1;

    for (byte j = 1; j < expoent; j++) {
        power = power * base;
    }

    return power;
}

void ChangeFontColor(byte color) {
    //FILVRM(CLRTBL + (NUMBER_OF_PATTERNS * 8), (HICHAR - LOCHAR + 1) * 8, color);
    FILVRM(CLRTBL + (256 * 8) + (NUMBER_OF_PATTERNS * 8), (HICHAR - LOCHAR + 1) * 8, color);
    //FILVRM(CLRTBL + (512 * 8) + (NUMBER_OF_PATTERNS * 8), (HICHAR - LOCHAR + 1) * 8, color);
}

// void ScrollDownFont() {
//   word startVRAMAddr = PATTBL + (256 * 8) + (NUMBER_OF_PATTERNS * 8);
//   word size = (HICHAR-LOCHAR+1) * 8;
//   byte value;

//   // Loading font patterns (1st bank)
//   	//void LDIRVM(uint16_t vdest, const uint8_t* msrc, uint16_t count);
//   //LDIRVM(PATTBL + (NUMBER_OF_PATTERNS * 8), FONT, (HICHAR-LOCHAR+1) * 8);

//   //uint8_t RDVRM(uint16_t addr) __z88dk_fastcall;

//   //void WRTVRM(uint16_t addr, uint8_t data);


//   for(word i=startVRAMAddr; i < (startVRAMAddr + size); i+=8) {
//     for(word j=i+7; j>0; j--) {
//       value = RDVRM(j-1);
//       //WRTVRM(j, value);
//       WRTVRM(j, 0);
//     }
//     WRTVRM(i, 0);

//   }

// }

void FadeOut() {
    byte counter = 0;

    //Wait(60);

    ChangeFontColor(0x40);
    //ScrollDownFont();
    //ChangeFontColor(0x80);
    //ScrollDownFont();

    // f e 5 4

    while (counter++ < 105) {
        byte lastJiffy = JIFFY;
        while (JIFFY == lastJiffy) {
        }

        if (counter == 90) {
            ChangeFontColor(0x40);
            Wait(5);
        }
        else if (counter == 75) {
            ChangeFontColor(0x50);
            Wait(5);
        }
        else if (counter == 60) {
            ChangeFontColor(0xe0);
            Wait(5);
        }
        else if (counter == 45) {
            ChangeFontColor(0xf0);
            Wait(180);
        }
        else if (counter == 30) {
            ChangeFontColor(0xe0);
            Wait(5);
        }
        else if (counter == 15) {
            //ScrollDownFont();
            ChangeFontColor(0x50);
            Wait(5);
        }
    }

    ChangeFontColor(0x10); // Hide text
    Wait(60);

}

void Intro() {

    InitVRAM();

    ChangeFontColor(0x10); // Hide text

    DrawString("A game by ", 11, 9);
    DrawString("ANDRE BAPTISTA  ", 9, 11);
    FadeOut();

    ClearNamesTable();
    DrawString("Powered by", 11, 9);
    DrawString("8 BIT WORKSHOP  ", 9, 11);
    FadeOut();

    ClearNamesTable();
    DrawString("Powered by", 11, 9);
    DrawString(" MSX TEMPLATES  ", 9, 11);
    DrawString("by DANILO ANGELO", 8, 13);
    FadeOut();

    ClearNamesTable();
    DrawString("Beta tester", 11, 9);
    DrawString("BIANCA BAPTISTA ", 9, 11);
    FadeOut();

}

#define A		TITLE_1
#define B		(TITLE_1 + 1)
#define C		(TITLE_1 + 2)
#define D		(TITLE_1 + 3)
#define E		(TITLE_1 + 4)
#define F		(TITLE_1 + 5)
#define G		(TITLE_1 + 6)






void SetBlock(byte col, byte line, byte tileNumber) {
    WRTVRM(NAMTBL + col + (line * 32), tileNumber);
}

void TitleScreen() {
    byte colorIndex = 0;

    // Title screen names table (32 x 24 chars)
    static const unsigned char title[] = {
        0, 0, 0, 0, A, A, A, A, 0, A, A, A, A, A, A, 0, 0, A, A, 0, 0, 0, A, A, 0, 0, 0, 0, A, A, A, A,
        0, 0, 0, A, A, A, A, A, 0, A, A, 0, 0, 0, A, A, 0, 0, A, A, 0, 0, A, A, 0, 0, 0, A, A, A, A, A,
        0, 0, A, A, A, 0, A, A, 0, A, A, 0, 0, 0, A, A, 0, 0, 0, A, A, 0, A, A, 0, 0, A, A, A, 0, A, A,
        0, A, A, A, 0, 0, A, A, 0, A, A, 0, 0, A, A, 0, 0, 0, 0, 0, A, A, A, 0, 0, A, A, A, 0, 0, A, A,
        0, A, A, 0, 0, 0, A, A, 0, A, A, A, A, A, 0, 0, 0, 0, 0, 0, A, A, 0, 0, 0, A, A, 0, 0, 0, A, A,
        0, A, A, 0, 0, 0, A, A, 0, A, A, A, A, A, 0, 0, 0, 0, 0, 0, A, A, 0, 0, 0, A, A, 0, 0, 0, A, A,
        0, A, A, A, A, A, A, A, 0, A, A, 0, 0, 0, A, 0, 0, 0, 0, 0, A, A, 0, 0, 0, A, A, A, A, A, A, A,
        0, A, A, 0, 0, 0, A, A, 0, A, A, 0, 0, 0, A, 0, 0, 0, 0, 0, A, A, 0, 0, 0, A, A, 0, 0, 0, A, A,
    };


    const char blocks[] = {
      A, // yellow
      B, // yellow
      C, // magenta
      D, // white
      E, // cyan
      F, // blue
      G, // blue
    };

    const char colors[] = { A, C, E, G }; // last item will be ignored
    //const char colors[] = { A, C }; // test

    InitVRAM();

    LDIRVM(NAMTBL, title, sizeof(title));

    DrawString("PRESS TRIGGER TO START", 5, 13);
    DrawString("ANDREBAPTISTA.COM.BR", 0, 23);
    DrawString("v.1.0.0", 32 - 7, 23);

    while (TRUE) {
        byte index = 0, col_1, line_1; //, col_2, line_2, col_3, line_3;
        //byte rnd;
        byte spaceBar, btn1, btn2;
        byte value;
        word counter = 0;
        //byte currentColor, nextColor;

        //word lastJiffy = JIFFY;
        //while (JIFFY == lastJiffy) {
        //}

        // Title animation v-sync'ed starts here


        do {

            //currentColor = colors[0];
            //nextColor = colors[colorIndex];

            //DrawNumber(nextColor, 0, 15);

            col_1 = GetRandomInInterval(31, 0b00011111);
            line_1 = GetRandomInInterval(7, 0b00000111);
            //col_2 = GetRandomInInterval(31, 0b00011111);
            //line_2 = GetRandomInInterval(7, 0b00000111);
            value = RDVRM(NAMTBL + col_1 + (line_1 * 32));

            // Next color
            if (counter++ > 1000) {
                colorIndex++;
                if (colorIndex >= sizeof(colors) - 1) colorIndex = 0;
                /*
                if(colorIndex == sizeof(colors) - 1) {
                  currentColor = colors[sizeof(colors) - 1];  // last color
                  nextColor = colors[0];                      // first color
                }
                else {
                  if(colorIndex == sizeof(colors)) {
                    colorIndex = 0;
                  }
                  currentColor = colors[colorIndex];
                  nextColor = colors[colorIndex + 1];
                }
                */

                counter = 0;
            }
        } while (value == EMPTY || value == colors[colorIndex + 1]);
        /*
        do {
          col_2 = GetRandomInInterval(31, 0b00011111);
          line_2 = GetRandomInInterval(7, 0b00000111);
        }
        while (RDVRM(NAMTBL + col_2 + (line_2 * 32)) == EMPTY);

        do {
          col_3 = GetRandomInInterval(31, 0b00011111);
          line_3 = GetRandomInInterval(7, 0b00000111);
        }
        while (RDVRM(NAMTBL + col_3 + (line_3 * 32)) == EMPTY);
        */
        //rnd = GetRandomInInterval(7, 0b00000111);

        for (byte i = 0; i < 10 + 0; i++) {

            word lastJiffy = JIFFY;
            while (JIFFY == lastJiffy) {
            }

            // Animation v-sync'ed starts here

            SetBlock(col_1, line_1, blocks[index]);
            //SetBlock(col_2, line_2, blocks[index]);
            //SetBlock(col_3, line_3, blocks[index]);

            if (index++ >= sizeof(blocks)) index = 0;

            //uint8_t GTTRIG(uint8_t index) __z88dk_fastcall;
            /*
            // parameter for GTTRIG
            typedef enum GTTRIG_Param {
              TRIG_Spacebar,
              TRIG_Joy1_A,
              TRIG_Joy2_A,
              TRIG_Joy1_B,
              TRIG_Joy2_B
            };
            */
            spaceBar = GTTRIG(TRIG_Spacebar);
            btn1 = GTTRIG(TRIG_Joy1_A);
            btn2 = GTTRIG(TRIG_Joy1_B);

            if (spaceBar == 0xff || btn1 == 0xff || btn2 == 0xff) {
                rndSeed = JIFFY;
                return;
            }
        }

        SetBlock(col_1, line_1, colors[colorIndex + 1]);

    }
}

void SoundFx_2() {
    GICINI();

    //__asm__("HALT");


    /*
    WRTPSG
    Address  : #0093
    Function : Writes data to PSG register
    Input    : A  - PSG register number
               E  - Data write
    */


    //uint16_t WRTPSG(uint16_t reg_data) __z88dk_fastcall;
    WRTPSG(0x0f08);

    for (byte i = 0; i < 255; i++) {
        WRTPSG((93 * 256) + 0);
        WRTPSG((0 * 256) + 1);
    }

    for (byte i = 0; i < 255; i++) {
        WRTPSG((45 * 256) + 0);
        WRTPSG((0 * 256) + 1);
    }

    WRTPSG((0 * 256) + 8);	// register 8, value 0
}

void SoundFx_1() {

    GICINI();

    WRTPSG((151 * 256) + 6);

    // Bits 7 and 6 serves to specify the direction of PSG I/O ports. 
    // Always set bit 7 and reset bit 6. Otherwise some devices connected to the joystick ports may malfunction.
    WRTPSG((0b10000001 * 256) + 7);

    WRTPSG((16 * 256) + 8);
    WRTPSG((19 * 256) + 12);
    WRTPSG((1 * 256) + 13);

    //for(word i=0; i<65000; i++) { }
    //Wait(180);
    //while(1) {}
}


/*void Sound() {
  GICINI();

  __asm__("ld a, 8");
  __asm__("ld e, 15");
  __asm__("call 0x0093");
}*/



//#define POINTS_PER_BLOCK_REMOVED 	      10
//#define POINTS_PER_COMBO 	              5

// void CheckIfPlayfieldIsValid() {
//   bool found = FALSE;
//   for(byte line = 0; line < LINES_PLAYFIELD; line++) {
//     for(byte col = 0; col < COLS_PLAYFIELD; col++) {
//       for(byte item = 0; item < 5; item++) {
//         found = FALSE;
//       	if(playfieldTemp[col][line] == pieces[item] || playfieldTemp[col][line] == EMPTY) {
//           found = TRUE;
//           break;
//         }
//       }
//       if(!found) {
//         d_col = col;
//         d_line = line;
//         d_value = playfieldTemp[col][line];

//         while(1) { 
//           BEEP();
//         }
//       }

//     }
//   }
// }


void CheckPlayfield(byte iteration) {
    bool piecesRemoved = FALSE;

    // Save copy of playfield
    for (byte line = 0; line < LINES_PLAYFIELD; line++) {
        for (byte col = 0; col < COLS_PLAYFIELD; col++) {
            playfieldTemp[line][col] = playfield[line][col];
        }
    }

    // Check lines
    for (byte line = 0; line < LINES_PLAYFIELD; line++) {
        for (byte col = 2; col < COLS_PLAYFIELD; col++) {
            if (playfieldTemp[line][col] != EMPTY &&
                playfieldTemp[line][col - 2] == playfieldTemp[line][col - 1] &&
                playfieldTemp[line][col - 1] == playfieldTemp[line][col]) {

                piecesRemoved = TRUE;

                // Set cells to removing status
                playfield[line][col - 2] = playfieldTemp[line][col - 2] | REMOVING_FLAG;
                playfield[line][col - 1] = playfieldTemp[line][col - 1] | REMOVING_FLAG;
                playfield[line][col] = playfieldTemp[line][col] | REMOVING_FLAG;
            }
        }
    }

    // Check columns
    for (byte line = 2; line < LINES_PLAYFIELD; line++) {
        for (byte col = 0; col < COLS_PLAYFIELD; col++) {
            if (playfieldTemp[line][col] != EMPTY &&
                playfieldTemp[line - 2][col] == playfieldTemp[line - 1][col] &&
                playfieldTemp[line - 1][col] == playfieldTemp[line][col]) {

                piecesRemoved = TRUE;

                // Set cells to removing status
                playfield[line - 2][col] = playfieldTemp[line - 2][col] | REMOVING_FLAG;
                playfield[line - 1][col] = playfieldTemp[line - 1][col] | REMOVING_FLAG;
                playfield[line][col] = playfieldTemp[line][col] | REMOVING_FLAG;
            }
        }
    }

    // Check diagonals
    for (byte line = 2; line < LINES_PLAYFIELD; line++) {
        for (byte col = 2; col < COLS_PLAYFIELD; col++) {
            if (playfieldTemp[line][col] != EMPTY &&
                playfieldTemp[line - 2][col - 2] == playfieldTemp[line - 1][col - 1] &&
                playfieldTemp[line - 1][col - 1] == playfieldTemp[line][col]) {

                piecesRemoved = TRUE;

                // Set cells to removing status
                playfield[line - 2][col - 2] = playfieldTemp[line - 2][col - 2] | REMOVING_FLAG;
                playfield[line - 1][col - 1] = playfieldTemp[line - 1][col - 1] | REMOVING_FLAG;
                playfield[line][col] = playfieldTemp[line][col] | REMOVING_FLAG;
            }
            if (playfieldTemp[line - 2][col] != EMPTY &&
                playfieldTemp[line - 1][col - 1] == playfieldTemp[line][col - 2] &&
                playfieldTemp[line][col - 2] == playfieldTemp[line - 2][col]) {

                piecesRemoved = TRUE;

                // Set cells to removing status
                playfield[line - 2][col] = playfieldTemp[line - 2][col] | REMOVING_FLAG;
                playfield[line - 1][col - 1] = playfieldTemp[line - 1][col - 1] | REMOVING_FLAG;
                playfield[line][col - 2] = playfieldTemp[line][col - 2] | REMOVING_FLAG;
            }
        }
    }

    if (piecesRemoved) {
        byte numberPiecesRemoved;
        byte oldLevel;

        // Animation
        byte counter = 72;
        byte x = 255, y = 0;

        byte leftmostPieceRemoved = 5, rightmostPieceRemoved = 0;
        byte lowermostPieceRemoved = 11, upmostPieceRemoved = 0;

        HideArrowSprite();

        SoundFx_2();

        // Count pieces removed
        numberPiecesRemoved = 0;
        for (byte line = 0; line < LINES_PLAYFIELD; line++) {
            for (byte col = 0; col < COLS_PLAYFIELD; col++) {
                if ((playfield[line][col] & REMOVING_FLAG) != 0) {
                    numberPiecesRemoved++;

                    if (col < leftmostPieceRemoved) leftmostPieceRemoved = col;
                    if (col > rightmostPieceRemoved) rightmostPieceRemoved = col;
                    if (line < lowermostPieceRemoved) lowermostPieceRemoved = line;
                    if (line > upmostPieceRemoved) upmostPieceRemoved = line;
                }
            }
        }

        // debug
        // DrawNumber(numberPiecesRemoved, 0, 0);//test
        // DrawNumber(iteration, 0, 1);//test
        // DrawNumber(leftmostPieceRemoved, 0, 13);//test
        // DrawNumber(rightmostPieceRemoved, 0, 14);//test
        // DrawNumber(lowermostPieceRemoved, 0, 16);//test
        // DrawNumber(upmostPieceRemoved, 0, 17);//test
        //Wait(60);

        while (counter-- > 0) {

            word lastJiffy = JIFFY;
            while (lastJiffy == JIFFY) {
            }
            // Animation loop sync'ed at 60/50 Hz starts here


            // "?x HIT" sprite logic
            if (numberPiecesRemoved >= 3) {

                if (numberPiecesRemoved >= 7) numberPiecesRemoved = 7;

                if (x == 255) { // do this expensive calculation only once
                    x = (PLAYFIELD_HORIZ_OFFSET * 8) + (((rightmostPieceRemoved * 16) - (leftmostPieceRemoved * 16)) / 2) + (leftmostPieceRemoved * 16);
                    //y = (((lowermostPieceRemoved * 16) - (upmostPieceRemoved * 16)) / 2) + (upmostPieceRemoved * 16) - (72 - counter);
                    y = (((lowermostPieceRemoved * 16) - (upmostPieceRemoved * 16)) / 2) + (upmostPieceRemoved * 16);
                    DrawHitSprite(numberPiecesRemoved, iteration, x, y, TRUE);
                }
                else {
                    //y -= (72 - counter);
                    // TODO: refactor here (line repeated; code expensive unnecessary)
                    //y = (((lowermostPieceRemoved * 16) - (upmostPieceRemoved * 16)) / 2) + (upmostPieceRemoved * 16) - (72 - counter);
                    DrawHitSprite(numberPiecesRemoved, iteration, x, y - (72 - counter), FALSE);
                }
            }

            for (byte line = 0; line < LINES_PLAYFIELD; line++) {
                for (byte col = 0; col < COLS_PLAYFIELD; col++) {

                    if ((playfield[line][col] & REMOVING_FLAG) != 0) {

                        // Animation 1: blocks blinking
                        if (counter > 12) {
                            if (JIFFY & 0b00000011) {
                                DrawBlock8(col, line, playfield[line][col] & 0b01111111);
                            }
                            else {
                                DrawBlock8(col, line, EMPTY);
                            }
                        }
                        else {
                            // Animation 2: blocks turning into dust
                            if (counter > 9) DrawBlock_SameTile(col, line, DUST_1);
                            else if (counter > 6) DrawBlock_SameTile(col, line, DUST_1 + 1);
                            else if (counter > 3) DrawBlock_SameTile(col, line, DUST_1 + 2);
                            else DrawBlock_SameTile(col, line, DUST_1 + 3);
                        }

                    }
                }
            }
        }

        HideHitSprite();



        // After animation
        for (byte line = 0; line < LINES_PLAYFIELD; line++) {
            for (byte col = 0; col < COLS_PLAYFIELD; col++) {

                if ((playfield[line][col] & REMOVING_FLAG) != 0) {

                    blocksRemoved++;

                    // Adjust the column above
                    for (byte line1 = line; line1 > 0; line1--) {
                        byte linesToBeRemoved = 1;
                        if ((playfield[line1 - 1][col] & REMOVING_FLAG) != 0) linesToBeRemoved++;
                        if ((playfield[line1 - 2][col] & REMOVING_FLAG) != 0) linesToBeRemoved++;

                        //playfield[col][line1] = playfieldTemp[col][line1 - linesToBeRemoved];
                        //playfield[col][line1] = playfield[col][(line1 - linesToBeRemoved >= 0) ? line1 - linesToBeRemoved : 0];
                        playfield[line1][col] = (line1 - linesToBeRemoved >= 0) ? playfield[line1 - linesToBeRemoved][col] : EMPTY;
                    }
                }
            }
        }

        // Wait(60);
        //score = score + numberPiecesRemoved + POINTS_PER_COMBO; // causing slowdown
        //score++;                                                  // works fine

        // Level logic
        oldLevel = level;
        if (blocksRemoved >= 90) {
            level = (blocksRemoved / 30) + 1;
            speed = 15;
            //DrawNumber(speed, 0, 0); // test
        }
        else if (blocksRemoved >= 60) {
            level = 3;
            speed = 30;
        }
        else if (blocksRemoved >= 30) {
            level = 2;
            speed = 45;
        }
        else {
            level = 1;
            speed = 60;
        }

        if (oldLevel != level) {
            newLevel = TRUE;
        }


        //DrawPlayfield();

        //Wait(90);

        //CheckIfPlayfieldIsValid(); // test

        //DrawScore();

        iteration++;
        CheckPlayfield(iteration);
    }
    else {
        //SoundFx_1();
        //DrawScore();
    }
}



byte RandomIndex() {
    byte random;
    do {
        random = GetRandom() & 0b00000111; // AND mask to get a value in the 0-7 range
    } while (random >= 7); // Get a number between 0 and 7

    return random;
}


void get_piece(Piece* piece)
{
    piece->id = RandomIndex();
    //piece->id = 0;
    memcpy(piece->shape, shapes[piece->id], sizeof(piece->shape));
}

void RandomPiece() {
    g_piece = g_next_piece;
    get_piece(&g_next_piece);
}

byte rect_piece = TILE_RED;

void UpdateAndDrawPieceStatic() {

    // Update and draw piece static
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            
            int tile = g_piece.shape[row][col];
            if (tile)
            {
                playfield[g_line + row][g_col + col] = rect_piece;
            }
        }
    }


    DrawPiece(g_line);

    // Set piece to next
    g_col = INITIAL_COL;
    g_line = INITIAL_LINE;

    counter = 0;

    // Check if initial place of piece is occupied
    if (playfield[g_line][g_col] != EMPTY ||
        playfield[g_line + 1][g_col] != EMPTY ||
        playfield[g_line + 2][g_col] != EMPTY) {
        gameOver = TRUE;
    }
    else {
        RandomPiece();

        //CheckPlayfield(1);

       // DrawNextPiece();
    }

    DrawPlayfield();
}

void BlinkPauseText() {
    if (JIFFY & 0b00001000) {
        DrawString("PAUSE", 13, 12);
    }
    else {
        //if(JIFFY & 0b11110000) {
          // Clear PAUSE text
        DrawLine(6);
        //}
    }
}

void Pause() {
    byte keyboard;

    DrawString("PAUSE", 13, 12);

    while (TRUE) {

        BlinkPauseText();

        // Check if ESC key is released
        keyboard = SNSMAT(7);
        if ((keyboard & 0b00000100) != 0) {

            while (TRUE) {

                BlinkPauseText();

                // Check if ESC key is pressed again
                keyboard = SNSMAT(7);
                if ((keyboard & 0b00000100) == 0) {

                    while (TRUE) {

                        BlinkPauseText();

                        // Check if ESC key is released again
                        keyboard = SNSMAT(7);
                        if ((keyboard & 0b00000100) != 0) {
                            // Clear PAUSE text
                            DrawLine(6);
                            return;
                        }
                    }

                }
            }

        }
    }

}

uint8_t colliding(const Piece* piece)
{
    for (uint8_t row = 0; row < 4; row++)
    {
        for (uint8_t col = 0; col < 4; col++)
        {
            if (piece->shape[row][col])
            {
                uint8_t x = g_line + row;
                uint8_t y = g_col + col;

                if (y < 0 || x < 0 || x >= LINES_PLAYFIELD || y >= COLS_PLAYFIELD)
                    return 1;

                if (playfield[x][y] != EMPTY)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int rotate()
{
    Piece tmp;
    memcpy(&tmp, &g_piece, sizeof(Piece));


    //조각은 회전할 경우 3x4 또는 4 x 3안에 타일이 모두 들어간다.
    if (g_piece.id == 0 || g_piece.id == 1)
    {
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                tmp.shape[row][col] = g_piece.shape[col][row];
            }
        }
    }
    else if(g_piece.id == 4 || g_piece.id == 5)
    {
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                tmp.shape[row][col] = g_piece.shape[3 - col - 1][row];
            }
        }
    }
    else
    {
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                tmp.shape[row][col] = g_piece.shape[3 - col - 1][row];
            }
        }
    }

    if (colliding(&tmp))
        return 0;

    g_piece = tmp;

    return 1;
}


void InitPiecePlayField()
{
    for (uint8_t row = 0; row < 4; row++)
    {
        for (uint8_t col = 0; col < 4; col++)
        {
            
            playfield[g_line + row][g_col + col] = EMPTY;
        }
    }
}

void UpdatePlayField()
{
    for (uint8_t row = 0; row < 4; row++)
    {
        for (uint8_t col = 0; col < 4; col++)
        {
            int tile = g_piece.shape[row][col];
            if (tile)
            {
                playfield[g_line + row][g_col + col] = rect_piece;
            }
            else
                playfield[g_line + row][g_col + col] = EMPTY;
        }
    }
}

void GameLoop() {

    byte keyboard, joystick, btn1, btn2;
    byte lastJoystick_LeftRight = STCK_none, lastJoystick_UpDown = STCK_none;
    byte lastBtn1 = 0, lastBtn2 = 0;
    byte bgCounter = 0, bgColor = 0x00;

    while (!gameOver) {

        word lastJiffy = JIFFY;
        //d_value = 0;
        while (lastJiffy == JIFFY) {
            //d_value++;
        }
        // Game loop sync'ed at 60/50 Hz starts here




        // Clear piece before update position

        InitPiecePlayField();
        //playfield[col][line] = EMPTY;
        //playfield[col][line + 1] = EMPTY;
        //playfield[col][line + 2] = EMPTY;

        // Read player input
        joystick = GTSTCK(STCK_Joy1);
        btn1 = GTTRIG(TRIG_Joy1_A);
        btn2 = GTTRIG(TRIG_Joy1_B);

        if (joystick == STCK_none) {
            joystick = GTSTCK(STCK_Cursors);
        }

        if (lastJoystick_LeftRight == STCK_none) {
            //if(lastJoystick_LeftRight != STCK_W && lastJoystick_LeftRight != STCK_E) {
            if (joystick == STCK_W && g_col > 0)
            {
                g_col--;

                if(colliding(&g_piece))
                    g_col++;
            }
            else if (joystick == STCK_E && g_col < COLS_PLAYFIELD - 1)
            {
                g_col++;

                if (colliding(&g_piece))
                    g_col--;
            }  
        }

        if (lastJoystick_UpDown == STCK_none) {
            //if(lastJoystick_UpDown != STCK_W && lastJoystick_UpDown != STCK_E) {

              // Rotate piece
            if (joystick == STCK_N || (lastBtn1 == 0 && btn1 == 0xff) || (lastBtn2 == 0 && btn2 == 0xff)) {
                rotate();
            }

            // Fall piece until hit bottom or other piece
            if (joystick == STCK_S) 
            {
                while (1)
                {
                    if (colliding(&g_piece))
                    {
                        g_line--;
                        break;
                    }

                    g_line++;
                }

                DrawPiece(g_line);
                UpdateAndDrawPieceStatic();

                /*
                for (byte i = g_line; i < LINES_PLAYFIELD; i++) 
                {
                    if (i == LINES_PLAYFIELD - 4 || playfield[i + 4][g_col] != EMPTY) {

                        // Clear current piece lines
                        DrawPiece(g_line);

                        g_line = i;

                        UpdateAndDrawPieceStatic();
                        break;
                    }
                }*/

            }

        }

        //if(joystick == STCK_W || joystick == STCK_E) lastJoystick_LeftRight = joystick; else lastJoystick_LeftRight = STCK_none;
        //if(joystick == STCK_N || joystick == STCK_S) lastJoystick_UpDown = joystick; else lastJoystick_UpDown = STCK_none;
        lastJoystick_LeftRight = joystick;
        lastJoystick_UpDown = joystick;
        lastBtn1 = btn1;
        lastBtn2 = btn2;

        // Read keyboard
        // http://map.grauw.nl/articles/keymatrix.php
        /*
                bit 7	bit 6	bit 5	bit 4	bit 3	bit 2	bit 1	bit 0
        row 0	7 &	6 ^	5 %	4 $	3 #	2 @	1 !	0 )
        row 1	; :	] }	[ {	\ ?	= +	- _	9 (	8 *
        row 2	B	A	DEAD	/ ?	. >	, <	` ~	' "
        row 3	J	I	H	G	F	E	D	C
        row 4	R	Q	P	O	N	M	L	K
        row 5	Z	Y	X	W	V	U	T	S
        row 6	F3	F2	F1	CODE	CAPS	GRAPH	CTRL	SHIFT
        row 7	RET	SELECT	BS	STOP	TAB	ESC	F5	F4
        row 8	→	↓	↑	←	DEL	INS	HOME	SPACE
        row 9	NUM4	NUM3	NUM2	NUM1	NUM0	NUM/	NUM+	NUM*
        row 10	NUM.	NUM,	NUM-	NUM9	NUM8	NUM7	NUM6	NUM5

        SNSMAT
        Function : Returns the value of the specified line from the keyboard matrix
        Input    : A  - For the specified line
        Output   : A  - For data (the bit corresponding to the pressed key will be 0)
        Registers: AF
        */
        keyboard = SNSMAT(7);
        // Check if ESC key is pressed
        if ((keyboard & 0b00000100) == 0) {
            Pause();
        }


        // Piece falling logic
        counter++;
        if (counter == speed) {

            counter = 0;

            // Draw piece before update position
            DrawPiece(g_line);


            // Check if piece hit bottom or other piece
            if (g_line == LINES_PLAYFIELD - 4 || playfield[g_line + 4][g_col] != EMPTY) {

                UpdateAndDrawPieceStatic();
            }

            g_line++;

        }

        UpdatePlayField();

        //Set piece at updated position
        //playfield[col][line] = topPiece;
        //playfield[col][line + 1] = midPiece;
        //playfield[col][line + 2] = bottomPiece;



        // Draw piece at current position
        DrawPiece(g_line);



        // Draw arrow sprite indicating where the piece will drop
        if (playfield[g_line + 4][g_col] != EMPTY) {
            HideArrowSprite();
        }
        else {
            for (byte i = g_line + 4; i < LINES_PLAYFIELD; i++) {
                if (i == LINES_PLAYFIELD - 1 && playfield[i][g_col] == EMPTY) {
                    DrawArrowSprite((g_col * 16) + (PLAYFIELD_HORIZ_OFFSET * 8), (i) * 16);
                    break;
                }
                else if (playfield[i][g_col] != EMPTY) {
                    DrawArrowSprite((g_col * 16) + (PLAYFIELD_HORIZ_OFFSET * 8), (i - 1) * 16);
                    break;
                }
            }
        }

    }

    DrawString("GAME OVER", 12, 12);

    //while(1) { };

    // Wait 5 seconds
    Wait(60 * 5);

}

/*void TestCase() {
    // playfield[2][ 8] = TILE_GREEN;
    // playfield[2][ 9] = TILE_BLUE;

    // playfield[0][10] = TILE_BLUE;
    // playfield[1][10] = TILE_BLUE;
    // playfield[2][10] = TILE_RED;
    // playfield[4][10] = TILE_RED;
    // playfield[5][10] = TILE_BLUE;

    // playfield[0][11] = TILE_GREEN;
    // playfield[1][11] = TILE_GREEN;
    // playfield[2][11] = TILE_RED;
    // playfield[3][11] = TILE_BLUE;
    // playfield[4][11] = TILE_RED;
    // playfield[5][11] = TILE_RED;

    // topPiece = TILE_BLUE;
    // midPiece = TILE_RED;
    // bottomPiece = TILE_YELLOW;

    playfield[2][8] = TILE_GREEN;
    playfield[2][9] = TILE_BLUE;

    playfield[0][10] = TILE_BLUE;
    playfield[1][10] = TILE_BLUE;
    playfield[2][10] = TILE_RED;
    playfield[4][10] = TILE_RED;
    playfield[5][10] = TILE_BLUE;

    playfield[0][11] = TILE_GREEN;
    playfield[1][11] = TILE_GREEN;
    playfield[2][11] = TILE_RED;
    playfield[3][11] = EMPTY;
    playfield[4][11] = TILE_RED;
    playfield[5][11] = TILE_RED;

    topPiece = TILE_BLUE;
    midPiece = TILE_RED;
    bottomPiece = TILE_RED;
}*/

void InitGame() {

    gameOver = FALSE;
    g_line = INITIAL_LINE;
    g_col = COLS_PLAYFIELD / 2 - 2;;
    //score = 0;
    blocksRemoved = 0;
    level = 1;
    newLevel = FALSE;
    speed = 60;

    InitVRAM();

    // Using user input on title screen to seed random number generator
    InitRnd(rndSeed, JIFFY * rndSeed, JIFFY * rndSeed + 99);

    // Call RandomPiece twice, to populate current and next pieces
    RandomPiece();
    RandomPiece();

    // Reset playfield
    for (byte line = 0; line < LINES_PLAYFIELD; line++) {
        for (byte col = 0; col < COLS_PLAYFIELD; col++) {
            playfield[line][col] = EMPTY;
        }
    }

    // Testing code
    //TestCase();

    //newLevel = TRUE;  //test

    //DrawBackground();

    //DrawPlayfield();

    //DrawScore();

    //ShowCountdown();

    //DrawNextPiece();

    GameLoop();
}
