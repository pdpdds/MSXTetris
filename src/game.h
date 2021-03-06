#ifndef __GAME_H__
#define __GAME_H__

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "msxbios.h"


typedef uint8_t 	byte;
typedef uint16_t 	word;
typedef uint8_t 	bool;

void Intro();
void TitleScreen();
word Power(byte base, byte expoent);
void Wait(word numberOfFrames);
void SoundFx_2();
void SoundFx_1();
//void Sound();

void CheckPlayfield();

void InitGame();
void FILVRM(uint16_t start, uint16_t len, uint8_t data);
void SetBlock(byte col, byte line, byte tileNumber);

#endif
