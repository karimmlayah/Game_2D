#include <math.h>
#include "enemies.h"
#include <stdbool.h>
void animer(entity *e , int etat) {
    int frameRow; 

   
    if (etat == 0) {
        frameRow = 2; 
    } else if (etat == 1) {
        frameRow = 3; 
    }

    
    e->posSprite.y = (frameRow - 1) * e->frameHeight;

    e->frameCounter++;
    if (e->frameCounter >= e->frameDelay) {
        e->frameCounter = 0;
        e->currentFrame = (e->currentFrame + 1) % e->numFrames;

        
        e->posSprite.x = e->currentFrame * e->posSprite.w;
    }
}


void move(entity *e)
{
   
    if (e->direction == 0) {
        animer(e, e->direction); 
        if (e->posScreen.x < 650) {
            e->posScreen.x = e->posScreen.x + 2;
        } else {
            e->direction = 1; 
        }
    } else {
        animer(e, e->direction); 
        if (e->posScreen.x > 450) {
            e->posScreen.x = e->posScreen.x - 2;
        } else {
            e->direction = 0; 
        }
    }
}


void move2(entity *e) {
    // Vérifiez la direction verticale actuelle et déplacez-vous en conséquence
    if (e->vert_direction == 0) {
        // Descendre
        if (e->posScreen.y < 850) {
            e->posScreen.y += 2; // Déplacer vers le bas
        } else {
            e->vert_direction = 1; // Atteindre le bas, changer de direction vers le haut
        }
    } else {
        // Monter
        if (e->posScreen.y > 700) {
            e->posScreen.y -= 2; // Déplacer vers le haut
        } else {
            e->vert_direction = 0; // Atteindre le haut, changer de direction vers le bas
        }
    }

    // Éventuellement, mettez à jour l'animation en fonction du mouvement vertical
    animer(e,e->vert_direction); // Assurez-vous que cette fonction existe
}

	



void initEntity(entity *e, const char *spritePath, int posX, int posY) {
    e->spriteSheet = IMG_Load(spritePath);
    if (!e->spriteSheet) {
        printf("Échec du chargement de la feuille de sprite : %s\n", IMG_GetError());
        return;
    }


    e->isActive = true;

    // Position initiale de l'entité
    e->posScreen.x = posX;
    e->posScreen.y = posY;
    e->posScreen.w = 64;
    e->posScreen.h = 64;

    // Position de la première frame dans la feuille de sprite
    e->posSprite.x = 0;
    e->posSprite.y = 0;
    e->posSprite.w = 64;
    e->posSprite.h = 64;

    e->direction = 0;
    e->vitesse = 0;
    e->frameHeight = 64;
    e->currentFrame = 0;
    e->frameCounter = 0;
    e->numFrames = 4;
    e->frameDelay = 5;
}

/*void afficherEnnemi(entity *e, SDL_Surface *screen) {
    if (e == NULL || screen == NULL || e->spriteSheet == NULL ||  e->isActive == false) {
        printf("Invalid pointers in afficherEnnemi\n");
        return;
    }


    if (SDL_BlitSurface(e->spriteSheet, &e->posSprite, screen, &e->posScreen) < 0) {
        printf("SDL_BlitSurface error: %s\n", SDL_GetError());
    }
}*/
void afficherEnnemi(entity *e, SDL_Surface *screen) {
    if (e == NULL || screen == NULL || e->spriteSheet == NULL || !e->isActive) {
        return;  // Ne rien faire si l'ennemi n'est pas actif
    }
    if (SDL_BlitSurface(e->spriteSheet, &e->posSprite, screen, &e->posScreen) < 0) {
        printf("SDL_BlitSurface error: %s\n", SDL_GetError());
    }
}


int collisionTri(entity *e, SDL_Rect pose){
    int x1,x2,y1,y2;
    float rayon1, rayon2, distance;
    // Calculer le centre et le rayon du cercle circonscrit pour la première entité
    x1 = e->posScreen.x + e->posScreen.w / 2;
    y1 = e->posScreen.y + e->posScreen.h / 2;
    rayon1 = sqrt((e->posScreen.w / 2) * (e->posScreen.w / 2) + (e->posScreen.h / 2) * (e->posScreen.h / 2));
    
    // Calculer le centre et le rayon du cercle circonscrit pour le deuxième objet
    x2 = pose.x + pose.w / 2;
    y2 = pose.y + pose.h / 2;
    rayon2 = sqrt((pose.w / 2) * (pose.w / 2) +(pose.h / 2) * (pose.h / 2));
    
    // Calculer la distance entre les centres des deux cercles
    distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    
    
    if (distance <= (rayon1 + rayon2)) {
        return 1;
    } else {
        return 0; 
    }
}

void initCoin(Coin *c, const char *filePath, int x, int y) {
    c->spriteSheet = IMG_Load(filePath);
    if (!c->spriteSheet) {
        printf("Failed to load coin image: %s\n", IMG_GetError());
     
    }

    c->pos.x = x;
    c->pos.y = y;
    c->frameWidth = c->spriteSheet->w / 6; 
    c->frameHeight = c->spriteSheet->h;
    c->numFrames = 6;
    c->currentFrame = 0;
    c->coinDelay =10;
}

void initClock(Coin *c, const char *filePath, int x, int y) {
    c->spriteSheet = IMG_Load(filePath);
    if (!c->spriteSheet) {
        printf("Failed to load coin image: %s\n", IMG_GetError());
     
    }

    c->pos.x = x;
    c->pos.y = y;
    c->frameWidth = c->spriteSheet->w / 7; 
    c->frameHeight = c->spriteSheet->h;
    c->numFrames = 7;
    c->currentFrame = 0;
    c->coinDelay =10;
}

void animateCoin(Coin *c) {
    c->currentFrame++;
    if (c->currentFrame >= c->numFrames) {
        c->currentFrame = 0;
    }
}


/*void showCoin(Coin *c, SDL_Surface *screen) {
    SDL_Rect srcRect = {c->currentFrame * c->frameWidth,0,c->frameWidth,c->frameHeight};
    SDL_BlitSurface(c->spriteSheet, &srcRect, screen, &c->pos);
}*/

// Fonction liberer
/*void liberer(entity *entity) {
    if (entity->spriteSheet) {
        SDL_FreeSurface(entity->spriteSheet);
        entity->spriteSheet = NULL;
    }
}*/
/*void move(entity *e) {
    // Animation de l'entité en fonction de la direction
    animer(e, e->direction); 
    // Mouvement horizontal avec changement de direction aux bornes
    if (e->direction == 0) {
        if (e->posScreen.x < 530) {
            e->posScreen.x += 2;
        } else {
            e->direction = 1; 
        }
    } else {
        if (e->posScreen.x > 430) {
            e->posScreen.x -= 2;
        } else {
            e->direction = 0;
        }
    }
}*/



void showCoin(Coin *c, SDL_Surface *screen) {
    SDL_Rect srcRect = {c->currentFrame * c->frameWidth, 0, c->frameWidth, c->frameHeight};
    SDL_BlitSurface(c->spriteSheet, &srcRect, screen, &c->pos);
}

