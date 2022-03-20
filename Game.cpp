#include "Game.h"

SDL_Window* window;
SDL_Renderer* Global::renderer;
SDL_Event event;//why event cannot be a pointer
SDL_Texture *Global::bg, *Global::gameBg;
Camera Global::camera;
Mix_Chunk* AudioManager::notesList[14][8];
Mix_Chunk* AudioManager::winnerChunk;

vector<Tile> tileList;
SDL_Rect Global::wrongRect;
bool Global::showWrongKey = 0;
int Global::tileCount;
int Global::curTileID = 0, Global::lastSeenID = 0, Global::score = 0;
int Global::waitingTimeForSecondNote = 180;
Uint32 Tile::curTick;

Text SCORETxt, scoreTxt;
PopUp failPopUp(300,300,400,300);
int songCnt = 1, curSongId;
string song[] = {
        "PianoPlay/pianoHub/TwinkleTwinkleLittleStar.txt",
        "PianoPlay/pianoHub/MyAll_AyumiHamasaki.txt",
        "PianoPlay/pianoHub/YoruNiKakeru_Yoasobi.txt",
        "PianoPlay/pianoHub/ToLove'sEnd_Inuyasa.txt",
        "PianoPlay/pianoHub/draft.txt"};

bool isChar(char c)
{
    if (c == ',' || c == '-' || c == '+')
        return 1;
    return 0;
}

void addTile(int songID)
{
    if (curSongId == songID)
        songID = (songID + 1) % songCnt;
    curSongId = songID;
    ifstream fin(song[songID]);
    fin >> Global::tileCount;
    //Global::tileCount = 5;
    string s;
    for (int i = 0; i < Global::tileCount; i++){
        fin >> s;
        if (i > 0)
            tileList.push_back(Tile(GAME_WIDTH/4,GAME_HEIGHT/4,i,tileList[i - 1].takePos()));
        else
            tileList.push_back(Tile(GAME_WIDTH/4,GAME_HEIGHT/4,i,4));
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
                }
                else if (s[pos] == '-'){
                    isSecond = 1; pos++; channel = 0;
                }
                else if (s[pos] == '+'){
                    channel++; pos++;
                }
            }
            pos++; isSecond = 0;
        }
    }
}

void addSound()
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
    for (int i = Global::lastSeenID; i < Global::tileCount &&
                                    i < Global::lastSeenID + 5; i++)
        tileList[i].show();
}

void init(const char* title, int xpos, int ypos,
              int width, int height, bool fullscreen, bool& isRunning, int& fail){
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
        SDL_SetRenderDrawBlendMode(Global::renderer,SDL_BLENDMODE_BLEND);
        if (Global::renderer)
            cout << "Created renderer\n";
        else cout << "Cannot create renderer\n";
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
            printf("%s", Mix_GetError());
        if (TTF_Init() == -1)
            printf("TTF_Init: %s\n", TTF_GetError());
        isRunning = 1;
    }
    else isRunning = 0;
    Global::camera.stop = 1; fail = 0;
    Global::gameBg = TextureManager::takeTexture("PianoPlay/pianoHub/piano.png");
    Global::bg = TextureManager::takeTexture("PianoPlay/pianoHub/pianobg.png");
    AudioManager::winnerChunk = Mix_LoadWAV("PianoPlay/pianoHub/piano-mp3/mixkit-male-voice-cheer-2010.wav");
    addSound();
    addTile(rand() % songCnt);
    SCORETxt = Text("Score",20,20,200,100);
    string test = "0";
    scoreTxt = Text(test,20,150,200/4*(int)test.length(),100);
    failPopUp.addBlock("",0,0,400,75,"You lose!",50,0,300,75);
    failPopUp.addBlock("score",0,75,400,75,"Score: 0",75,15,250,50);
}

void render(int& fail){
    if (fail == 1){
        if (Global::showWrongKey){
            SDL_SetRenderDrawColor(Global::renderer,255,13,13,255);
            SDL_RenderFillRect(Global::renderer, &Global::wrongRect);
            SDL_RenderPresent(Global::renderer);
            Global::showWrongKey = 0;
        }
        SDL_Delay(3000);
        fail = 2;
    }
    SDL_RenderClear(Global::renderer);
    SDL_Rect srcRGame = {0,0,1080,2052}, desRGame = {WINDOW_WIDTH/2 - GAME_WIDTH/2,
                                                    WINDOW_HEIGHT/2 - GAME_HEIGHT/2,
                                                    GAME_WIDTH,GAME_HEIGHT};
    SDL_Rect srcRBg = {0,0,1000,900}, desRBg = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
    TextureManager::drawImage(Global::bg, srcRBg, desRBg);
    TextureManager::drawImage(Global::gameBg, srcRGame, desRGame);
    showTile();
    SCORETxt.show(); scoreTxt.show();
    if (fail)
        failPopUp.show();
    SDL_RenderPresent(Global::renderer);
}

void handleInput(bool& isRunning, int& fail){
    SDL_PollEvent(&event);
    switch(event.type)
    {
    case SDL_QUIT:
        isRunning = 0; break;
    case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
        case SDLK_f:
            if (!fail && !Global::camera.stop)
                tileList[Global::curTileID].handleInput(0, fail, scoreTxt, failPopUp);
            break;
        case SDLK_g:
            if (!fail && !Global::camera.stop)
                tileList[Global::curTileID].handleInput(1, fail, scoreTxt, failPopUp);
            break;
        case SDLK_h:
            if (!fail && !Global::camera.stop)
                tileList[Global::curTileID].handleInput(2, fail, scoreTxt, failPopUp);
            break;
        case SDLK_j:
            if (!fail && !Global::camera.stop)
                tileList[Global::curTileID].handleInput(3, fail, scoreTxt, failPopUp);
            break;
        case SDLK_SPACE:
        {
            if (Global::camera.stop && !fail)
                Global::camera.stop = 0;
            else
                Global::camera.stop = 1;
            if (fail)
                fail = 0;
        }
        break;
        case SDLK_ESCAPE:
            isRunning = 0; break;
        default: break;
        } break;
    default: break;
    }
    //tileList[Global::curTileID].handleInput(3, fail, scoreTxt, failPopUp);
}

void update(bool& isRunning, int& fail){
    Global::camera.update();
    int goback = tileList[Global::curTileID].desR.y +
                    tileList[Global::curTileID].desR.h;
    for (int i = Global::lastSeenID; i < Global::tileCount; i++)
        tileList[i].update(fail, goback, scoreTxt, failPopUp);
    if (tileList[Global::lastSeenID].desR.y > WINDOW_HEIGHT &&
        tileList[Global::lastSeenID].hadTouched())
        Global::lastSeenID++;
    if (Global::lastSeenID >= Global::tileCount){
        Mix_PlayChannel(6, AudioManager::winnerChunk, 0);
        SDL_Delay(1000);
        tileList.clear();
        addTile(rand() % songCnt);
        Global::curTileID = 0; Global::lastSeenID = 0;
    }
}

void clean(){
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(Global::renderer);
    SDL_Quit();
    cout << "Game clean!";
}
