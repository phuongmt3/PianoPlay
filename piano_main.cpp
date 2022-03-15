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
    clean();
    return 0;
}
