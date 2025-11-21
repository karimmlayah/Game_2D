#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "enigme.h"
#include "time.h"


void initialiser_enigme(enigme *e,char *nomfichier)
{
	int i=0,q;
	FILE* f;
	srand(time(NULL));
	q=rand()%6;  //nombre de question
	f=fopen(nomfichier,"r");
	if(f==NULL)
		printf("Erreur d ouvrir fichier d enigme\n");
	else
	{
		while(i<=q)
		{
			fgets(e->ch,100,f);
			fscanf(f,"%s %s %s %d\n",e->ch1,e->ch2,e->ch3,&e->reponsev);
			i++;
		}
		fclose(f);
	}
	
		e->back.img=IMG_Load("backe.png");
		e->back.pos_img_ecran.x=0;
		e->back.pos_img_ecran.y=0;
		e->bouton1.img=IMG_Load("bouton.png");
		e->bouton2.img=IMG_Load("bouton.png");
		e->bouton3.img=IMG_Load("bouton.png");
		e->bouton1.pos_img_ecran.x=61;
		e->bouton1.pos_img_ecran.y=300;
		e->bouton2.pos_img_ecran.x=61;
		e->bouton2.pos_img_ecran.y=454;
		e->bouton3.pos_img_ecran.x=61;
		e->bouton3.pos_img_ecran.y=608;
		e->over.img=IMG_Load("over.png");
		e->over.pos_img_ecran.x=0;
		e->over.pos_img_ecran.y=0;
		e->win.img=IMG_Load("win.png");
		e->win.pos_img_ecran.x=0;
		e->win.pos_img_ecran.y=0;
		e->clock.img=IMG_Load("clock.png");
		e->clock.pos_img_ecran.x=380;
		e->clock.pos_img_ecran.y=180;
		e->clock.pos_img_affiche.x=0;
		e->clock.pos_img_affiche.y=0;
		e->clock.pos_img_affiche.w=57;
		e->clock.pos_img_affiche.h=81;
	e->t.police=TTF_OpenFont("Ubuntu-B.ttf",30);
	e->t.color_txt.r=255;
	e->t.color_txt.g=255;
	e->t.color_txt.b=255;
	e->t.txt=TTF_RenderText_Blended(e->t.police,e->ch,e->t.color_txt);
	e->t.pos_txt.x=600;
	e->t.pos_txt.y=90;
	e->t1.police=TTF_OpenFont("Ubuntu-B.ttf",20);
	e->t1.color_txt.r=255;
	e->t1.color_txt.g=255;
	e->t1.color_txt.b=255;
	e->t1.txt=TTF_RenderText_Blended(e->t1.police,e->ch1,e->t1.color_txt);
	e->t1.pos_txt.x=401-e->t1.txt->w/2;
	e->t1.pos_txt.y=335;
	e->t2.police=TTF_OpenFont("Ubuntu-B.ttf",20);
	e->t2.color_txt.r=255;
	e->t2.color_txt.g=255;
	e->t2.color_txt.b=255;
	e->t2.txt=TTF_RenderText_Blended(e->t2.police,e->ch2,e->t2.color_txt);
	e->t2.pos_txt.x=401-e->t2.txt->w/2;
	e->t2.pos_txt.y=489;
	e->t3.police=TTF_OpenFont("Ubuntu-B.ttf",20);
	e->t3.color_txt.r=255;
	e->t3.color_txt.g=255;
	e->t3.color_txt.b=255;
	e->t3.txt=TTF_RenderText_Blended(e->t3.police,e->ch3,e->t3.color_txt);
	e->t3.pos_txt.x=401-e->t3.txt->w/2;
	e->t3.pos_txt.y=643;
	e->pos=1;
	e->pos2=6;
	e->reponsech=0;
	e->quit=0;
}

void afficher_enigme(SDL_Surface *screen,enigme e)
{
	SDL_BlitSurface(e.back.img,NULL, screen, &e.back.pos_img_ecran);
	SDL_BlitSurface(e.clock.img,&e.clock.pos_img_affiche, screen, &e.clock.pos_img_ecran);
	if(e.pos==1)
		SDL_BlitSurface(e.bouton1.img,NULL, screen, &e.bouton1.pos_img_ecran);
	else if(e.pos==2)
		SDL_BlitSurface(e.bouton1.img,NULL, screen, &e.bouton2.pos_img_ecran);
	else if(e.pos==3)
		SDL_BlitSurface(e.bouton1.img,NULL, screen, &e.bouton3.pos_img_ecran);
	SDL_BlitSurface(e.t.txt,NULL,screen,&e.t.pos_txt);
	SDL_BlitSurface(e.t1.txt,NULL,screen,&e.t1.pos_txt);
	SDL_BlitSurface(e.t2.txt,NULL,screen,&e.t2.pos_txt);
	SDL_BlitSurface(e.t3.txt,NULL,screen,&e.t3.pos_txt);
	if(e.reponsech!=0)
	{
		if(e.reponsev==e.reponsech)
			SDL_BlitSurface(e.win.img,NULL, screen, &e.win.pos_img_ecran);
		else
			SDL_BlitSurface(e.over.img,NULL, screen, &e.over.pos_img_ecran);
	}
}

void liberer_enigme(enigme e)
{
	SDL_FreeSurface(e.bouton1.img);
	SDL_FreeSurface(e.bouton2.img);
	SDL_FreeSurface(e.bouton3.img);
	SDL_FreeSurface(e.over.img);
	SDL_FreeSurface(e.clock.img);
	SDL_FreeSurface(e.win.img);
	SDL_FreeSurface(e.back.img);
	TTF_CloseFont(e.t.police);
	TTF_CloseFont(e.t1.police);
	TTF_CloseFont(e.t2.police);
	TTF_CloseFont(e.t3.police);
}

void animer_enigme(enigme *e)
{
	if(e->pos2==6)
		e->pos2=0;
	else
		e->pos2+=1;
	e->clock.pos_img_affiche.x=e->pos2*e->clock.pos_img_affiche.w;
}


