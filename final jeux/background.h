#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <math.h>
#include <time.h>
#include "string.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>



struct background
{
	SDL_Surface* bg;
	SDL_Rect pos_bg;
	SDL_Rect pos_scrolling;
};
typedef struct background background; 



void init_background (background* bg);
void scrolling_right (background* bg , int* test_scrolling_end_right);
void scrolling_left (background* bg, int* test_scrolling_end_left);
void free_background (background* bg);
void blit_background (background* bg,SDL_Surface* screen);


#endif 
