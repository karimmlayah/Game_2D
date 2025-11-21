#ifndef IMAGE_H
#define IMAGE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define SCREEN_W 1792
#define SCREEN_H 1024

typedef struct {
    SDL_Rect pos1; // Position on the screen
    SDL_Rect pos2; // Part to display
    SDL_Surface *img; // Image frame

    int vy; // Vertical speed
    int saut; // Jump state
    int doubleJump;;
    int frameHeight; // Height of a single frame
    int numFrames; // Number of frames
    int currentFrame; // Currently displayed frame
    int frameDelay; // Time between frames
    int frameCounter; // Frame counter
    int direction; // Direction of player
    int acceleration; // Acceleration parameter
    int vie;
    int etatAnimation;
    int score;

    // Text rendering for the score
    int wKeyPressCount; // Counter for 'W' key presses
    char pressCountText[1000]; // To store the press count text display
} personne;

void initPlayer(personne *A, int x, int y, const char *imagePath);
void initlife(personne *A, int x);
void initshot(personne *A, int x, int y);
void initbackground(personne *backg);
void liberer(personne A);
void afficher(personne p, SDL_Surface *screen, TTF_Font *font, int multi, int joueurNum,SDL_Surface *image,SDL_Rect dest);
void afficherlife(personne p, SDL_Surface *screen) ;
void affichershot(personne p, SDL_Surface *screen) ;
void moveImage(personne *img, int dx, int dy);
void moveshot(personne *img, int dx);
void animerPlayer(personne *player, int direction);
void animerlife(personne *player, int etat);
void animershot(personne *player);

SDL_Surface* loadImage(const char* filePath);
void applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);

#endif // IMAGE_H
