#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "defaultSave.h"	// default savefile
#include "bittendef.h"
#include "bit_file.h"
#define VERSION 0x01
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
void resetGame(bit_game* game);
int loadGame(bit_game* game)
{
    if (file_exists("bitten.sav")) {
		FILE *f1 = fopen("bitten.sav", "rb"); // open in binary mode
		char* buffer;
		long saveSize;
		if (f1!=NULL){
			fseek(f1, 0L, SEEK_END);
			saveSize = ftell(f1);
			rewind(f1);
			buffer = calloc(1, saveSize+1);
			if (!buffer)
			{
				fclose(f1);
				fputs("mem alloc failed", stderr);
				return 1;
			}
			if (1!=fread(buffer,saveSize, 1, f1))
			{
				fclose(f1);
				fputs("read failed", stderr);
				return 1;
			}
			// read and printout the data
			printf("header:\n");
			for (int i=0; i<9; i++)
			{
				if (buffer[i] != saveD[i])
				{
					printf("header mismatch");
					resetGame(game);
				}
				printf("%c",buffer[i]);
			}
			printf("\n");
			printf("Version of game saved: %i\n", buffer[10]);
			for(int i = 7; 0 <= i; i --) {
        		printf("%d\n", (buffer[11] >> i) & 0x01);
				if (i==7) {
					if ((buffer[11] >> i) & 0x01) {
						printf("music enabled\n");
						game->settings.audio=true;
					}
				}
			}
			printf("xpos: %i\n", buffer[12]);
			game->player.x=buffer[12];
			printf("ypos: %i\n", buffer[13]);
			game->player.y=buffer[13];
			// free memory
			fclose(f1);
			free(buffer);
		} // dont write default data to file that already exsits
		else {
			fclose(f1);
			resetGame(game);
    	}
    }
    else {
        resetGame(game);
    }
	return 0;
}
void saveGame(bit_game* game) {
	FILE* f1 = fopen("bitten.sav", "wb");
	
}
void resetGame(bit_game* game)
{
	FILE* f1 = fopen("bitten.sav", "wb");
	printf("save data that is being created\n");
	printf("%s", saveD);
	fwrite(saveD, sizeof(char), sizeof(saveD), f1);
	fclose(f1);
	return;
}
