#include<SDL2/SDL.h>
#include<iostream>
#include<math.h>
#include<ctime>



bool quit = false;
bool quit2 = false;
void Destroy();
SDL_Event event;

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Surface *brick;
SDL_Surface *ball;
SDL_Surface *bk;
SDL_Surface *bat;

SDL_Texture *bricktexture;
SDL_Texture *balltexture;
SDL_Texture *bktexture;
SDL_Texture *battexture;


int getRandom(int min,int max){
    srand(time(0));
    return min + rand()%(max+1-min);
}

int ballx = getRandom(0,630-20);
int bally = 80;
int ballvelx = getRandom(-4,4);
int ballvely = getRandom(4,5);
int bkw=630;
int bkh=480;
int bkwmin=0;
int bkhmin=0;
int brickw=70;
int brickh=30;
int batx = bkw/2-25;
int baty = bkh-20;
int delete_brick_count=0;
int no_of_bricks=18;

SDL_Rect ballrect;
SDL_Rect brickrect[2][9];

void InitializeBrick(){
	brickrect[0][0]={0,10,brickw,brickh};
	brickrect[0][1]={70,10,brickw,brickh};
	brickrect[0][2]={140,10,brickw,brickh};
	brickrect[0][3]={210,10,brickw,brickh};
	brickrect[0][4]={280,10,brickw,brickh};
	brickrect[0][5]={350,10,brickw,brickh};
	brickrect[0][6]={420,10,brickw,brickh};
	brickrect[0][7]={490,10,brickw,brickh};
	brickrect[0][8]={560,10,brickw,brickh};

	brickrect[1][0]={0,40,brickw,brickh};
	brickrect[1][1]={70,40,brickw,brickh};
	brickrect[1][2]={140,40,brickw,brickh};
	brickrect[1][3]={210,40,brickw,brickh};
	brickrect[1][4]={280,40,brickw,brickh};
	brickrect[1][5]={350,40,brickw,brickh};
    brickrect[1][6]={420,40,brickw,brickh};
    brickrect[1][7]={490,40,brickw,brickh};
    brickrect[1][8]={560,40,brickw,brickh};

}

void EventHandler(){

	SDL_PollEvent(&event);
	if(event.type==SDL_QUIT){
		quit=true;
	}
}
void Destroy(){
	SDL_DestroyTexture(battexture);
	SDL_DestroyTexture(bricktexture);
	SDL_DestroyTexture(bktexture);
	SDL_DestroyTexture(balltexture);

	SDL_FreeSurface(bat);
	SDL_FreeSurface(brick);
	SDL_FreeSurface(bk);
	SDL_FreeSurface(ball);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}
int gameOver(){
	SDL_Surface *go = SDL_LoadBMP("gover.bmp");
	SDL_Texture *gotexture = SDL_CreateTextureFromSurface(renderer,go);
	SDL_Rect gorect = {0,0,bkw,bkh};
	SDL_RenderCopy(renderer,gotexture,NULL,&gorect);
	SDL_RenderPresent(renderer);
	SDL_Delay(5000);
	Destroy();
	SDL_Quit();


}
int startScreen(){
    SDL_Surface *mn = SDL_LoadBMP("start.bmp");
    SDL_Texture *mntexture = SDL_CreateTextureFromSurface(renderer,mn);
    SDL_Rect mnrect = {0,0,bkw,bkh};
    SDL_RenderCopy(renderer,mntexture,NULL,&mnrect);
    SDL_RenderPresent(renderer);
    SDL_PollEvent(&event);
    if(event.type==SDL_KEYDOWN){
        quit2 = true;
    }

}

int bat_ball_collision(){
    if((ballx+20>=batx+20)&&(bally+20==baty)&&(ballx<=batx+40)){
        return 1;
    }
    else if(((ballx+20==batx)||(ballx==batx+60))&&(bally+20>baty)){
        return 2;
    }
    // left collision
    else if((ballx+20>=batx)&&(bally+20==baty)&&(ballx+20<batx+20)){
        return 3;
    }
    // right collision
    else if((ballx>batx+40)&&(ballx<=batx+60)&&(bally+20==baty)){
        return 4;
    }
    else
        return 0;
}

