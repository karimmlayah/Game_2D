#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "menu.h" // Include the header file where liberer is declared
#include "perso.h"
#include "background.h"
#include "fonction.h"
#include "enemies.h"
#include "enigme.h"
#include "tik-tak.h"
#include <string.h>
#define MAX_SHOTS 100  // Maximum number of shots active at once

#define SCREEN_W 1792
#define SCREEN_H 1024
#define FPS 60


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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {

    SDL_Init(SDL_INIT_EVERYTHING);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
	        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	        return -1;
        }
	atexit(SDL_Quit);

        if (TTF_Init() == -1) 
	{
	        fprintf(stderr, "TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
	        return -1;
        }

    	/*SDL_Surface* screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    	if (!screen) 
    	{
    	    fprintf(stderr, "Screen could not be set! SDL_Error: %s\n", SDL_GetError());
    	    SDL_Quit();
    	    return -1;
    	}
*/
    	SDL_Surface* screen1 = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    	if (!screen1) 
    	{
    	    fprintf(stderr, "Screen could not be set! SDL_Error: %s\n", SDL_GetError());
    	    SDL_Quit();
    	    return -1;
    	}


    	TTF_Font* font = TTF_OpenFont("game.ttf", 60); // Assurez-vous que ce chemin est correct
    	if (!font) 
	{
    	    fprintf(stderr, "Failed to load font! TTF_Error: %s\n", TTF_GetError());
    	    SDL_Quit();
    	    return -1;
    	}

    SDL_Rect firePos;

    firePos.x = 1500;
    firePos.y = 10;
    SDL_Rect camera;
    camera.x = 0;
    camera.y = 0;

    minimap m;

GameTime timeingame;
init_time(&timeingame);

int xo=0;


    TTF_Font *font1 = NULL;
    font1 = TTF_OpenFont("./font/Roboto.ttf", 100);
    if (font1 == NULL)
        return -1;

    initminimap(&m);

personne shots[MAX_SHOTS];
int shot_count = 0;

    	personne player,player2,life,life2,life3,life5,life6,life7;
    	//initPlayer(&player,100,700,"spritesheet1.png");
    	initPlayer(&player2,1000,700,"spritesheet1.png");
    	initlife(&life, 20);
    	initlife(&life2, 90);
    	initlife(&life3, 160);
    	initlife(&life5, 1000);
    	initlife(&life6, 1070);
    	initlife(&life7, 1140);

    background bg;
    init_background(&bg);
    int test_scrolling_end_right = 0,test_scrolling_end_left = 0;

       // Movement flags
    	int moveLeft = 0;
	int moveRight = 0;
    	int moveback=0;
    	int moveLeft2 = 0;
	int moveRight2 = 0;
    	int moveback2 =0;

    entity myEntity, myEntity2; 
    Coin coin, coin1, coin2, coin4, coin5, coin3,clock; 

    initClock(&clock, "clock.png", 1400, 10); 
    initCoin(&coin, "coin.png", 1150, 650); 
    initCoin(&coin1, "coin.png", 1250, 650);
    initCoin(&coin2, "coin.png", 1350, 650); 
    initCoin(&coin3, "coin.png", 950, 800);
    initCoin(&coin4, "coin.png", 850, 800);
    initCoin(&coin5, "coin.png", 750, 800);
    initEntity(&myEntity, "sprite.png", 450, 850); 
    initEntity(&myEntity2, "sprite.png", 1420,850); 
	
   	int input=-1;
    enigme e;
    initialiser_enigme(&e, "enigme.txt");
	int eni=0;

	//mode multiplayer
    	int multi=0;

    	int done1 = 0;

    mini_game game;
    init_mini_game(&game);
    SDL_WM_SetCaption("Tik-Tak", NULL);
    	
	SDL_Event event1;

	SDL_Surface *image;

    image = IMG_Load("tabcoin.png");
    if (!image) {
        fprintf(stderr, "Could not load image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Rect dest;
    dest.x = 1500;
    dest.y = 50;
    dest.w = image->w;
    dest.h = image->h;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    TTF_Init();
    SDL_Surface *screen;
    Mix_Music *music;
    Animation anim;
    atexit(SDL_Quit);
    Text t;
    screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        printf("Screen could not be set! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Image background1, button1,button21, button2,button3,button31,buttonsc,buttonsc1,buttoncr,buttoncr1,button4,button41,backskin;
    Image
background2,buttonsound,buttonfls,buttonns,buttonrt,buttonmt,buttonmt1,buttonmusic,buttonup,buttonqz,buttonsoundmute,buttonsoundmute1,checkd,checkg,musicp,musicm,soundp,soundm,volume100,volume80,volume60,volume40,volume20,volume0,sound100,sound80,sound60,sound40,sound20,sound0;
    Image background3,background4,buttonabt,buttonabt1,volume30,sound30;
    initImage(&background1, "backg.jpg", 0, 0, SCREEN_W, SCREEN_H);
    initImage(&background2, "backg2.jpg", 0, 0, SCREEN_W, SCREEN_H);
    initImage(&background3, "backg3.jpg", 0, 0, SCREEN_W, SCREEN_H);
    initImage(&background4, "backg4about.jpg", 0, 0, SCREEN_W, SCREEN_H);
    initImage(&button1, "name.png", 0, 40, 1500, 850);
    initImage(&button2, "play.png", 0, 40, 200, 250);
    initImage(&button21, "play2.png", 0, 40, 200, 250);
    initImage(&button3, "setting.png", -30, 10, 200, 350);
    initImage(&button31, "setting2.png", -30, 10, 200, 350);
    initImage(&button4, "exit.png", 30, 10, 200, 350);
    initImage(&button41, "exit2.png", 30, 10, 200, 350);
       initImage(&buttonsc, "score.png", 0, 20, 200, 350);
       initImage(&buttonsc1, "score2.png", 0, 20, 200, 350);
       initImage(&buttoncr, "credit.png", -10, 20, 200, 350);
       initImage(&buttoncr1, "credit2.png", -10, 20, 200, 350);
//set2
    initImage(&buttonmusic, "music.png", 0, 0, 350, 1000);
    initImage(&buttonsound, "sound.png", 0, 0, 350, 1000);
    initImage(&buttonfls, "fullscreen.png", 0, 0, 350, 1000);
    initImage(&buttonns, "normalscreen.png", 0, 0, 350, 1000);
    initImage(&buttonrt, "return.png", 0, -15, 350, 1000);
    initImage(&buttonmt, "mute.png", 0, 0, 350, 1000);
    initImage(&buttonmt1, "mute1.png", 0, 0, 350, 1000);
    initImage(&buttonup, "up.png", 0, 0, 350, 1000);
    initImage(&buttonqz, "qz.png", 0, 0, 350, 1000);
    initImage(&buttonsoundmute, "soundmute.png", 0, 0, 350, 1000);
    initImage(&buttonsoundmute1, "soundmute1.png", 0, 0, 350, 1000);
    initImage(&checkd, "checkd.png", 0, 0, 350, 1000);
    initImage(&checkg, "checkg.png", 0, 0, 350, 1000);
    initImage(&musicp, "musicp.png", 0, 0, 350, 1000);
    initImage(&musicm, "musicm.png", 0, 0, 350, 1000);
    initImage(&soundp, "soundp.png", 0, 0, 350, 1000);
    initImage(&soundm, "soundm.png", 0, 0, 350, 1000);

    initImage(&volume100, "volume100.png", 0, 0, 350, 1000);
    initImage(&volume80, "volume80.png", 0, 0, 350, 1000);
    initImage(&volume60, "volume60.png", 0, 0, 350, 1000);
    initImage(&volume40, "volume40.png", 0, 0, 350, 1000);
    initImage(&volume30, "volume30.png", 0, 0, 350, 1000);
    initImage(&volume20, "volume20.png", 0, 0, 350, 1000);
    initImage(&volume0, "volume0.png", 0, 0, 350, 1000);

    initImage(&sound100, "sound100.png", 0, 0, 350, 1000);
    initImage(&sound80, "sound80.png", 0, 0, 350, 1000);
    initImage(&sound60, "sound60.png", 0, 0, 350, 1000);
    initImage(&sound40, "sound40.png", 0, 0, 350, 1000);
    initImage(&sound30, "sound30.png", 0, 0, 350, 1000);
    initImage(&sound20, "sound20.png", 0, 0, 350, 1000);
    initImage(&sound0, "sound0.png", 0, 0, 350, 1000);
    initImage(&backskin, "backskin.png", 0, 0, 350, 1000);

    //initImage(&buttonabt1, "returnabout1.png", 130, 0, 350, 1000);
    //initImage(&buttonabt, "returnabout.png", 325, 140, 350, 1000);


	int skin;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }


    // Initialize SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }
    music = Mix_LoadMUS("song.mp3");
    if (!music) {
        printf("Error loading music: %s\n", Mix_GetError());
        return 1;
    }

    Mix_PlayMusic(music, -1);

    SDL_Event event;
    int done = 0;//quit
    int set=1;//background
    int bts=0,bts2=0,bts4=0;//button survoler
    int mute=0;//son
    int soundpaused=0;
    initialiser_animation(&anim);

    int control=0;
    int bts8;

    initText(&t);

    Mix_Chunk *clickSound = NULL; // Sound effect variable
    clickSound = Mix_LoadWAV("effet.wav");
    if (!clickSound) {
        printf("Error loading click sound effect: %s\n", Mix_GetError());
    }

    // Get and print the current music volume
    int musicVolume = Mix_VolumeMusic(-1); // Get current volume

    // Get and print the current sound volume
    int soundVolume = Mix_VolumeChunk(clickSound, -1); // Get current volume of the chunk


    while (!done) {
        while (SDL_PollEvent(&event1)) {
            switch (event1.type) {
                case SDL_QUIT:
                    done = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (set){
			case 1:{
                if (event1.key.keysym.sym == SDLK_ESCAPE)
                        {done = 1;playClickSound(soundpaused,clickSound);}
                    else if (event1.key.keysym.sym == SDLK_p) 
			{set=2;playClickSound(soundpaused,clickSound);}
                    else if (event1.key.keysym.sym == SDLK_SPACE)
			{set=8;playClickSound(soundpaused,clickSound);}
                    else if (event1.key.keysym.sym == SDLK_c)
			{set=4;playClickSound(soundpaused,clickSound);}
                    else if (event1.key.keysym.sym == SDLK_b)
			{set=5;playClickSound(soundpaused,clickSound);}
		break;}
case 2:{
    if (event1.key.keysym.sym == SDLK_s) {
	playClickSound(soundpaused,clickSound);
        soundpaused = !soundpaused; // Toggle soundPaused state without playing any sound
    } // Ensure sound effects only play when sound is not paused
        switch(event1.key.keysym.sym) {
		case SDLK_UP:
                playClickSound(soundpaused,clickSound);
                control = 0;
                break;
		case SDLK_q:
                playClickSound(soundpaused,clickSound);
                control = 1;
                break;
            case SDLK_m:
                playClickSound(soundpaused,clickSound);
                mute = !mute;
                if (!mute) {
                    Mix_ResumeMusic();
                } else {
                    Mix_PauseMusic();
                }
                break;
            case SDLK_r:
                playClickSound(soundpaused,clickSound);
                set = 1;
                break;
            case SDLK_f:
                playClickSound(soundpaused,clickSound);
                // Switch to full screen
                screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
                break;
            case SDLK_n:
                playClickSound(soundpaused,clickSound);
                // Switch to windowed mode
                screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
                break;
                default:
                    break;
        }
    break;
}
		case 4:{if (event1.key.keysym.sym == SDLK_r){playClickSound(soundpaused,clickSound);set=1;}
}//case4
			case 8:{if(event1.key.keysym.sym == SDLK_r){set=1;playClickSound(soundpaused,clickSound);}//skin
				if(event1.key.keysym.sym == SDLK_w){skin=2;playClickSound(soundpaused,clickSound);}
				if(event1.key.keysym.sym == SDLK_e){skin=1;playClickSound(soundpaused,clickSound);}
				if(event1.key.keysym.sym == SDLK_f){skin=3;playClickSound(soundpaused,clickSound);}
			break;}//case 4
}//switch
                case SDL_MOUSEBUTTONDOWN:
			switch (set){
			case 1:{
				if(event1.button.button == SDL_BUTTON_LEFT){	
				 if (event1.motion.x>820 && event1.motion.x<960 && event1.motion.y<605 && event1.motion.y>470){set=8;playClickSound(soundpaused,clickSound);}//bouton play
				 if (event1.button.x>940 && event1.button.x<1115 && event1.button.y<675 && event1.button.y>590){set=2;playClickSound(soundpaused,clickSound);}// button settings
				if (event1.button.x>665 && event1.button.x<845 && event1.button.y<675 && event1.button.y>590){done=1;playClickSound(soundpaused,clickSound);}//button exit
				if (event1.button.x>805 && event1.button.x<980 && event1.button.y<747 && event1.button.y>665){set=4;playClickSound(soundpaused,clickSound);}//button credit
				if (event1.button.x>795 && event1.button.x<990 && event1.button.y<840 && event1.button.y>760){set=2;playClickSound(soundpaused,clickSound);}//button score
}//case1
	 	break;}
		case 2:{
				if(event1.button.button == SDL_BUTTON_LEFT){
				 if (event1.button.x>635 && event1.button.x<695 && event1.button.y<346 && event1.button.y>286){musicVolume = Mix_VolumeMusic(-1);changeMusicVolume(20);playClickSound(soundpaused,clickSound);}//bouton +music
				 if (event1.button.x>189 && event1.button.x<249 && event1.button.y<347 && event1.button.y>287){musicVolume = Mix_VolumeMusic(-1);changeMusicVolume(-20);playClickSound(soundpaused,clickSound);}//bouton -music
			if (event1.button.x>1693 && event1.button.x<1753 && event1.button.y<347 && event1.button.y>287){soundVolume = Mix_VolumeChunk(clickSound, -1);changeSoundVolume(clickSound, 20);playClickSound(soundpaused,clickSound);}//bouton +sound
			if (event1.button.x>1248 && event1.button.x<1308 && event1.button.y<347 && event1.button.y>287){soundVolume = Mix_VolumeChunk(clickSound, -1);changeSoundVolume(clickSound, -20);playClickSound(soundpaused,clickSound);}//bouton -sound
			  		if(event1.button.x>396 && event1.button.x<489 && event1.button.y<277 && event1.button.y>203) {
					playClickSound(soundpaused,clickSound);
					mute=!mute;
					if(!mute)
					{Mix_ResumeMusic();}
					else{Mix_PauseMusic();}}//boutton music
				if (event1.button.x>1452 && event1.button.x<1552 && event1.button.y<278 && event1.button.y>199){soundpaused = !soundpaused;playClickSound(soundpaused,clickSound);}//sound
				if (event1.button.x>1078 && event1.button.x<1577 && event1.button.y<957 && event1.button.y>594){control=0;playClickSound(soundpaused,clickSound);}//updown
				if (event1.button.x>216 && event1.button.x<703 && event1.button.y<948 && event1.button.y>586){control=1;playClickSound(soundpaused,clickSound);}//qzsd
				if (event1.button.x>789 && event1.button.x<1005 && event1.button.y<999 && event1.button.y>896){set=1;playClickSound(soundpaused,clickSound);}//return
				if (event1.button.x>614 && event1.button.x<802 && event1.button.y<482 && event1.button.y>364){
        // Basculer en mode plein écran
	playClickSound(soundpaused,clickSound);
        screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN); } //button full
				if (event1.button.x>983 && event1.button.x<1167 && event1.button.y<482 && event1.button.y>362){
        // Basculer en mode fenêtré
	playClickSound(soundpaused,clickSound);
        screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);}//button normal
}// if

break;}// case2
		case 4:{if(event1.button.button == SDL_BUTTON_LEFT){
			if (event1.button.x>300 && event1.button.x<400 && event1.button.y<410 && event1.button.y>360){set=1;playClickSound(soundpaused,clickSound);}
}//if
break;}//case 4
		case 8:{if(event1.button.button == SDL_BUTTON_LEFT){
			if (event1.button.x>1122 && event1.button.x<1393 && event1.button.y<819 && event1.button.y>310){bts8=1;playClickSound(soundpaused,clickSound);}
			if (event1.button.x>393 && event1.button.x<664 && event1.button.y<834 && event1.button.y>325){bts8=2;playClickSound(soundpaused,clickSound);}
			if (event1.button.x>713 && event1.button.x<1080 && event1.button.y<817 && event1.button.y>208){bts8=3;playClickSound(soundpaused,clickSound);}
}//if
break;}//case 4
}//switch
	case SDL_MOUSEMOTION:
    	switch (set){
	case 1:{
     if (event1.motion.x>820 && event1.motion.x<960 && event1.motion.y<605 && event1.motion.y>470) {
        bts = 1; // Souris sur le bouton play
    } else if (event1.motion.x>940 && event1.motion.x<1115 && event1.motion.y<675 && event1.motion.y>590) {
        bts = 2; // Souris sur le bouton setting
    } else if (event1.motion.x>665 && event1.motion.x<845 && event1.motion.y<675 && event1.motion.y>590) {
        bts = 3; // Souris sur le bouton exit
    } else if (event1.motion.x>805 && event1.motion.x<980 && event1.motion.y<747 && event1.motion.y>665) {
        bts = 4; // Souris sur le bouton credit
    } else if (event1.motion.x>795 && event1.motion.x<990 && event1.motion.y<840 && event1.motion.y>760) {
        bts = 5; // Souris sur le bouton score
    } 
       else if (event1.motion.x > 0 && event1.motion.x < 1792 && event1.motion.y > 0 && event1.motion.y < 1024) {
        bts = 6; // Souris sur tous l'ecran sauf les bouttun
    } 
	    else {
        bts = 0; // Souris n'est sur aucun bouton
    } break; }  
	case 2:{
	if (event1.motion.x>396 && event1.motion.x<489 && event1.motion.y<277 && event1.motion.y>203) {
        bts2 = 1; // Souris sur le bouton music
    }   else if (event1.motion.x>635 && event1.motion.x<695 && event1.motion.y<346 && event1.motion.y>286) {
        bts2 = 8; // Souris sur le bouton sound
    }  else if (event1.motion.x>189 && event1.motion.x<249 && event1.motion.y<347 && event1.motion.y>287) {
        bts2 = 9; // Souris sur le bouton sound
    }  else if (event1.motion.x>1693 && event1.motion.x<1753 && event1.motion.y<347 && event1.motion.y>287) {
        bts2 = 10; // Souris sur le bouton sound
    }  else if (event1.button.x>1248 && event1.button.x<1308 && event1.button.y<347 && event1.button.y>287) {
        bts2 = 11; // Souris sur le bouton sound
    }    
	 else if (event1.button.x>1452 && event1.button.x<1552 && event1.button.y<278 && event1.button.y>199) {
        bts2 = 2; // Souris sur le bouton sound
    }
	else if (event1.button.x>614 && event1.button.x<802 && event1.button.y<482 && event1.button.y>364) {
        bts2 = 3; // Souris sur le bouton full screen
    }   
	else if (event1.button.x>983 && event1.button.x<1167 && event1.button.y<482 && event1.button.y>362) {
        bts2 = 4; // Souris sur le bouton normal screen
    } 
	else if (event1.button.x>789 && event1.button.x<1005 && event1.button.y<999 && event1.button.y>896) {
        bts2 = 5; // Souris sur le bouton return
    } else if (event1.button.x>1078 && event1.button.x<1577 && event1.button.y<957 && event1.button.y>594) {
        bts2 = 6; // Souris sur le bouton updown
    } else if (event1.button.x>216 && event1.button.x<703 && event1.button.y<948 && event1.button.y>586) {
        bts2 = 7; // Souris sur le bouton qzsd
    } 
	else {
        bts2 = 0; // Souris n'est sur aucun bouton
    }    
}//case
	case 4:if (event1.motion.x > 300 && event1.motion.x < 400 && event1.motion.y > 310 && event1.motion.y < 460) {
        bts4 = 1; // Souris sur le bouton return
 }      else {
        bts4 = 0; // Souris n'est sur aucun bouton
    }   

	case 8:if (event1.motion.x > 300 && event1.motion.x < 400 && event1.motion.y > 310 && event1.motion.y < 460) {
        bts8 = 1; // Souris sur le bouton return
 }      else {
        bts8 = 0; // Souris n'est sur aucun bouton
    }

		
		}
        }
}

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

    if (set == 1) {        
    afficher0(background1, screen);
    afficher0(button1, screen);

    if (bts == 1) {        
        afficher0(button21, screen); 
    } else {
        afficher0(button2, screen); 
    }
    if (bts == 2) {
        afficher0(button31, screen);
    } else {
        afficher0(button3, screen);  
    }
    if (bts == 4) {
        afficher0(buttoncr1, screen);
    } else {
        afficher0(buttoncr, screen);  
    }
    if (bts == 5) {
        afficher0(buttonsc1, screen);
    } else {
        afficher0(buttonsc, screen);  
    }
    if (bts == 3) {
        afficher0(button41, screen);
    } else {
        afficher0(button4, screen);
    }
    if (bts>= 0 && bts <=6) {        
		afficher_animation( anim,screen);
    }
    SDL_Delay(16); 
    SDL_Flip(screen);
    //SDL_Flip(screen);

}

	else if (set==8){
	afficher0(backskin, screen);
	if (skin==1)
	{initPlayer(&player,100,700,"spritesheet1.png");set=3;}
	if (skin==2)
	{initPlayer(&player,100,700,"spritesheet2.png");set=3;}
	if (skin==3)
	{initPlayer(&player,100,700,"spritesheet3.png");set=3;}
	SDL_Flip(screen);
}


	else if (set==2){
	afficher0(background2, screen);

	if (musicVolume==128){afficher0(volume100,screen);}
	if (musicVolume==108){afficher0(volume80,screen);}
	if (musicVolume==88){afficher0(volume60,screen);}
	if (musicVolume==68){afficher0(volume40,screen);}
	if (musicVolume==48){afficher0(volume30,screen);}
	if (musicVolume==28){afficher0(volume20,screen);}
	if (musicVolume==8){afficher0(volume0,screen);}

	if (soundVolume==128){afficher0(sound100,screen);}
	if (soundVolume==108){afficher0(sound80,screen);}
	if (soundVolume==88){afficher0(sound60,screen);}
	if (soundVolume==68){afficher0(sound40,screen);}
	if (soundVolume==48){afficher0(sound30,screen);}
	if (soundVolume==28){afficher0(sound20,screen);}
	if (soundVolume==8){afficher0(sound0,screen);}



	if (control==0){afficher0(checkd,screen);}
	else{afficher0(checkg,screen);}

	if(bts2==3){afficher0(buttonfls,screen);}
	if(bts2==4){afficher0(buttonns,screen);}
	if(bts2==5){afficher0(buttonrt,screen);}
	if (mute){
		afficher0(buttonmt, screen);	
		if (bts2==1){afficher0(buttonmt1, screen);}
		 }
	else{
		if (bts2==1){afficher0(buttonmusic, screen);}
	    }
	if (soundpaused==0){
	        if(bts2==2){afficher0(buttonsound,screen);}
		}
	else{
		afficher0(buttonsoundmute,screen);
		if(bts2==2){afficher0(buttonsoundmute1,screen);}
		}
	if(bts2==6){afficher0(buttonup,screen);}
	if(bts2==7){afficher0(buttonqz,screen);}
	if(bts2==8){afficher0(musicp,screen);}
	if(bts2==9){afficher0(musicm,screen);}
	if(bts2==10){afficher0(soundp,screen);}
	if(bts2==11){afficher0(soundm,screen);}
	SDL_Flip(screen);
}
	
	else if (set==3){
	    	while (!done1) 
		{
        	while (SDL_PollEvent(&event)) 
		{
             		switch (event.type) 
			{
            			case SDL_QUIT:
                			done1 = 1;
                		break;
                		case SDL_MOUSEBUTTONDOWN: 
					                    if (game.turn == 1) 
                        handle_player_input(event.button.x,event.button.y,&game);
					if(event.button.button==SDL_BUTTON_LEFT && event.motion.y<=400 && event.motion.y>=300 && event.motion.x<=741 && event.motion.x>=61 )
							input=10; 
					if(event.button.button==SDL_BUTTON_LEFT && event.motion.y<=554 && event.motion.y>=454 && event.motion.x<=741 && event.motion.x>=61 )
							input=11; 
					if(event.button.button==SDL_BUTTON_LEFT && event.motion.y<=708 && event.motion.y>=608 && event.motion.x<=741 && event.motion.x>=61 )
							input=12;
			        break;
	    			/*case SDL_MOUSEBUTTONDOWN:	
					if(event.button.button == SDL_BUTTON_LEFT)
					{
				 		if (event.button.x>0 && event.button.x<1800 && event.button.y<1040 && event.button.y>0)
							{multi=!multi;}//to switch mode 
					}
	 			break;*/
            			case SDL_KEYDOWN:
					if (multi==0)
					{
/*case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                       printf("Escape key pressed. Quitting.\n");
                       continueGame = 0;
                    }
                    else if (t.playerNumber == 1) { 
                        switch (event.key.keysym.sym) {
                            case SDLK_RETURN:
                                int row = t.track[1];
                                int col = t.track[0];
                                int currentbox = convert(row,col); 
                                if (t.tabfollow[currentbox] == 0) {
                                    updatekeyboard(&t,currentbox);
                                    t.roundNumber++;
                                    printf("current round number:%d.\n", t.roundNumber);
                                    printf("current player number:%d.\n", t.playerNumber);
                                }
                                break;
                            case SDLK_UP:
                                if (t.track[1] > 0) {
                                    t.track[1]--;
                                    int current = convert(t.track[1],t.track[0]);
                                    if (t.tabfollow[current] == 0){
                                        displaydash(&t,current,screen);
                                    }
                                    printf("moved up\n");
                                    
                                }
                                break;
                            case SDLK_DOWN:
                                if (t.track[1] < 2) {
                                    t.track[1]++;
                                    int current = convert(t.track[1],t.track[0]);
                                    if (t.tabfollow[current] == 0){
                                        displaydash(&t,current,screen);
                                    }
                                    printf("moved down\n");
                                }
                                break;
                            case SDLK_LEFT:
                                if (t.track[0] > 0) {
                                    t.track[0]--;
                                    int current = convert(t.track[1],t.track[0]);
                                    if (t.tabfollow[current] == 0){
                                        displaydash(&t,current,screen);
                                    }
                                    printf("moved to the left\n");
                                }
                                break;
                            case SDLK_RIGHT:
                                if (t.track[0] < 2) {
                                    t.track[0]++;
                                    int current = convert(t.track[1],t.track[0]);
                                    if (t.tabfollow[current] == 0){
                                        displaydash(&t,current,screen);
                                    }
                                    printf("moved to the right\n");
                                }
                                break;
                            default:
                                printf("use up down left right\n");
                                break;
                        }
                    }
                    break;

*/
						if (event.key.keysym.sym == SDLK_h) 
							{player.direction=20;}//lose
						else if (event.key.keysym.sym == SDLK_e) 
							{eni=!eni;}//get ready
						else if (event.key.keysym.sym == SDLK_x) 
							{xo=!xo;}//get ready
						else if (event.key.keysym.sym == SDLK_RETURN) {
                            				input=1;
                        				}
						else if (event.key.keysym.sym == SDLK_m) 
							{player.direction=19;}//get ready
						else if (event.key.keysym.sym == SDLK_i) 
							{player.direction=17;}//
						else if (event.key.keysym.sym == SDLK_p) 
							{player.direction=16;}//
						else if (event.key.keysym.sym == SDLK_o) 
							{player.direction=14;}//
						else if (event.key.keysym.sym == SDLK_LEFT) 
							{moveLeft = 1;player.direction=2;}// deplacement gauche
						else if (event.key.keysym.sym == SDLK_w) 
							{/*player.wKeyPressCount+=100; // Increment the count
    							sprintf(player.pressCountText, "%d", player.wKeyPressCount);} // Update display text*/
							// Lors de l'incrémentation du score :
							player.wKeyPressCount += 1;
							// Utilisez snprintf pour éviter de dépasser la taille du tableau
							snprintf(player.pressCountText, sizeof(player.pressCountText), "%d", player.wKeyPressCount);player.direction=18;}
                
						else if (event.key.keysym.sym == SDLK_RIGHT)
							{moveRight = 1;player.direction=1;}// deplacement droit
                				else if (event.key.keysym.sym == SDLK_DOWN) 
							{moveback = 1;player.direction=13;input=3;}// deplacement back
						else if (event.key.keysym.sym == SDLK_RSHIFT) 
							{player.acceleration=1;
if (moveLeft==1){player.direction=3;}
else if (moveRight==1){player.direction=4;}
}// deplacement back
						/*else if (event.key.keysym.sym == SDLK_UP && player.saut) 
						{
                    					player.vy = -15; // Jump with an initial velocity
                    					player.saut = 0; // The player is no longer on the ground
                    					player.direction=3; // Set action to jumping
						}*/else if (event.key.keysym.sym == SDLK_UP) {input=2;
    							if (player.saut == 1) { // First jump
        						player.vy = -17; // Initial jump velocity
        						player.saut = 0; // Now in air
        						player.doubleJump = 1; // Allow double jump
        						player.direction = 5; // Jumping animation
    							} else if (player.doubleJump == 1) { // Check for double jump
        						player.vy = -20; // Double jump velocity
       							 player.doubleJump = 0; // Double jump used
        						player.direction = 5; // Jumping animation
    							}
						}
                				else if (event.key.keysym.sym == SDLK_c)
						{             	             	
						// Condition pour réduire la vie et définir l'état d'animation sur 2 (perte de vie)
                            				if (player.vie >0) 
							{
								if (player.vie==1){                                				
								player.vie--;
								player.direction=9;
                                				life.etatAnimation = 2;}
								else{player.vie--;
								player.direction=6;
                                				life.etatAnimation = 2;}
                            				}							
						}
                				else if (event.key.keysym.sym == SDLK_v)
						{
                            			// Condition pour augmenter la vie et définir l'état d'animation sur 1 (gain de vie)
                            				if (player.vie < 3) 
							{
                                				player.vie++;
                                				life.etatAnimation = 1;
								player.direction=21;
                            				}
						}
						else if (event.key.keysym.sym == SDLK_j) 
							{player.direction=8;}// deplacement back
						else if (event.key.keysym.sym == SDLK_k) 
							{player.direction=7;
if (shot_count < MAX_SHOTS) {
        initshot(&shots[shot_count], player.pos1.x + 185, player.pos1.y+20);
        shot_count++;
    }}// deplacement back
                			}
					else //multi=1
					{ 
                				if (event.key.keysym.sym == SDLK_LEFT) 
							{moveLeft = 1;player.direction=2;}// deplacement gauche
						else if (event.key.keysym.sym == SDLK_w) 
							{player.wKeyPressCount+=100; // Increment the count
    							sprintf(player.pressCountText, "%d", player.wKeyPressCount);} // Update display text                
						else if (event.key.keysym.sym == SDLK_RIGHT)
							{moveRight = 1;player.direction=1;}// deplacement droit
                				else if (event.key.keysym.sym == SDLK_DOWN) 
							{moveback = 1;player.direction=5;}// deplacement back
						else if (event.key.keysym.sym == SDLK_RSHIFT) 
							{player.acceleration=1;player.direction=4;}// deplacement back
						else if (event.key.keysym.sym == SDLK_UP && player.saut) 
						{
                    					player.vy = -15; // Jump with an initial velocity
                    					player.saut = 0; // The player is no longer on the ground
                    					player.direction=3; // Set action to jumping
						}
                				else if (event.key.keysym.sym == SDLK_c)
						{             	             	
						// Condition pour réduire la vie et définir l'état d'animation sur 2 (perte de vie)
                            				if (player.vie >0) 
							{
								if (player.vie==1){                                				
								player.vie--;
								player.direction=9;
                                				life.etatAnimation = 2;}
								else{player.vie--;
								player.direction=6;
                                				life.etatAnimation = 2;}
                            				}							
						}
                				else if (event.key.keysym.sym == SDLK_v)
						{
                            			// Condition pour augmenter la vie et définir l'état d'animation sur 1 (gain de vie)
                            				if (player.vie < 3) 
							{
                                				player.vie++;
								life.etatAnimation = 1;
                            				}
						}
						else if (event.key.keysym.sym == SDLK_j) 
							{player.direction=7;}// deplacement back
						else if (event.key.keysym.sym == SDLK_k) 
							{player.direction=8;}// deplacement back
						else if (event.key.keysym.sym == SDLK_q) 
							{moveLeft2 = 1;player2.direction=2;}// deplacement gauche
						else if (event.key.keysym.sym == SDLK_g) 
							{player2.wKeyPressCount+=100; // Increment the count
    							sprintf(player2.pressCountText, "%d", player2.wKeyPressCount);} // Update display text                
						else if (event.key.keysym.sym == SDLK_d)
							{moveRight2 = 1;player2.direction=1;}// deplacement droit
                				else if (event.key.keysym.sym == SDLK_s) 
							{moveback2 = 1;player2.direction=5;}// deplacement back
						else if (event.key.keysym.sym == SDLK_LSHIFT) 
							{player2.acceleration=1;player2.direction=4;}// deplacement back
						else if (event.key.keysym.sym == SDLK_z && player2.saut) 
						{
                    					player2.vy = -15; // Jump with an initial velocity
                    					player2.saut = 0; // The player is no longer on the ground
                    					player2.direction=3; // Set action to jumping
						}
                				else if (event.key.keysym.sym == SDLK_b)
						{             	             	
						// Condition pour réduire la vie et définir l'état d'animation sur 2 (perte de vie)
                            				if (player2.vie >0) 
							{
								if (player2.vie==1){                                				
								player2.vie--;
								player2.direction=9;
                                				life5.etatAnimation = 2;}
								else{player2.vie--;
								player2.direction=6;
                                				life5.etatAnimation = 2;}
                            				}							
						}
                				else if (event.key.keysym.sym == SDLK_n)
						{
                            			// Condition pour augmenter la vie et définir l'état d'animation sur 1 (gain de vie)
                            				if (player2.vie < 3) 
							{
                                				player2.vie++;
                                				life5.etatAnimation = 1;
                            				}
						}
						else if (event.key.keysym.sym == SDLK_t) 
							{player2.direction=7;}// deplacement back
						else if (event.key.keysym.sym == SDLK_y) 
							{player2.direction=8;}// deplacement back

				       }
				break;
            			case SDL_KEYUP:
					if (multi==1){
                				if (event.key.keysym.sym == SDLK_LEFT ) {moveLeft = 0;
							if (moveRight==1){player.direction=1;}
							else if (moveback==1){player.direction=4;}
							else {player.direction=0;}}
                else if (event.key.keysym.sym == SDLK_RIGHT) {moveRight = 0;
							if (moveLeft==1){player.direction=2;}
							else if (moveback==1){player.direction=4;}
							else {player.direction=0;}}
		else if (event.key.keysym.sym == SDLK_DOWN) {moveback = 0;
							if (moveRight==1){player.direction=1;}
							else if (moveLeft==1){player.direction=2;}
							else {player.direction=0;}}
		else if (event.key.keysym.sym == SDLK_RSHIFT) {player.acceleration=0;
								if (moveRight==1){player.direction=1;}
								else if (moveLeft==1){player.direction=2;}
								else {player.direction=0;}}
                else if (event.key.keysym.sym == SDLK_UP) {
			player.saut=0;} // Reset to idle if the movement keys are released
//player2		
		else if (event.key.keysym.sym == SDLK_q ) {moveLeft2 = 0;
							if (moveRight2==1){player2.direction=1;}
							else if (moveback2==1){player2.direction=4;}
							else {player2.direction=0;}}
                else if (event.key.keysym.sym == SDLK_d) {moveRight2 = 0;
							if (moveLeft2==1){player2.direction=2;}
							else if (moveback2==1){player2.direction=4;}
							else {player2.direction=0;}}
		else if (event.key.keysym.sym == SDLK_s) {moveback2 = 0;
							if (moveRight2==1){player2.direction=1;}
							else if (moveLeft2==1){player2.direction=2;}
							else {player2.direction=0;}}
		else if (event.key.keysym.sym == SDLK_LSHIFT) {player2.acceleration=0;}
                else if (event.key.keysym.sym == SDLK_z) {
			player2.saut=0;} // Reset to idle if the movement keys are released
}	
		else{
                if (event.key.keysym.sym == SDLK_LEFT ) {moveLeft = 0;
							if (moveRight==1){player.direction=1;}
							else if (moveback==1){player.direction=4;}
							else {player.direction=0;}}
                else if (event.key.keysym.sym == SDLK_RIGHT) {moveRight = 0;
							if (moveLeft==1){player.direction=2;}
							else if (moveback==1){player.direction=4;}
							else {player.direction=0;}}
		else if (event.key.keysym.sym == SDLK_DOWN) {moveback = 0;
							if (moveRight==1){player.direction=1;}
							else if (moveLeft==1){player.direction=2;}
							else {player.direction=0;}}
		else if (event.key.keysym.sym == SDLK_RSHIFT) {player.acceleration=0;}
		else if (event.key.keysym.sym == SDLK_k) {player.direction=0;}
		else if (event.key.keysym.sym == SDLK_j) {player.direction=0;}
                else if (event.key.keysym.sym == SDLK_UP) {
			player.saut=0;} // Reset to idle if the movement keys are released
}
                break;
                case SDL_MOUSEMOTION: 
					if(event.motion.y<=400 && event.motion.y>=300 && event.motion.x<=741 && event.motion.x>=61)
							input=5; 
					if(event.motion.y<=554 && event.motion.y>=454 && event.motion.x<=741 && event.motion.x>=61 )
							input=6; 
					if(event.motion.y<=708 && event.motion.y>=608 && event.motion.x<=741 && event.motion.x>=61 )
							input=7;
			break;
        }
    }


	// Logique d'animation et d'affichage
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0)); // Efface l'écran

        if (multi==0){
	animerPlayer(&player, player.direction);
        // Logique de mise à jour du jeu (à remplir)
        animateCoin(&coin);
        animateCoin(&coin1);
        animateCoin(&coin2);
        animateCoin(&coin3);
        animateCoin(&coin4);
        animateCoin(&coin5);
        animateCoin(&clock);
	blit_background(&bg,screen);
//////////////////////////////////////////////////////////////
        afficherminimap(m, screen);
        afficherTemps(&timeingame, screen);

        SDL_BlitSurface(m.animation.spriteSheet, &m.animation.Clips[m.animation.clipLoaded], screen, &firePos);

        if ((SDL_GetTicks()) % 2 == 0)
        {
            if (m.animation.clipLoaded < m.animation.frames)
            {
                m.animation.clipLoaded++;
            }
            else
                m.animation.clipLoaded = 0;
        }


        //afficherminimap(m, screen);

        afficher(player, screen, font,multi,1,image,dest);
     for (int i = 0; i < shot_count; i++) {
    animershot(&shots[i]);
    affichershot(shots[i], screen);
    moveshot(&shots[i], 7);
}
    if (collisionTri(&myEntity, player.pos1) && myEntity.isActive) {
    player.vie--;  // Décrémenter la vie
    myEntity.isActive = false;  // Désactiver l'entité pour éviter d'autres collisions
    printf("Collision avec myEntity. Vies restantes: %d\n", player.vie);
    } else {

   //printf("Vies restantes: %d\n", player.vie);
    //printf("Pas de collision avec myEntity ou collision déjà gérée.\n");
    }

    
    if (collisionTri(&myEntity2, player.pos1)) {
        player.vie--;
    } else {
        //printf("Pas de collision avec myEntity2.\n");
    }

    if (collisionTri(&coin.pos, player.pos1)) {
        player.wKeyPressCount+=100; // Increment the count
    							sprintf(player.pressCountText, "%d", player.wKeyPressCount);;
    } 
    if (collisionTri(&coin1.pos, player.pos1)) {
        player.wKeyPressCount+=100; // Increment the count
    							sprintf(player.pressCountText, "%d", player.wKeyPressCount);;
    }
    if (collisionTri(&coin2.pos, player.pos1)) {
        player.wKeyPressCount+=100; // Increment the count
    							sprintf(player.pressCountText, "%d", player.wKeyPressCount);;
    }
    if (collisionTri(&coin3.pos, player.pos1)) {
        player.wKeyPressCount+=100; // Increment the count
    							sprintf(player.pressCountText, "%d", player.wKeyPressCount);;
    }
    if (collisionTri(&coin4.pos, player.pos1)) {
        player.wKeyPressCount+=100; // Increment the count
    							sprintf(player.pressCountText, "%d", player.wKeyPressCount);;
    }
    if (collisionTri(&coin5.pos, player.pos1)) {
        player.wKeyPressCount+=100; // Increment the count
    							sprintf(player.pressCountText, "%d", player.wKeyPressCount);;
    }

    		if (player.acceleration==0){
    		// Apply continuous movement
    		if (moveRight) {
			if (player.pos1.x < SCREEN_W / 2 - player.pos1.w/2){
			//scrolling_right(&bg,&test_scrolling_end_right);
				if ( test_scrolling_end_right == 0 ){
					moveImage(&player, 3, 0);
				}
			}
			else{scrolling_right(&bg,&test_scrolling_end_right);}
}
    		if (moveLeft) {
			if (player.pos1.x + (player.pos1.w/2) > 0) {
			moveImage(&player, -2, 0);scrolling_left(&bg, &test_scrolling_end_left);
				if ( test_scrolling_end_left == 0 ){
					scrolling_left(&bg, &test_scrolling_end_left);
				}
			else {scrolling_left(&bg, &test_scrolling_end_left);}
}
}
}
    		else {if (moveLeft && player.pos1.x + (player.pos1.w/2) > 0) 
			moveImage(&player, -6, 0);
    		if (moveRight && player.pos1.x < SCREEN_W / 2 - player.pos1.w/2) 
			moveImage(&player, 6, 0);}

    
    		// Jumping logic
    		if (!player.saut) {
        	moveImage(&player, 0, player.vy); // Apply vertical velocity
        	player.vy += 1; // Gravity effect
        	if (player.pos1.y >= 700) { // Check if the player is back on the ground
        		    player.pos1.y = 700; // Reset the player's position to the ground level
        		    player.saut = 1; // The player is back on the ground
        		    player.vy = 0; // Reset vertical velocity
                if (moveLeft) {
        		player.direction = 2; // Gauche
    		} else if (moveRight) {
        		player.direction = 1; // Droite
    		} else if (moveback) {
        		player.direction = 4; // Back
    		} else {
        		player.direction = 0; // neutre
    		}
        }
    }


        // Animer et afficher les vies selon `vie`
        switch (player.vie) {
	    case -1:
 	    break;
            case 0:
		if ( life.etatAnimation ==2 ){
                animerlife(&life, 2);
                afficherlife(life, screen);}

                break;            
	    case 1:
		if ( life.etatAnimation ==1 ){
                animerlife(&life, 1);
                afficherlife(life, screen);}
		else if ( life.etatAnimation ==2){
		animerlife(&life, 0);
                afficherlife(life, screen);
                animerlife(&life2, 2);
                afficherlife(life2, screen);}	
                break;
            case 2:	
		if ( life.etatAnimation ==1 ){
                animerlife(&life, 0);
                afficherlife(life, screen);
                animerlife(&life2, 1);
                afficherlife(life2, screen);}
		else if ( life.etatAnimation ==2){
		animerlife(&life, 0);
                afficherlife(life, screen);
                animerlife(&life2, 0);
                afficherlife(life2, screen);
                animerlife(&life3, 2);
                afficherlife(life3, screen);}	
                break;
            case 3:
		if ( life.etatAnimation ==1 ){
                animerlife(&life, 0);
                afficherlife(life, screen);
                animerlife(&life2, 0);
                afficherlife(life2, screen);
                animerlife(&life3, 1);
                afficherlife(life3, screen);}
		else{
                animerlife(&life, 0);
                afficherlife(life, screen);
                animerlife(&life2, 0);
                afficherlife(life2, screen);
                animerlife(&life3, 0);
                afficherlife(life3, screen);}
                break;
        }
        move(&myEntity);
        move2(&myEntity2);
        afficherEnnemi(&myEntity, screen1);
        afficherEnnemi(&myEntity2, screen1);
        showCoin(&coin, screen1);
        showCoin(&coin1, screen1);
        showCoin(&coin2, screen1);
        showCoin(&coin3, screen1);
        showCoin(&coin4, screen1);
        showCoin(&coin5, screen1);
        showCoin(&clock, screen1);
        //SDL_Flip(screen);
}
	else{//multiplayer
	animerPlayer(&player, player.direction);
	animerPlayer(&player2, player2.direction);
        // Logique de mise à jour du jeu (à remplir)

    		if (player.acceleration==0){
    		// Apply continuous movement
    		if (moveLeft) //&& player.pos1.x + (player.pos1.w/2) > 0 )
			moveImage(&player, -3, 0);
    		if (moveRight )//&& player.pos1.x < SCREEN_W / 4 - player.pos1.w/2) 
			moveImage(&player, 3, 0);}
    		else {if (moveLeft && player.pos1.x + (player.pos1.w/2) > 0) 
			moveImage(&player, -6, 0);
    		if (moveRight && player.pos1.x < SCREEN_W / 4 - player.pos1.w/2) 
			moveImage(&player, 6, 0);}

    		if (player2.acceleration==0){
    		// Apply continuous movement
    		if (moveLeft2 && player2.pos1.x + (player2.pos1.w/2) > SCREEN_W / 2) 
			moveImage(&player2, -3, 0);
    		if (moveRight2 && player2.pos1.x < (SCREEN_W/4)*3 - player2.pos1.w/2) 
			moveImage(&player2, 3, 0);}
    		else {if (moveLeft2 && player2.pos1.x + (player2.pos1.w/2) > SCREEN_W / 2 ) 
			moveImage(&player2, -6, 0);
    		if (moveRight2 && player2.pos1.x < (SCREEN_W/4)*3 - player2.pos1.w/2) 
			moveImage(&player2, 6, 0);}

    
    		// Jumping logic
    		if (!player.saut) {
        	moveImage(&player, 0, player.vy); // Apply vertical velocity
        	player.vy += 1; // Gravity effect
        	if (player.pos1.y >= 700) { // Check if the player is back on the ground
        		    player.pos1.y = 700; // Reset the player's position to the ground level
        		    player.saut = 1; // The player is back on the ground
        		    player.vy = 0; // Reset vertical velocity
                if (moveLeft) {
        		player.direction = 2; // Gauche
    		} else if (moveRight) {
        		player.direction = 1; // Droite
    		} else if (moveback) {
        		player.direction = 4; // Back
    		} else {
        		player.direction = 0; // neutre
    		}
        }
    }

    		// Jumping logic
    		if (!player2.saut) {
        	moveImage(&player2, 0, player2.vy); // Apply vertical velocity
        	player2.vy += 1; // Gravity effect
        	if (player2.pos1.y >= 700) { // Check if the player is back on the ground
        		    player2.pos1.y = 700; // Reset the player's position to the ground level
        		    player2.saut = 1; // The player is back on the ground
        		    player2.vy = 0; // Reset vertical velocity
                if (moveLeft2) {
        		player2.direction = 2; // Gauche
    		} else if (moveRight2) {
        		player2.direction = 1; // Droite
    		} else if (moveback2) {
        		player2.direction = 4; // Back
    		} else {
        		player2.direction = 0; // neutre
    		}
        }
    }
	
        //afficher(background, screen, font,multi,1);
        //afficher(player, screen, font,multi,1);
        //afficher(player2, screen, font,multi,2);
        
        // Animer et afficher les vies selon `vie`
        switch (player.vie) {
	    case -1:
		break;
            case 0:
		if ( life.etatAnimation ==2 ){
                animerlife(&life, 2);
                afficherlife(life, screen);}

                break;            
	    case 1:
		if ( life.etatAnimation ==1 ){
                animerlife(&life, 1);
                afficherlife(life, screen);}
		else if ( life.etatAnimation ==2){
		animerlife(&life, 0);
                afficherlife(life, screen);
                animerlife(&life2, 2);
                afficherlife(life2, screen);}	
                break;
            case 2:	
		if ( life.etatAnimation ==1 ){
                animerlife(&life, 0);
                afficherlife(life, screen);
                animerlife(&life2, 1);
                afficherlife(life2, screen);}
		else if ( life.etatAnimation ==2){
		animerlife(&life, 0);
                afficherlife(life, screen);
                animerlife(&life2, 0);
                afficherlife(life2, screen);
                animerlife(&life3, 2);
                afficherlife(life3, screen);}	
                break;
            case 3:
		if ( life.etatAnimation ==1 ){
                animerlife(&life, 0);
                afficherlife(life, screen);
                animerlife(&life2, 0);
                afficherlife(life2, screen);
                animerlife(&life3, 1);
                afficherlife(life3, screen);}
		else{
                animerlife(&life, 0);
                afficherlife(life, screen);
                animerlife(&life2, 0);
                afficherlife(life2, screen);
                animerlife(&life3, 0);
                afficherlife(life3, screen);}
                break;
        }

        switch (player2.vie) {
	    case -1:
		break;
            case 0:
		if ( life5.etatAnimation ==2 ){
                animerlife(&life5, 2);
                afficherlife(life5, screen);}

                break;            
	    case 1:
		if ( life5.etatAnimation ==1 ){
                animerlife(&life5, 1);
                afficherlife(life5, screen);}
		else if ( life5.etatAnimation ==2){
		animerlife(&life5, 0);
                afficherlife(life5, screen);
                animerlife(&life6, 2);
                afficherlife(life6, screen);}	
                break;
            case 2:
		if ( life5.etatAnimation ==1 ){
                animerlife(&life5, 0);
                afficherlife(life5, screen);
                animerlife(&life6, 1);
                afficherlife(life6, screen);}
		else if ( life5.etatAnimation ==2){
		animerlife(&life5, 0);
                afficherlife(life5, screen);
                animerlife(&life6, 0);
                afficherlife(life6, screen);
                animerlife(&life7, 2);
                afficherlife(life7, screen);}	
                break;
            case 3:
		if ( life5.etatAnimation ==1 ){
                animerlife(&life5, 0);
                afficherlife(life5, screen);
                animerlife(&life6, 0);
                afficherlife(life6, screen);
                animerlife(&life7, 1);
                afficherlife(life7, screen);}
		else{
                animerlife(&life5, 0);
                afficherlife(life5, screen);
                animerlife(&life6, 0);
                afficherlife(life6, screen);
                animerlife(&life7, 0);
                afficherlife(life7, screen);}
                break;
        }
}



        MAJMinimap(player.pos1, &m, camera, 20);
        if (eni==1){
                afficher_enigme(screen,e);
                if(e.quit!=0)
		{
			done=0;
			SDL_Delay(1000);
		}
		if(e.reponsech!=0)
			e.quit=1;
		animer_enigme(&e);
		switch(input)
		{
			case 5:
			e.pos=1;
			break;
			case 6:
			e.pos=2;
			break;
			case 7:
			e.pos=3;
			break;
				case 2:
				if(e.pos>1)
					e.pos--;
				else
					e.pos=3;
				break;
				case 3:
				if(e.pos<3)
					e.pos++;
				else
					e.pos=1;
				break;
			case 1:
			case 10:
			case 11:
			case 12:
			switch(e.pos)
			{
					case 1:
						e.reponsech=1;
					break;
					case 2:
						e.reponsech=2;
					break;
					case 3:
						e.reponsech=3;
					break;		
			}
		}
        }


if (xo==1){        if(game.turn==-1 && game.game_over == 0) 
                ai_make_move(&game);            
        
        animation(&game);        
        draw_board(game,screen);

        
        if(check_game_over(game)){

            int winner = check_win(game);
            if(winner){             
                if (game.turn==-1) {            
            
                    if(game.start_animation ==0)        
                    {
                        game.Zoom=0.6;
                        game.start_animation=1;
                        game.animation=SDL_GetTicks();
                    }
            show_result(&game,"AI Player (X) wins!",screen);
            game.game_over=1;
        }

            else if (game.turn==1) {
                if(game.start_animation == 0)
                        {
                            game.Zoom=0.3;
                            game.start_animation=1;
                            game.animation=SDL_GetTicks();
                        }
                game.game_over=1;
                show_result(&game,"player (O) wins!",screen);
                
            }
         
        } else {                    
            game.game_over=1;
            show_result(&game,"Tie game!",screen);
            
        }

        }
        }
        // Update the screen
        input=-1;
	SDL_Delay(16); // Cap frame rate
        SDL_Flip(screen); // Met à jour l'écran
    }
}
	
	/*else if (set == 4) {
    afficher(background4, screen);
    displayText(t,screen);
    if (bts4 == 1) {
        afficher(buttonabt1, screen);
    } else {
        afficher(buttonabt, screen);
    }

    SDL_Flip(screen);

}*/

}


    TTF_Quit();
    liberer0(&background1);
    liberer0(&background2);
    liberer0(&background3);
    liberer0(&background4);
    liberer0(&button1);
    liberer0(&buttonsc);
    liberer0(&buttonsc1);
    liberer0(&buttoncr);
    liberer0(&buttoncr1);
    liberer0(&button2);
    liberer0(&button21);
    liberer0(&button3);
    liberer0(&button31);
    liberer0(&button4);
    liberer0(&button41);
