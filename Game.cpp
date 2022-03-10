#include "Game.h"

SDL_Window* window;
SDL_Renderer* Global::renderer;
SDL_Event event;//why event cannot be a pointer
SDL_Texture *Global::click, *Global::unclick, *Global::bg;
Camera Global::camera;
Mix_Chunk* AudioManager::notesList[14][8];
Mix_Chunk* AudioManager::winnerChunk;

vector<Tile> tileList;
int Global::tileCount;
int Global::curTileID = 0;
int Global::waitingTimeForSecondNote = 120;

bool isChar(char c)
{
    if (c == ',' || c == '-' || c == '+')
        return 1;
    return 0;
}

void addTile()//a,b: a with b; a-b: a before b
{
    ifstream fin("PianoPlay/pianoHub/MyAll_AyumiHamasaki.txt");
    fin >> Global::tileCount;
    //Global::tileCount = 5;
    string s;
    for (int i = 0; i < Global::tileCount; i++){
        fin >> s;
        if (i > 0)
            tileList.push_back(Tile(150,200,i,tileList[i - 1].takePos()));
        else
            tileList.push_back(Tile(150,200,i,4));
        Tile* curTile = &tileList.back();
        int pos = 0, channel = 0;
        bool isSecond = 0;//2 note lien tiep
        for (int bass = 0; bass < 2; bass++){
            channel = 0;
            while (s[pos] != ',' && pos < int(s.length())){
                if (!isChar(s[pos])){
                    string note = "";
                    while (!isChar(s[pos]) && pos < int(s.length()))
                        note += s[pos], pos++;
                    curTile->setNote(note,channel,isSecond,bass);
                    isSecond = 0;
                }
                else if (s[pos] == '-'){
                    isSecond = 1; pos++;
                }
                else if (s[pos] == '+'){//currently +/- only
                    channel++; pos++;
                }
            }
            pos++;
        }
    }
}

void addNote()
{
    for (int chu = 0; chu < 14; chu++)
        for (int so = 0; so < 8; so++){
            string s = "";
            if (chu < 7)
                s = s + char(chu + 'A') + char(so + '0');
            else
                s = s + char(chu + 'A' - 7) + 'b' + char(so + '0');
            AudioManager::addNote(s);
        }
}

void showTile()
{
    for (int i = 0; i < Global::tileCount; i++)
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
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
            printf("%s", Mix_GetError());
        isRunning = 1;
    }
    else isRunning = 0;
    Global::click = TextureManager::takeTexture("PianoPlay/pianoHub/tileTouched.png");
    Global::unclick = TextureManager::takeTexture("PianoPlay/pianoHub/tileUntouched.png");
    Global::bg = TextureManager::takeTexture("PianoPlay/pianoHub/piano.png");
    AudioManager::winnerChunk = Mix_LoadWAV("PianoPlay/pianoHub/piano-mp3/mixkit-male-voice-cheer-2010.wav");
    addNote();
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
        case SDLK_f:
            tileList[Global::curTileID].handleInput(0, isRunning); break;
        case SDLK_g:
            tileList[Global::curTileID].handleInput(1, isRunning); break;
        case SDLK_h:
            tileList[Global::curTileID].handleInput(2, isRunning); break;
        case SDLK_j:
            tileList[Global::curTileID].handleInput(3, isRunning); break;
        default: break;
        } break;
    default: break;
    }
}

void update(bool& isRunning){
    Global::camera.update();
    for (int i = 0; i < Global::tileCount; i++)
        tileList[i].update(isRunning);
    if (Global::curTileID == Global::tileCount
        && tileList[Global::curTileID - 1].desR.y > WINDOW_HEIGHT){
        isRunning = 0; cout << "You are winner!\n";
        Mix_PlayChannel(6, AudioManager::winnerChunk, 0);
    }
}

void clean(){
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(Global::renderer);
    SDL_Quit();
    cout << "Game clean!";
}
