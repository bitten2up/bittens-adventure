/*
* MIT License
*
* Copyright (c) 2021-2023 bitten2up
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "defaultSave.h"	// default savefile

#include "sdl_bittendef.h"
#include "g_game.h"
#include "f_save.h"

#define VERSION 0x00

// self explanitory utiliy
int getBit(char byte, int bitNum)
{
    return (byte & (0x1 << (bitNum - 1)));
}

bool file_exists(const char *filename)
{
  FILE *fp = fopen(filename, "r");
  bool is_exist = false;
  if (fp != NULL)
  {
    is_exist = true;
    fclose(fp); // close the file
  }
  return is_exist;
}

void resetGame();
// loads the game on start
int loadGame(g_game* game)
{
  // check if the save exists before reading it
  if (!file_exists("bitten.sav")) {
    resetGame();
  }
  else {
		FILE *f1 = fopen("bitten.sav", "rb"); // open in binary mode
		char* buffer;
		long saveSize;
    // if file is null reset save
		if (f1 == NULL){
		  fclose(f1);
		  resetGame();
		  f1 = fopen("bitten.sav", "rb");
		}
		fseek(f1, 0L, SEEK_END);
		saveSize = ftell(f1);
		rewind(f1);
		buffer = calloc(1, saveSize+1);
    // see if there is a buffer
		if (buffer == NULL)
		{
			fclose(f1);
			free(buffer);
			fputs("mem alloc failed\n", stderr);
			return 1;
		}
    // is the file blank
		if (1 != fread(buffer,saveSize, 1, f1))
		{
			fclose(f1);
			fputs("read failed, or file is blank\n", stderr);
			resetGame();
		}
    // save is wrong size (too small) need to make this run after we check version of the save to prevent loss of progress, but i havent done that yet
		if (saveSize < sizeof(saveD)) {
			fclose(f1);
			fputs("INVALID SAVE DATA\n", stderr);
			resetGame();
			f1 = fopen("bitten.sav", "rb");
		}
		// read the data
#ifdef DEBUG
		printf("header:\n");
#endif
		for (int i = 0; i <= 9; i++)
		{
      // header is wrong
			if (buffer[i] != saveD[i])
			{
				printf("header mismatch");
				fclose(f1);
				resetGame(game);
				f1 = fopen("bitten.sav", "rb");
			}
#ifdef DEBUG
			printf("%c", buffer[i]);
#endif
		}
		printf("\n");

		printf("Version of game saved: %i\n", buffer[HEADERVERSION]);
		if (buffer[HEADERVERSION] > VERSION)
		{
			printf("Version of save's saving format is greater than the current version, aborting to prevent overwriting the save\nPlease update to the latest version\n");
			fclose(f1);
			exit(1);
		}
		// i need to redo this, because this makes no fucking sense rn
		for(int i = 7; 0 <= i; i --) {
			if (i == 7) {
				if (getBit(buffer[SETTINGS], 1)) {
					printf("music enabled\n");
					game->settings.audio=true;
				}
			}
		}

    // buffers to read the x cords
		char x[4];
		x[3]=buffer[SAVEDXPOS];
		x[2]=buffer[SAVEDXPOS+1];
		x[1]=buffer[SAVEDXPOS+2];
		x[0]=buffer[SAVEDXPOS+3];

#ifdef DEBUG
		printf("xpos: %i\n", (x[3]  << 24) | (x[2] << 16) | (x[1] << 8) | x[0]);
#endif
    // bitmask to read x as a 32 bit int
		game->player.x = (x[3]  << 24) | (x[2] << 16) | (x[1] << 8) | x[0];
	  // buffer to read the y cords
	  char y[4];
		y[3]=buffer[SAVEDYPOS];
		y[2]=buffer[SAVEDYPOS+1];
		y[1]=buffer[SAVEDYPOS+2];
		y[0]=buffer[SAVEDYPOS+3];
#ifdef DEBUG
		printf("ypos: %i\n", (y[3]  << 24) | (y[2] << 16) | (y[1] << 8) | y[0]);
#endif
    // bitmask to read y as a 32 bit int
		game->player.y = (y[3]  << 24) | (y[2] << 16) | (y[1] << 8) | y[0];
		game->invalidSave = false;
		// free memory
		fclose(f1);
		free(buffer);
	}
	return 0;
}
// saves the game, takes g_game to get the data wanted to save
void saveGame(g_game* game)
{
	FILE* f1 = fopen("bitten.sav", "wb");
	
#ifdef DEBUG
	printf("xpos: %i\n", game->player.x);
	printf("ypos: %i\n", game->player.y);
#endif
// 12-15 are for the x position
	saveD[SAVEDXPOS] = (game->player.x>>24) & 0xFF;
	saveD[SAVEDXPOS+1] = (game->player.x>>16) & 0xFF;
	saveD[SAVEDXPOS+2] = (game->player.x>>8) & 0xFF;
	saveD[SAVEDXPOS+3] = game->player.x & 0xFF;
// 16-19 are for the y position
	saveD[SAVEDYPOS] = (game->player.y>>24) & 0xFF;
	saveD[SAVEDYPOS+1] = (game->player.y>>16) & 0xFF;
	saveD[SAVEDYPOS+2] = (game->player.y>>8) & 0xFF;
	saveD[SAVEDYPOS+3] = game->player.y & 0xFF;
#ifdef DEBUG
	printf("saved data\n");
	printf("%s", saveD);
#endif
	fwrite(saveD, sizeof(char), sizeof(saveD), f1);
	fclose(f1);
	return;
}

// called if the save file is corrupt, runs when the game starts
void resetGame()
{
	FILE* f1 = fopen("bitten.sav", "wb");
	#ifdef DEBUG
	printf("save data that is being created\n");
	printf("%s", saveD);
	printf("\n");
	#endif
	fwrite(saveD, sizeof(char), sizeof(saveD), f1);
	fclose(f1);
	return;
}
