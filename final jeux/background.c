#include "background.h"


void init_background ( background* bg)
{
	bg->bg = IMG_Load("bg.jpg");
	bg->pos_bg.x = 0;
	bg->pos_bg.y = 0;

	bg->pos_scrolling.x = 0;
	bg->pos_scrolling.y = 0;
	bg->pos_scrolling.w = 1794;
	bg->pos_scrolling.h = 1024;
}

void blit_background(background* bg,SDL_Surface *screen)
{
	SDL_BlitSurface(bg->bg,&bg->pos_scrolling , screen , &bg->pos_bg );
}


void scrolling_right (background* bg, int* test_scrolling_end_right ) 
{
	if ( bg->pos_scrolling.x < 4500 ) // 780 kifesh l9ineha |||  ( width of image - width of screen )  1600 - 800 = 800
	{
		bg->pos_scrolling.x +=3;
		*test_scrolling_end_right = 0;
	}
	else{
		*test_scrolling_end_right = 1;
	}

}

void scrolling_left (background* bg , int* test_scrolling_end_left) 
{

	if ( bg->pos_scrolling.x > 20 )
	{
		bg->pos_scrolling.x -=2;
		*test_scrolling_end_left = 0;
	}
	else{
		*test_scrolling_end_left = 1;
	}

	

}


void free_background(background* bg)
{
	SDL_FreeSurface(bg->bg);
}