void moveBall(){
	ballx = ballx + ballvelx;
	bally = bally + ballvely;
    if(bat_ball_collision()==1){
        ballvely = -ballvely;
    }
    else if(bat_ball_collision()==2){
        ballvelx = -ballvelx;
    }
    else if(bat_ball_collision()==3){
        ballvely = -ballvely;
        ballvelx = -abs(ballvelx);
    }
    else if(bat_ball_collision()==4){
        ballvely = -ballvely;
        ballvelx = abs(ballvelx);
    }
}


void ball_collision(){
	if(ballx<=bkwmin || ballx>=bkw-20){
		ballvelx=-ballvelx;
	}
	if(bally<=bkhmin )
	{
	ballvely=-ballvely;
	}
	if(bally>bkh+50){
		gameOver();
	}

}
bool ball_brick_collision_detect(SDL_Rect rect1,SDL_Rect rect2){

	if ( rect1.x > rect2.x + rect2.w ){
		return false;
	}
	if ( rect1.x + rect1.w < rect2.x ){
		return false;
	}
	if ( rect1.y > rect2.y + rect2.h ){
		return false;
	}
	if ( rect1.y + rect1.h < rect2.y ) {
		return false;
	}
	return true;

}
void ball_brick_collision(){
	bool a;
	 for(int i=0; i<2; i++){
	 	for(int j=0; j<9; j++){
	 		a= ball_brick_collision_detect(brickrect[i][j],ballrect);
		 	if(a==true){
		 		brickrect[i][j].x=3000;
			 	ballvely = -ballvely;
			 	delete_brick_count++;
			}
		 	a=false;
		 }

	 }

}


void winning(){
	SDL_Surface *win = SDL_LoadBMP("win.bmp");
	SDL_Texture *wintexture=SDL_CreateTextureFromSurface(renderer,win);
	SDL_Rect winrect={0,0,630,480};
	SDL_RenderCopy(renderer,wintexture,NULL,&winrect);
	SDL_RenderPresent(renderer);
	SDL_Delay(5000);
	Destroy();
	SDL_Quit();

}

int main(int argc, char ** argv){

	SDL_Init(SDL_INIT_VIDEO);
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;



    const Uint8 *keyState;
    Uint32 frameStart;
    int frameTime;

	window = SDL_CreateWindow("The Game",
	SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,630,480,0);
	renderer = SDL_CreateRenderer(window,-1,0);
	SDL_Rect bkrect = {0,0,630,480};
	InitializeBrick();
	ball = SDL_LoadBMP("ball.bmp");
	bk = SDL_LoadBMP("background.bmp");
	bat = SDL_LoadBMP("bat.bmp");
	brick = SDL_LoadBMP("block.bmp");
	balltexture = SDL_CreateTextureFromSurface(renderer,ball);
	bktexture = SDL_CreateTextureFromSurface(renderer,bk);
	battexture = SDL_CreateTextureFromSurface(renderer,bat);
	bricktexture = SDL_CreateTextureFromSurface(renderer,brick);
	SDL_RenderCopy(renderer,bktexture,NULL,&bkrect);
	startScreen();
	while(!quit2){
        startScreen();
	}
	while(!quit ){
        frameStart = SDL_GetTicks();
		EventHandler();
		ballrect = {ballx,bally,20,20};
		SDL_Rect batrect = {batx,baty,60,12};

		moveBall();
		keyState = SDL_GetKeyboardState(NULL);

            if(keyState[SDL_SCANCODE_RIGHT] && batx <= 630-60){
                batx += 5;
            }
            if(keyState[SDL_SCANCODE_LEFT] && batx >= 0 ){
                batx -= 5;
            }

		ball_collision();
		ball_brick_collision();
		if(delete_brick_count==no_of_bricks){
			winning();
		}

		SDL_RenderCopy(renderer,bktexture,NULL,&bkrect);
		SDL_RenderCopy(renderer,balltexture,NULL,&ballrect);
		SDL_RenderCopy(renderer,battexture,NULL,&batrect);

		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][0]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][1]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][2]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][3]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][4]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][5]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][6]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][7]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][8]);

		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][0]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][1]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][2]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][3]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][4]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][5]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][6]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][7]);
		SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][8]);



		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);

		frameTime = SDL_GetTicks() - frameStart;
            if(frameDelay > frameTime){
                SDL_Delay(frameDelay-frameTime);
            }
        }

	Destroy();
	SDL_Quit();

}
