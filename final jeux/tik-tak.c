#include "tik-tak.h"



/*int main(int argc, char* argv[])
{
    
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();  
    SDL_Surface*screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_DOUBLEBUF);
    tik_tak(screen);
    
    
    SDL_Quit();
    return 0;
}*/

int tik_tak(SDL_Surface*screen){

    mini_game game;
    init_mini_game(&game);
    SDL_WM_SetCaption("Tik-Tak", NULL);
    while (game.game)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    game.game = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                 
                    if (game.turn == 1) 
                        handle_player_input(event.button.x,event.button.y,&game); 
                    break;
            }

        }
        if(game.turn==-1 && game.game_over == 0) 
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
        
        SDL_Flip(screen);
        SDL_Delay(100);
    }
    
}

void init_mini_game(mini_game*game){
    for (int i = 0; i < SIZE; i++)
        for (int j = 0;j < SIZE;j++)
            game->game_board[i][j]=0;
    
    game->font=TTF_OpenFont("./src/font.ttf",86);  
    game->result_font=TTF_OpenFont("./src/Norse.ttf",30);
    game->background=IMG_Load("./src/tic-tac_sheet.png");
    game->background_bars=IMG_Load("./src/tic-tac.png");

    if(!game->font)printf("%s","X_O error\n");
    SDL_Color color={255,0,0};
    game->o_img=TTF_RenderText_Blended(game->font,"O",color); 
    game->result=NULL;
    color.g=255;
    color.b=255;
    game->x_img=TTF_RenderText_Blended(game->font,"X",color); 
    if(!game->background)printf("%s","background error\n");
    if(!game->o_img)printf("%s","o_img error\n");
    if(!game->x_img)printf("%s","x_img error\n");

    game->number_animation_background=0;        
    game->timer_animation=SDL_GetTicks();       
    game->turn=1;                               
    game->game=1;                               
    game->pos_screen.x=0;
    game->pos_screen.y=0;
    game->pos_result.x=150;                     
    game->pos_result.y=400;
    game->game_over=0;
    game->start_animation=0;                    
    game->Zoom=0.3;
    game->score=0;                              
}


void animation(mini_game*game){
    if(SDL_GetTicks()-game->number_animation_background>250){   
        
        game->number_animation_background++;
        game->timer_animation=SDL_GetTicks();
        if(game->number_animation_background>=36) 
         game->number_animation_background=0;
    }
}
void draw_board(mini_game game,SDL_Surface*screen)
{
    int width=game.background->w/8;     
    int height=game.background->h/6;    
    
    SDL_Rect animation={
        width*(game.number_animation_background%8), 
        height*(game.number_animation_background/8), 
        width,
        height
    };
    SDL_BlitSurface(game.background,&animation,screen,&game.pos_screen);        
    SDL_BlitSurface(game.background_bars,NULL,screen,&game.pos_screen);         

    for (int i = 0; i < SIZE; i++)
        for (int j = 0;j<SIZE;j++)      
        {
            SDL_Rect pos={
                
                    50+game.pos_screen.x+ j * SPACE_X,  
                    50+game.pos_screen.y+ i * SPACE_Y
                };
            if(game.game_board[i][j]==1)        
                SDL_BlitSurface(game.o_img,NULL,screen,&pos);       
            
            else if(game.game_board[i][j]==-1)
                SDL_BlitSurface(game.x_img,NULL,screen,&pos);           
            
        }
}

