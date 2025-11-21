#include "perso.h"
#include <string.h>

void initPlayer(personne *A, int x, int y, const char *imagePath) {
    
    A->wKeyPressCount = 0; // Initialize counter
    strcpy(A->pressCountText, "0"); // Initialise avec "0"
    // S'assure que le reste du tableau est initialisé à zéro
    memset(A->pressCountText + 1, 0, sizeof(A->pressCountText) - 1);

    // Set up the frame height and width for animation
    A->frameHeight = 190; // The height of a single frame
    A->numFrames = 10; // Number of frames per animation
    A->currentFrame = 0;
    A->frameDelay = 5; // Change to desired delay
    A->frameCounter = 0;
    A->direction = 0;
    A->acceleration = 0;
    A->vie=3;
    
    A->vy = 0;
    A->saut = 1; // Commence n'est pas sauté
    A->doubleJump = 0;
    // Load the sprite sheet
    A->img = IMG_Load(imagePath);
    if (A->img == NULL) {
        printf("Unable to load image: %s\n", IMG_GetError());
        return;
    }
    
    // Initialize frame properties
    A->pos1.x = x; A->pos1.y = y;
    A->pos1.w = A->pos2.w = 190; // The width of a single frame
    A->pos1.h = A->pos2.h = A->frameHeight;
    A->pos2.x = 0; A->pos2.y = A->frameHeight * 2; // Start with the bottom row
}


void initlife(personne *A, int x) {
    

    // Set up the frame height and width for animation
    A->frameHeight = 90; // The height of a single frame
    A->numFrames = 8; // Assuming there are 3 frames per animation
    A->currentFrame = 0;
    A->frameDelay = 10; // Change to desired delay
    A->frameCounter = 0;
    A->vie = 0;

    

    // Load the sprite sheet
    A->img = IMG_Load("heart.png");
    if (A->img == NULL) {
        printf("Unable to load image: %s\n", IMG_GetError());
        return;
    }
    
    // Initialize frame properties
    A->pos1.x = x; A->pos1.y = 30;
    A->pos1.w = A->pos2.w = 90; // The width of a single frame
    A->pos1.h = A->pos2.h = A->frameHeight;
    A->pos2.x = 0; A->pos2.y = A->frameHeight * 2; // Start with the bottom row
}

void initshot(personne *A, int x, int y) {
    

    // Set up the frame height and width for animation
    A->frameHeight = 80; // The height of a single frame
    A->numFrames = 5; // Assuming there are 3 frames per animation
    A->currentFrame = 0;
    A->frameDelay = 10; // Change to desired delay
    A->frameCounter = 0;

    

    // Load the sprite sheet
    A->img = IMG_Load("spriteshot.png");
    if (A->img == NULL) {
        printf("Unable to load image: %s\n", IMG_GetError());
        return;
    }
    
    // Initialize frame properties
    A->pos1.x = x; A->pos1.y = y;
    A->pos1.w = A->pos2.w = 75; // The width of a single frame
    A->pos1.h = A->pos2.h = A->frameHeight;
    A->pos2.x = 0; A->pos2.y = A->frameHeight * 2; // Start with the bottom row
}

void initbackground(personne *backg){
    backg->img= IMG_Load("background.jpg");
    if (backg->img == NULL) {
        printf("Unable to load image: %s \n", IMG_GetError());
        return;
    }
    
    backg->pos1.x = 0;
    backg->pos1.y = 0;
    backg->pos2.x = 0;
    backg->pos2.y = 0;
    backg->pos2.w = SCREEN_W;
    backg->pos2.h = SCREEN_H;
}

void liberer(personne A) {
    if (A.img != NULL) {
        SDL_FreeSurface(A.img);
        A.img = NULL;
    }
}


void afficher(personne p, SDL_Surface *screen, TTF_Font *font, int multi, int joueurNum,SDL_Surface *image,SDL_Rect dest) {
    SDL_BlitSurface(p.img, &p.pos2, screen, &p.pos1); // Display the image
    SDL_BlitSurface(image, NULL, screen, &dest);
    SDL_Color textColor = {255, 255, 255, 0}; // Yellow color for the text

    SDL_Surface* textSurface;
    SDL_Rect textLocation;

    if (multi == 0) {
        // Single player mode or Player 1 in multiplayer
        textSurface = TTF_RenderText_Solid(font, p.pressCountText, textColor);
        textLocation.x = 1600;
        textLocation.y = 55;
    } else if (multi == 1 && joueurNum == 2) {
        // Player 2 in multiplayer
        textSurface = TTF_RenderText_Solid(font, p.pressCountText, textColor);
        textLocation.x = 1400;
        textLocation.y = 100;
    } else if (multi == 1 && joueurNum == 1) {
        // Player 2 in multiplayer
        textSurface = TTF_RenderText_Solid(font, p.pressCountText, textColor);
        textLocation.x = 700;
        textLocation.y = 100;
    }

    if (textSurface != NULL) {
        SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
        SDL_FreeSurface(textSurface);
    }
}


