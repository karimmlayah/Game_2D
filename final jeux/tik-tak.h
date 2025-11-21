#ifndef tik_tak_h
#define tik_tak_h

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define SPACE_X 200 
#define SPACE_Y 200 
#define SIZE 3     
#define MAX_DEPTH 4 
typedef struct mini_game{
int game_board[SIZE][SIZE];
TTF_Font*font,  
        *result_font;
SDL_Surface*x_img,  
            *o_img, 
            *background,  
            *background_bars, 
            *result;            
SDL_Rect pos_screen,            
         pos_result;            
SDL_Color color_result; 
int number_animation_background;    
Uint32 timer_animation;         
int turn;                       
int game,                       
    game_over;                  
Uint32 animation;               
int start_animation;            
float Zoom;                     
int score;                      
int try;
}mini_game;

void init_mini_game(mini_game*game); 

int tik_tak(SDL_Surface*screen);        

void draw_board(mini_game game,SDL_Surface*screen); 
void animation(mini_game*game); 
void show_result(mini_game*game,char*result,SDL_Surface*screen);  
void handle_player_input(int x, int y,mini_game*game);  
int evaluate_game_state(mini_game game);                
int minmax(int depth,int player,mini_game*game);            
int ai_make_move(mini_game*game);
int check_win(mini_game game);      
int check_game_over(mini_game game);  
#endif
