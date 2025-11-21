#ifndef IMAGE_H
#define IMAGE_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

// Définition de la structure Image
struct Image {
    int x, y;
    int w, h;
    SDL_Rect pos1;
    SDL_Rect pos2;
    SDL_Surface *img;
};
typedef struct Image Image;

// Définition de la structure Text
typedef struct {
    SDL_Rect position;
    TTF_Font *font;
    SDL_Surface *surfaceTexte;
    SDL_Color textColor;
    char texte[1000];
} Text;

// Définition de la structure Animation
typedef struct {
    SDL_Surface *anim[20];
    SDL_Rect pos;
} Animation;

// Déclaration des fonctions
void initImage(Image *img, const char *imagePath, int x, int y, int w, int h);
void liberer0(Image *img);
void afficher0(Image img, SDL_Surface *screen);
void initText(Text *t);
void freeText(Text A);
void displayText(Text r, SDL_Surface *screen);
void initialiser_animation(Animation *animat);
void afficher_animation(Animation animat, SDL_Surface *screen);
void liberer_animation(Animation animat);
void playClickSound(int clickSoundEnabled,Mix_Chunk *clickSound);
void changeMusicVolume(int volumeChange);
void changeSoundVolume(Mix_Chunk *sound, int volumeChange);



void initSDL();

#endif // IMAGE_H