//set2
    liberer0(&buttonsound);
    liberer0(&buttonfls);
    liberer0(&buttonns);
    liberer0(&buttonrt);
    liberer0(&buttonmt);
    liberer0(&buttonmusic);
    liberer0(&buttonup);
    liberer0(&buttonqz);
    liberer0(&buttonsoundmute);
    liberer0(&buttonsoundmute1);
    liberer0(&checkd);
    liberer0(&checkg);
    //liberer(&buttonabt);
    //liberer(&buttonabt1);
    liberer0(&musicp);
    liberer0(&musicm);
    liberer0(&soundp);
    liberer0(&soundm);
    liberer0(&volume100);
    liberer0(&volume80);
    liberer0(&volume60);
    liberer0(&volume40);
    liberer0(&volume30);
    liberer0(&volume20);
    liberer0(&volume0);

    liberer0(&sound100);
    liberer0(&sound80);
    liberer0(&sound60);
    liberer0(&sound40);
    liberer0(&sound30);
    liberer0(&sound20);
    liberer0(&sound0);

    SDL_FreeSurface(image);
    liberer(player);
    liberer(life);
    liberer(life2);
    liberer(life3);
    liberer(player2);
    liberer(life5);
    liberer(life6);
    liberer(life7);
    free_background(&bg);

    if (myEntity.spriteSheet) {
        SDL_FreeSurface(myEntity.spriteSheet);
    }
    if (myEntity2.spriteSheet) {
        SDL_FreeSurface(myEntity2.spriteSheet);
    }
    if (coin.spriteSheet) {
        SDL_FreeSurface(coin.spriteSheet);
    }

    clean(&timeingame, &m);
    TTF_CloseFont(font1);
    TTF_CloseFont(font);
    SDL_Quit();

    liberer_animation(anim);
    Mix_FreeMusic(music);
    Mix_FreeChunk(clickSound); // Free the sound effect
    Mix_CloseAudio();
    IMG_Quit();
    return 0;
}