void show_result(mini_game*game,char*result,SDL_Surface*screen){
    game->color_result.r=255;
    game->color_result.g=0;
    game->color_result.b=255;
    game->pos_result.x=screen->w/3;
    game->pos_result.y=screen->h/3;
    if(game->result)
        SDL_FreeSurface(game->result);
game->result=TTF_RenderText_Blended(game->result_font,result,game->color_result);
    
    if(check_win(*game)&&game->turn==-1){   
        if(SDL_GetTicks()-game->animation>200){ 
            if(game->Zoom>2)
                game->Zoom=2;
            game->Zoom+=0.3;
            game->animation=SDL_GetTicks();        
        }
        game->result=rotozoomSurface(game->result,0,game->Zoom,0);
    }
    else if(check_win(*game)&&game->turn==1){   
        if(SDL_GetTicks()-game->animation>50){
            game->result=rotozoomSurface(game->result,game->Zoom,2,0); 
            game->Zoom+=30;
        } 
    }
   
SDL_BlitSurface(game->result,NULL,screen,&game->pos_result);
}
void handle_player_input(int x, int y,mini_game*game){  
   int clicked=0;
   int row=0,col=0;
    for (int i = 0; i < SIZE; i++){
        for (int j = 0;j <SIZE;j++){
            printf("%d\t",game->game_board[i][j]);
        }printf("\n");  
    }
   for (int i = 0; i <SIZE; i++)
    for (int j = 0;j < SIZE;j++)
    {
        if((y>game->pos_screen.y+(j*SPACE_Y))&&(y<game->pos_screen.y+(j+1)*SPACE_Y))
            if((x>game->pos_screen.x+(i*SPACE_X))&&(x<game->pos_screen.x+(i+1)*SPACE_X))
            {   
                if(game->game_board[j][i]==0) 
                        {
                            clicked=1;
                            row=j;
                            col=i;
                            break;
                        }
            }
    }
   

    if(clicked==0)return;     
    game->game_board[row][col] = game->turn; 

    //*
    int result = check_game_over(*game); 
    if (result) {
        game->game_over = 1;        
        return;
    }
    //*/
      game->turn=-1;                
}
int ai_make_move(mini_game*game)    
{                                  
    int best_score = -1000;          
    int best_row = -1;
    int best_col = -1;
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (game->game_board[i][j] == 0) {      
                game->game_board[i][j] = 1;          
                int score = minmax(0, game->turn,game); 
                game->game_board[i][j] = 0;             
                if (score > best_score) {               
                    best_score = score;
                    best_row = i;                      
                    best_col = j;
                }
            }
        }
    }
    game->game_board[best_row][best_col] = game->turn;      
    int result = check_game_over(*game);                    
    if (result) {
        game->game_over = 1;
        return 0;
    }
    game->turn=1;                                           

    return 1;
}
int check_game_over(mini_game game){
    if(check_win(game)==0) 
    {
    for (int i = 0; i < SIZE; i++)          
        for (int j = 0; j < SIZE; j++)
            if(game.game_board[i][j]==0)
                return 0;
    }
    return 1;
}







int evaluate_game_state(mini_game game)     
{
     
    for (int i = 0; i < SIZE; i++) {
        int row_sum = 0;
        for (int j = 0; j < SIZE; j++) {
            row_sum += game.game_board[i][j];
        }
        if (row_sum == SIZE) {
            return 10;
        } else if (row_sum == -SIZE) {
            return -10;
        }
    }
    
    
    for (int j = 0; j < SIZE; j++) {
        int col_sum = 0;
        for (int i = 0; i < SIZE; i++) {
            col_sum += game.game_board[i][j];
        }
        if (col_sum == SIZE) {
            return 10;
        } else if (col_sum == -SIZE) {
            return -10;
        }
    }
    
    int diag1=0,diag2=0;
   
    for (int i = 0; i < SIZE; i++)
    {
        diag1+=game.game_board[i][i];
        diag2+=game.game_board[SIZE-1-i][i];
    }
    
  
   
    if (diag1 == SIZE || diag2== SIZE) {
        return 10;
    } else if (diag1 == -SIZE || diag2 == -SIZE) {
        return -10;
    }
    
    
    return 0;
}

int minmax(int depth,int player,mini_game*game)
{
    int game_state = evaluate_game_state(*game); 
    if (game_state == 10) {                 
        return game_state - depth;
    } else if (game_state == -10) {         
        return game_state + depth;
    } else if (game->game_over) {           
        return 0;
    }
    if (depth == MAX_DEPTH) {               
        return 0;
    }
    
    
    int best_score;
    if (player == -1) {
        best_score = -1000;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (game->game_board[i][j] == 0) {
                    game->game_board[i][j] =player; 
                    int score = minmax(depth + 1, -player,game);
                    game->game_board[i][j] = 0;
                    if (score > best_score) {
                        best_score = score;
                    }
                }
            }
        }
    } else {
        best_score = 1000;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (game->game_board[i][j] == 0) {
                    game->game_board[i][j] = player;
                    int score = minmax(depth + 1, -player,game);
                    game->game_board[i][j] = 0;
                    if (score < best_score) {
                        best_score = score;
                    }
                }
            }
        }
    }
    
    return best_score;
}





int check_win(mini_game game)
{   
    for (int i = 0; i < SIZE; i++) {
        int win = 1;
        for (int j = 0; j < SIZE; j++) {
            if (game.game_board[i][j] != game.turn) {
                win = 0;
                break;
            }
        }
        if (win)
        {
            return 1;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        int win = 1;
        for (int j = 0; j < SIZE; j++) {
            if (game.game_board[j][i] != game.turn) {
                win = 0;
                break;
            }
        }
        if (win)
        {    
            return 1;
        }
        
    }
    
    int win = 1;
    for (int i = 0; i <SIZE; i++) {
        if (game.game_board[i][i] != game.turn) {
            win = 0;
            break;
        }
    }
    if (win) 
        {
            return 1;
        }
    win=1;
    
    for (int i = 0; i <SIZE; i++) {
        if (game.game_board[i][SIZE-1-i] != game.turn) {
            win = 0;
            break;
        }
    }
    if (win) 
        {

            return 1;
        }
    return 0;
}
