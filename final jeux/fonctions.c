#include <stdio.h>
#include <stdlib.h>
#include "fonction.h"

void initminimap(minimap *m)
{
    m->img_background = IMG_Load("images/lel.png");
    m->pos_background.x = 0;
    m->pos_background.y = 0;

    m->mask = IMG_Load("images/mask.png");
    m->pos_mask.x = 0;
    m->pos_mask.y = 0;

    m->img_minimap = IMG_Load("images/minimap.png");
    m->pos_minimap.x = 250;
    m->pos_minimap.y = 10;

    m->img_minimain = IMG_Load("images/bout.png");
    m->pos_minimain.x = 700;  // Set to 700
    m->pos_minimain.y = 100;  // Set to 100

    // Debug output
    printf("Minimain initialized at %d, %d\n", m->pos_minimain.x, m->pos_minimain.y);
}

void init_time(GameTime *timeingame)
{
    timeingame->pos_time.x = 1300;
    timeingame->pos_time.y = 15;
    timeingame->font = TTF_OpenFont("./font/Roboto.ttf", 40);
    if (timeingame->font == NULL) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
    }
}

void draw(SDL_Surface *screen, SDL_Surface *textSurface, char string[], int size, int x, int y, int fR, int fG, int fB, TTF_Font *font)
{    (void)size; // Mark as unused
    SDL_Color foregroundColor = {fR, fG, fB, 255};  // Fully initialize the color, including 'unused'
    // Function implementation
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    textSurface = TTF_RenderText_Solid(font, string, foregroundColor);
    if (textSurface == NULL)
    {
        return;
    }
    SDL_BlitSurface(textSurface, NULL, screen, &pos);
}
void afficherminimap(minimap m, SDL_Surface *screen)
{
    SDL_BlitSurface(m.img_background, NULL, screen, &m.pos_background);
    SDL_BlitSurface(m.img_minimap, NULL, screen, &m.pos_minimap);
    SDL_BlitSurface(m.img_minimain, NULL, screen, &m.pos_minimain);

    // Debug output
    printf("Minimain displayed at %d, %d\n", m.pos_minimain.x, m.pos_minimain.y);
}

void afficherTemps(GameTime *timeingame, SDL_Surface *screen)
{
    SDL_Surface *TimetextArea;
    char timeS[10];

    int seconds = SDL_GetTicks() / 1000;
    int minutes = seconds / 60;
    int remaining_seconds = seconds % 60;

    sprintf(timeS, "%d:%02d", minutes, remaining_seconds);

    draw(screen, TimetextArea, timeS, 50, timeingame->pos_time.x, timeingame->pos_time.y, 255, 255, 255, timeingame->font);
}


void MAJMinimap(SDL_Rect posJoueur, minimap *m, SDL_Rect camera, int redimensionnement)
{
    m->pos_minimain.x = (posJoueur.x + camera.x) * redimensionnement / 90;
    m->pos_minimain.y = (posJoueur.y + camera.y) * redimensionnement / 90;
}

SDL_Color GetPixel(SDL_Surface *pSurface, int x, int y)
{
    SDL_Color color;
    Uint32 col = 0;
    char *pPosition = (char *)pSurface->pixels;
    pPosition += (pSurface->pitch * y);
    pPosition += (pSurface->format->BytesPerPixel * x);
    memcpy(&col, pPosition, pSurface->format->BytesPerPixel);
    SDL_GetRGB(col, pSurface->format, &color.r, &color.g, &color.b);
    return (color);
}

int collisionPP(SDL_Rect p, SDL_Surface *Masque)
{

    SDL_Color colorOfObject;
    colorOfObject.r = 255;
    colorOfObject.g = 255;
    colorOfObject.b = 255;

    SDL_Rect Pos[8];
    Pos[0].x = p.x;
    Pos[0].y = p.y;
    Pos[1].x = p.x + (p.w / 2);
    Pos[1].y = p.y;
    Pos[2].x = p.x + p.w;
    Pos[2].y = p.y;
    Pos[3].x = p.x;
    Pos[3].y = p.y + (p.h / 2);
    Pos[4].x = p.x;
    Pos[4].y = p.y + p.h;
    Pos[5].x = p.x + (p.w / 2);
    Pos[5].y = p.y + p.h;
    Pos[6].x = p.x + p.w;
    Pos[6].y = p.y + p.h;
    Pos[7].x = p.x + p.w;
    Pos[7].y = p.y + (p.h / 2);

    int i = 0;
    while ((i <= 7))
    {

        SDL_Color colorDetected;
        colorDetected = GetPixel(Masque, Pos[i].x, Pos[i].y);

        if ((colorOfObject.r == colorDetected.r) && (colorOfObject.b == colorDetected.b) && (colorOfObject.g == colorDetected.g))
            return 1;
        else
            i++;
    }
    return 0;
}

void clean(GameTime *timeingame, minimap *m)
{
    if (timeingame->font != NULL)
    {
        TTF_CloseFont(timeingame->font);
    }

    if (m->img_minimap != NULL)
    {
        SDL_FreeSurface(m->img_minimap);
    }
    if (m->img_minimap != NULL)
    {
        SDL_FreeSurface(m->img_background);
    }
    if (m->img_minimain != NULL)
    {
        SDL_FreeSurface(m->img_minimain);
    }
}
