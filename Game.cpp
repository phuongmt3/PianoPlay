#include "Game.h"

SDL_Window* window;
SDL_Renderer* Renderer::renderer;
SDL_Event event;//why event cannot be a pointer
Title* title1;

void init(const char* title, int xpos, int ypos,
              int width, int height, bool fullscreen, bool& isRunning){
    int flag = 0;
    if (fullscreen)
        flag = SDL_WINDOW_FULLSCREEN;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        cout << "Init completed!\n";
        window = SDL_CreateWindow(title,xpos,ypos,width,height,flag);
        if (window){
            cout << "Created window!\n";
        }
        Renderer::renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawColor(Renderer::renderer,200,180,150,255);
        if (Renderer::renderer)
            cout << "Created renderer\n";
        else cout << "Cannot create renderer\n";
        isRunning = 1;
    }
    else isRunning = 0;
    title1 = new Title(150,200); title1->setPos();
}

void render(){
    SDL_RenderClear(Renderer::renderer);
    SDL_Rect srcR = {0,0,1080,2052}, desR = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
    TextureManager::drawImage("pianoHub/piano.png", srcR, desR);
    title1->show(1);
    SDL_RenderPresent(Renderer::renderer);
}

void handleInput(bool& isRunning){
    SDL_PollEvent(&event);
    switch(event.type)
    {
    case SDL_QUIT:
        isRunning = 0; break;
    default: break;
    }
}

void update(){}

void clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(Renderer::renderer);
    SDL_Quit();
    cout << "Game clean!";
}
