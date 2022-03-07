#include "Game.h"

SDL_Window* window;
SDL_Renderer* Global::renderer;
SDL_Event event;//why event cannot be a pointer
SDL_Texture *Global::click, *Global::unclick, *Global::bg;

vector<Tile> tileList;
int tileCount = 100;
int Global::curTileID = 0;


void addTile()
{
    for (int i = 0; i < tileCount; i++)
        tileList.push_back(Tile(150,200,i));
}

void showTile()
{
    for (int i = 0; i < tileCount; i++)
        tileList[i].show();
}

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
        Global::renderer = SDL_CreateRenderer(window, -1, 0);
        if (Global::renderer)
            cout << "Created renderer\n";
        else cout << "Cannot create renderer\n";
        isRunning = 1;
    }
    else isRunning = 0;
    Global::click = TextureManager::takeTexture("pianoHub/tileTouched.png");
    Global::unclick = TextureManager::takeTexture("pianoHub/tileUntouched.png");
    Global::bg = TextureManager::takeTexture("pianoHub/piano.png");
    addTile();
}

void render(){
    SDL_RenderClear(Global::renderer);
    SDL_Rect srcR = {0,0,1080,2052}, desR = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
    TextureManager::drawImage(Global::bg, srcR, desR);
    showTile();
    SDL_RenderPresent(Global::renderer);
}

void handleInput(bool& isRunning){
    SDL_PollEvent(&event);
    switch(event.type)
    {
    case SDL_QUIT:
        isRunning = 0; break;
    case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
        case SDLK_h:
            tileList[Global::curTileID].handleInput(0, isRunning); break;
        case SDLK_j:
            tileList[Global::curTileID].handleInput(1, isRunning); break;
        case SDLK_k:
            tileList[Global::curTileID].handleInput(2, isRunning); break;
        case SDLK_l:
            tileList[Global::curTileID].handleInput(3, isRunning); break;
        default: break;
        } break;
    default: break;
    }
}

void update(bool& isRunning){
    //camera.update();
    for (int i = 0; i < tileCount; i++)
        tileList[i].update(isRunning);
}

void clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(Global::renderer);
    SDL_Quit();
    cout << "Game clean!";
}
