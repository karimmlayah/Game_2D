#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

typedef struct{
	SDL_Rect pos_img_ecran;
	SDL_Rect pos_img_affiche;
	SDL_Surface *img;
}image;
typedef struct
{
	SDL_Surface *txt;
	SDL_Rect pos_txt;
	SDL_Colour color_txt;
	TTF_Font *police;
}texte;
typedef struct{
	image back,bouton1,bouton2,bouton3,over,clock,win;
	texte t,t1,t2,t3;
	char ch[100],ch1[20],ch2[20],ch3[20];
	int reponsev,pos,quit,reponsech,pos2;
}enigme;
void initialiser_enigme(enigme *e,char *nomfichier);
void afficher_enigme(SDL_Surface *screen,enigme e);
void liberer_enigme(enigme e);
void animer_enigme(enigme *e);
#endif
