#ifndef ENEMIES_H
#define ENEMIES_H


#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct{
    SDL_Surface *spriteSheet;
    SDL_Rect posScreen;
    SDL_Rect posSprite;
    int frameWidth;
    int frameHeight; 
    int numFrames; // Number of frames
    int currentFrame; // Currently displayed frame
    int frameDelay; // Time between frames
    int frameCounter; // Frame counter 
    int vitesse;
    int direction;
    int vert_direction;
    bool isActive; // Pour déterminer si l'entité doit être active  
}entity;
typedef struct {
    SDL_Surface *spriteSheet;
    SDL_Rect pos;
    int frameWidth;
    int frameHeight;
    int numFrames;
    int currentFrame;
    int coinDelay;
} Coin;
void initEntity(entity *e, const char *spritePath, int posX, int posY);
void afficherEnnemi(entity *e , SDL_Surface *screen);
void animer(entity *e , int etat) ;
int collisionTri(entity *e, SDL_Rect pose);
void move(entity *e);
void moveIA(entity *e);
void move2(entity *e);
void initCoin(Coin *c, const char *filePath, int x, int y);
void animateCoin(Coin *c);
void showCoin(Coin *c, SDL_Surface *screen);
void initClock(Coin *c, const char *filePath, int x, int y);

#endif 