void afficherlife(personne p, SDL_Surface *screen) {
    SDL_BlitSurface(p.img, &p.pos2, screen, &p.pos1); // Existing rendering code
}

void affichershot(personne p, SDL_Surface *screen) {
    SDL_BlitSurface(p.img, &p.pos2, screen, &p.pos1); // Existing rendering code
}

void moveImage(personne *img, int dx, int dy) {
    img->pos1.x += dx;
    img->pos1.y += dy;
}

void moveshot(personne *img, int dx) {
    img->pos1.x += dx;
}

void animerPlayer(personne *player, int direction) {
    int frameRow; // Variable to determine the row based on action

    if (direction == 1) {
        frameRow = 1; // deplacement droit
    } else if (direction == 2) {
        frameRow = 2; // deplacement gauche
    } else if (direction == 3) {
        frameRow = 4; // saut
    } else if (direction == 4) {
        frameRow = 3; // back
    } else if (direction == 5) {
        frameRow = 5; // back
    } else if (direction == 6) {
        frameRow = 6; // back
    } else if (direction == 7) {
        frameRow = 7; // back
    } else if (direction == 8) {
        frameRow = 8; // back
    } else if (direction == 9) {
        frameRow = 9; // back
    } else if (direction == 18) {
        frameRow = 18; // back
    }  else if (direction == 13) {
        frameRow = 13; // back
    }  else if (direction == 14) {
        frameRow = 14; // back
    }    else if (direction == 21) {
        frameRow = 21; // back
    }    else if (direction == 16) {
        frameRow = 16; // back
    }   else if (direction == 17) {
        frameRow = 17; // back
    } else if (direction == 19) {
        frameRow = 19; // back
    } else if (direction == 20) {
        frameRow = 20; // back
    }else {
        // If action does not match, default to idle
        frameRow = 0; // Adjust this based on the actual layout of your sprite sheet
    }
    
    // Correctly calculate Y position of the sprite sheet row
    player->pos2.y = frameRow * player->frameHeight;

    player->frameCounter++;
    if (player->frameCounter >= player->frameDelay) {
        player->frameCounter = 0;
        if ( frameRow == 6 || frameRow == 7 || frameRow == 16 || frameRow == 17 || frameRow == 9 || frameRow == 10 || frameRow == 13) {
            if (player->currentFrame < player->numFrames - 1) {
                player->currentFrame++;
            }
            // Si vous avez besoin de réinitialiser l'animation pour l'état 1, faites-le ici
        }else {
            player->currentFrame = (player->currentFrame + 1) % player->numFrames;
        } 

        // Update the pos2 x position to display the current frame
        player->pos2.x = player->currentFrame * player->pos2.w;
    }
}

void animerlife(personne *player, int etat) {
    int frameRow; // Variable to determine the row based on action

    if (etat == 0) {
        frameRow = 0; // etat initiale
    } else if (etat == 1) {
        frameRow = 1; // deplacement droit
    } else if (etat == 2) {
        frameRow = 2; // deplacement gauche
    } else {
        frameRow = 0; // Default to idle
    }
    
    // Correctly calculate Y position of the sprite sheet row
    player->pos2.y = frameRow * player->frameHeight;

    player->frameCounter++;
    if (player->frameCounter >= player->frameDelay) {
        player->frameCounter = 0;
        if (frameRow == 0 || frameRow == 1) {
            player->currentFrame = (player->currentFrame + 1) % player->numFrames;
        } else if (frameRow == 2) {
            if (player->currentFrame < player->numFrames - 1) {
                player->currentFrame++;
            }
            // Si vous avez besoin de réinitialiser l'animation pour l'état 1, faites-le ici
        }

        // Update the pos2 x position to display the current frame
        player->pos2.x = player->currentFrame * player->pos2.w;
    }
}

void animershot(personne *player) {
    int frameRow = 0; // etat initiale
    
    // Correctly calculate Y position of the sprite sheet row
    player->pos2.y = frameRow * player->frameHeight;

    player->frameCounter++;
    if (player->frameCounter >= player->frameDelay) {
        player->frameCounter = 0;
            player->currentFrame = (player->currentFrame + 1) % player->numFrames;

        // Update the pos2 x position to display the current frame
        player->pos2.x = player->currentFrame * player->pos2.w;
    }
}


