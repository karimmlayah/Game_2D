#include "menu.h"
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>


void initImage(Image *img, const char *imagePath, int x, int y, int w, int h) {
    img->img = IMG_Load(imagePath);
    if (img->img == NULL) {
        printf("Unable to load image %s: %s\n", imagePath, IMG_GetError());
        return;
    }
    img->pos1.x = x;
    img->pos1.y = y;
    img->pos2.x = 0;
    img->pos2.y = 0;
    img->pos2.w = w;
    img->pos2.h = h;
}

/*void liberer(Image img) {
    if (img.img != NULL) {
        SDL_FreeSurface(img.img);
    }
}*/
void liberer0(Image *img) {
    if (img->img != NULL) {
        SDL_FreeSurface(img->img);
        img->img = NULL;
    }
}


void afficher0(Image img, SDL_Surface *screen) {
    SDL_BlitSurface(img.img, NULL, screen, &img.pos1);
}
void initText(Text *A)
{A->position.x=40;
A->position.y=70;
A->textColor.r=0;
A->textColor.g=0;
A->textColor.b=0;
A -> font = TTF_OpenFont("arial.ttf",20);}
void freeText(Text A )
{ SDL_FreeSurface (A.surfaceTexte);}

void displayText(Text t, SDL_Surface *screen) {
    SDL_Surface *textSurface;
    SDL_Rect textPosition = t.position; // Copie la position initiale
    int lineHeight = 24; // Hauteur approximative d'une ligne de texte

    // Diviser le texte en lignes
    const char *textLines[] = {
        "Dear players, welcome to TerraGradiens",
        " before you start playing let us tell you about our story...",
        "In 2024, due to a terrible virus caused by pollution", 
         "only six people stayed alive.",
        " They lived together in a safe place high up where the air is clean.",
        "They work hard to find a way to stop the virus.",
        "Together, they keep hope alive and show that even when almost everything is gone,",
        "people can still find a way to start again.",
        "Along their journey they will face multiple obstacles",
       " fight monsters and solve enigmas.",
        "Clic play and let the fun begins!"
    };

    // Afficher chaque ligne séparément
    for (size_t i = 0; i < sizeof(textLines) / sizeof(textLines[0]); i++) {

    //for (int i = 0; i < sizeof(textLines) / sizeof(textLines[0]); i++) {
        textSurface = TTF_RenderText_Solid(t.font, textLines[i], t.textColor);
        if (textSurface != NULL) {
            SDL_BlitSurface(textSurface, NULL, screen, &textPosition);
            SDL_FreeSurface(textSurface);
            
            textPosition.y += lineHeight; // Décaler pour la prochaine ligne
        }
    }
}
/*void initialiser_animation(Animation *animat)
{
int i;
char c[3],chemin[100]="";

//animat->anim[0]=IMG_Load("background/background0.png");

for(i=1;i<4;i++)
{
strcpy(chemin,"background/background");
sprintf(c,"%d",i);

strcat(chemin,c);
strcat(chemin,"0.png");

animat->anim[i-1]=IMG_Load(chemin);
}

// initialiser la position
animat->pos.x=0;
animat->pos.y=0;
}*/

void initialiser_animation(Animation *animat) {
    int i;
    char chemin[100];

    // Initialiser tous les éléments à NULL pour sécurité.
    for (i = 0; i < 20; i++) {
        animat->anim[i] = NULL;
    }

    // Charger les images pour les premières quatre positions (0 à 3).
    for (i = 0; i < 4; i++) {
        sprintf(chemin, "background/background%d0.png", i+1); // Assurez-vous que l'indexation et le nommage correspondent aux fichiers disponibles.
        animat->anim[i] = IMG_Load(chemin);
        if (animat->anim[i] == NULL) {
            fprintf(stderr, "Failed to load image: %s\n", chemin);
            // Gérer l'erreur, peut-être arrêter l'initialisation ou charger une image par défaut.
        }
    }

    // Initialiser la position
    animat->pos.x = 0;
    animat->pos.y = 0;
    animat->pos.w = 0; // Assurez-vous de définir la largeur et la hauteur si nécessaire.
    animat->pos.h = 0;
}




/*void afficher_animation( Animation animat,SDL_Surface *screen)
{
int i;
for(i=0;i<4;i++)
{
SDL_BlitSurface(animat.anim[i],NULL,screen,&animat.pos);
SDL_Delay(250);
SDL_Flip(screen);
}
}*/

void afficher_animation(Animation animat, SDL_Surface *screen) {
    int i;
    for(i = 0; i < 4; i++) {
        if (animat.anim[i] != NULL) {
            SDL_BlitSurface(animat.anim[i], NULL, screen, &animat.pos);
            SDL_Delay(250);
            SDL_Flip(screen);
        } else {
            fprintf(stderr, "Surface %d is NULL\n", i);
        }
    }
}



void playClickSound(int clickSoundEnabled,Mix_Chunk *clickSound) {
    if (clickSoundEnabled==0) {
        Mix_PlayChannel(-1, clickSound, 0);  // Assuming Mix_PlayChannel is used for playing sound
    }
}


void liberer_animation(Animation animat)
{
int i;
for(i=0;i<4;i++)
SDL_FreeSurface(animat.anim[i]);

}

// Function to change music volume
void changeMusicVolume(int volumeChange) {
    int currentVolume = Mix_VolumeMusic(-1); // Get current volume
    Mix_VolumeMusic(currentVolume + volumeChange);
}

// Function to change sound volume
void changeSoundVolume(Mix_Chunk *sound, int volumeChange) {
    int currentVolume = Mix_VolumeChunk(sound, -1); // Get current volume of the chunk
    Mix_VolumeChunk(sound, currentVolume + volumeChange);
}


