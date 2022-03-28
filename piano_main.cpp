#include "Game.h"

int main(int argc, char* argv[])
{
    srand(time(0));
    Game* game = new Game;
    game->init("PianoTitles",SDL_WINDOWPOS_CENTERED,
         SDL_WINDOWPOS_CENTERED,
         WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    int framesPerSec = 60;
    const int secPerFrame = 1000/framesPerSec;
    do{
        Uint32 startTime = SDL_GetTicks();
        game->render();
        game->handleInput();
        game->update();
        int framePeriod = SDL_GetTicks() - startTime;
        if (framePeriod < secPerFrame)
            SDL_Delay(secPerFrame - framePeriod);
    }while(game->isRunning);
    game->clean();
    return 0;
}
