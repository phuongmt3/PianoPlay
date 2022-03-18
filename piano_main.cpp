#include "Game.h"

int main(int argc, char* argv[])
{
    srand(time(0));
    bool isRunning;
    int fail;
    init("PianoTitles",SDL_WINDOWPOS_CENTERED,
         SDL_WINDOWPOS_CENTERED,
         WINDOW_WIDTH, WINDOW_HEIGHT, 0, isRunning, fail);
    int framesPerSec = 60;
    const int secPerFrame = 1000/framesPerSec;
    do{
        Uint32 startTime = SDL_GetTicks();
        render(fail);
        handleInput(isRunning, fail);
        update(isRunning, fail);
        int framePeriod = SDL_GetTicks() - startTime;
        if (framePeriod < secPerFrame)
            SDL_Delay(secPerFrame - framePeriod);
    }while (isRunning);
    /*SDL_RenderClear(Global::renderer);
    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    TTF_Font *gFont = TTF_OpenFont("TTFfonts/Fine College.ttf", 1000);
    if( gFont == NULL )
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
    SDL_Color texColor = { 255,255,255 };
    int n = 6000; string s = std::to_string(n);
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont,&(std::to_string(n))[0],texColor);
    SDL_Texture* mTexture = SDL_CreateTextureFromSurface(Global::renderer, textSurface);
    SDL_FreeSurface( textSurface );
    SDL_Rect des = {300,300,400,100};
    SDL_RenderCopy(Global::renderer,mTexture,NULL,&des);
    SDL_RenderPresent(Global::renderer);
    SDL_Delay(3000);*/
    clean();
    return 0;
}
