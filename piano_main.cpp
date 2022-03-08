#include "Game.h"

int main(int argc, char* argv[])
{
    srand(time(0));
    bool isRunning;
    init("PianoTitles",SDL_WINDOWPOS_CENTERED,
         SDL_WINDOWPOS_CENTERED,
         WINDOW_WIDTH, WINDOW_HEIGHT, 0, isRunning);
    int framesPerSec = 60;
    const int secPerFrame = 1000/framesPerSec;
    do{
        Uint32 startTime = SDL_GetTicks();
        render();
        handleInput(isRunning);
        update(isRunning);
        int framePeriod = SDL_GetTicks() - startTime;
        if (framePeriod < secPerFrame)
            SDL_Delay(secPerFrame - framePeriod);
        if (!isRunning)
            SDL_Delay(2000);
    }while (isRunning);
    //render();
    //SDL_Delay(2000);
    clean();
    return 0;
}


/*	Mix_Chunk* chunk1, *chunk2, *chunk3;
	chunk1 = Mix_LoadWAV("pianoHub/piano-mp3/A4.wav");
	chunk2 = Mix_LoadWAV("pianoHub/piano-mp3/A0.wav");
	chunk3 = Mix_LoadWAV("pianoHub/piano-mp3/A6.wav");
	if (chunk1 == NULL)
	{
		printf("%s", Mix_GetError());
	}
	while (isRunning)
	{
		SDL_RenderClear(renderer);
		while (SDL_PollEvent(&mainEvent))
		{
			switch (mainEvent.type)
			{
			case SDL_QUIT:
			{
				isRunning = false;
				break;
			}
			case SDL_KEYDOWN:
			{
				if (mainEvent.key.keysym.sym == SDLK_RETURN)
				{
					if (!Mix_Playing(1)){

						Mix_PlayChannel(1, chunk1, 2);
						//SDL_Delay(1000);
						//Mix_Pause(-1);
						Mix_PlayChannel(2, chunk2, 0);
					}
				}
			}
			default: break;
			}
		}
*/
