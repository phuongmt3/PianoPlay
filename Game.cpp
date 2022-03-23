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
int Global::curTileID, Global::lastSeenID, Global::score, Global::highScore;
int Global::waitingTimeForSecondNote = 180;
Uint32 Tile::curTick;

PopUp scoreTxt(20,20,200,250);
PopUp highScoreTxt(770,20,200,250);
PopUp failPopUp(300,300,400,300);
Block speedTxt;
PopUp speedPopUp(0,550,200,250);
PopUp chooseSongPopUp(300, 200, 400, 475);
bool showSpeedPopUp = 0, showChooseSong = 0;
int songCnt = 6, curSongId;

string song[] = {
        "TwinkleTwinkleLittleStar",
        "MyAll_AyumiHamasaki",
        "YoruNiKakeru_Yoasobi",
        "ToLove'sEnd_Inuyasa",
        "3107_3",
        "draft"
};

bool isChar(char c)
{
    if (c == ',' || c == '-' || c == '+')
        return 1;
    return 0;
}

void addTile(int songID)
{
    curSongId = songID;
    ifstream fin("PianoPlay/pianoHub/" + song[songID] + ".txt");
    fin >> Global::tileCount;
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
            while (pos < int(s.length()) && s[pos] != ','){
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

bool inside(const int& x, const int& y, const SDL_Rect& rec)
{
    if (x < rec.x || x > rec.x + rec.w)
        return 0;
    if (y < rec.y || y > rec.y + rec.h)
        return 0;
    return 1;
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

    scoreTxt.setColor(transparent);
    scoreTxt.addBlock("",0,0,200,100,transparent,"Score",0,0,100,white);
    scoreTxt.addBlock("scoreOnlyNum",0,100,200,100,transparent,"0",0,0,150,white);

    highScoreTxt.setColor(transparent);
    highScoreTxt.addBlock("",0,0,200,100,transparent,"High Score",0,0,80,white);
    highScoreTxt.addBlock("scoreOnlyNum",0,100,200,100,transparent,"0",0,0,150,white);

    failPopUp.setColor(lightGrey);
    failPopUp.addBlock("failTitle",0,0,400,75,darkGrey,"You lose!",50,0,80,white);
    failPopUp.addBlock("score",0,75,400,75,transparent,"Score: 0",75,0,80,white);
    failPopUp.addBlock("",0,200,200,75,transparent,"Choose song",10,0,80,white);

    speedTxt = Block("",20,800,200,100,blueTranparent,"Speed",0,0,70,white);
    speedPopUp.setColor(transparent);
    speedPopUp.addBlock("",0,0,200,50,lightGrey,"0.5",10,0,60,white);
    speedPopUp.addBlock("",0,50,200,50,lightGrey,"1",10,0,60,white);
    speedPopUp.addBlock("",0,100,200,50,white,"1.5",10,0,60,black);
    speedPopUp.addBlock("",0,150,200,50,lightGrey,"2",10,0,60,white);
    speedPopUp.addBlock("",0,200,200,50,lightGrey,"2.5",10,0,60,white);

    chooseSongPopUp.setColor(darkGrey);
    chooseSongPopUp.addBlock("title", 0, 0, 400, 100, transparent, "Song List", 75, 0, 100, white);
    for (int i = 0; i < songCnt; i++)
        chooseSongPopUp.addBlock("", 0, 100 + i * 75, 400, 75, lightGrey, song[i], 10, 0, 65, white);
    chooseSongPopUp.limitMoveUp = 100;
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
    scoreTxt.show(); highScoreTxt.show(); speedTxt.show();
    if (showSpeedPopUp)
        speedPopUp.show();
    if (fail) 
        failPopUp.show();
    if (showChooseSong) 
        chooseSongPopUp.show();
    SDL_RenderPresent(Global::renderer);
}

void handleInput(bool& isRunning, int& fail){
    SDL_PollEvent(&event);
    switch(event.type)
    {
    case SDL_QUIT:
        isRunning = 0; break;
    case SDL_MOUSEMOTION:
    {
        int x, y; SDL_GetMouseState(&x, &y);
        if (inside(x, y, speedTxt.bloR))
            speedTxt.setColor(white), speedTxt.setText(-1, black);
        else
            speedTxt.setColor(blueTranparent), speedTxt.setText(-1, white);

        if (fail && inside(x, y, failPopUp.container[2].bloR))
            failPopUp.container[2].setColor(white), failPopUp.container[2].setText(-1,black);
        else
            failPopUp.container[2].setColor(transparent), failPopUp.container[2].setText(-1, white);

        if (showChooseSong) {
            for (int i = 1; i <= songCnt; i++) {
                if (chooseSongPopUp.visibleBlock(i) && inside(x, y, chooseSongPopUp.container[i].bloR)) {
                    chooseSongPopUp.container[i].setColor(white);
                    chooseSongPopUp.container[i].setText(-1, black);
                }
                else {
                    chooseSongPopUp.container[i].setColor(lightGrey);
                    chooseSongPopUp.container[i].setText(-1, white);
                }

            }
        }
    }break;
    case SDL_MOUSEBUTTONDOWN:
    {
        int x, y; SDL_GetMouseState(&x, &y);
        if (showSpeedPopUp){
            for (int i = 0; i < 5; i++) {
                if (inside(x, y, speedPopUp.container[i].bloR)) {
                    Global::camera.speed = 0.5 + 0.5 * i;
                    speedPopUp.container[i].setColor(white);
                    speedPopUp.container[i].setText(-1, black);
                }
                else
                    speedPopUp.container[i].setColor(lightGrey),
                    speedPopUp.container[i].setText(-1, white);
            }
        }
        if (showChooseSong) {
            for (int i = 1; i <= songCnt; i++)
                if (chooseSongPopUp.visibleBlock(i) && inside(x, y, chooseSongPopUp.container[i].bloR)) {
                    tileList.clear();
                    addTile(i - 1);
                    Global::curTileID = 0; Global::lastSeenID = 0;
                }
        }
    }break;
    case SDL_MOUSEBUTTONUP:
    {
        int x, y; SDL_GetMouseState(&x, &y);
        if (inside(x, y, speedTxt.bloR))
            showSpeedPopUp = 1;
        else
            showSpeedPopUp = 0;
        if (fail && inside(x, y, failPopUp.container[2].bloR))
            showChooseSong = 1;
        else
            showChooseSong = 0;
    }break;
    case SDL_MOUSEWHEEL:
    {
        int x, y; SDL_GetMouseState(&x, &y);
        if (inside(x, y, chooseSongPopUp.desR)) {
            if (event.wheel.y < 0 && chooseSongPopUp.takeY_BasePopUp(songCnt) +
                chooseSongPopUp.container[songCnt].bloR.h != chooseSongPopUp.limitMoveDown) {
                for (int i = 1; i <= songCnt; i++)
                    chooseSongPopUp.container[i].changePos(0, -chooseSongPopUp.container[i].bloR.h);
            }
            else if (event.wheel.y > 0 && chooseSongPopUp.takeY_BasePopUp(1) != chooseSongPopUp.limitMoveUp) {
                for (int i = 1; i <= songCnt; i++)
                    chooseSongPopUp.container[i].changePos(0, chooseSongPopUp.container[i].bloR.h);
            }
        }
    }break;
    case SDL_KEYDOWN:
    {
        if (!fail){
            switch(event.key.keysym.sym)
            {
                case SDLK_f:
                    if (!Global::camera.stop)
                        tileList[Global::curTileID].handleInput(0, fail, scoreTxt, highScoreTxt, failPopUp);
                    break;
                case SDLK_g:
                    if (!Global::camera.stop)
                        tileList[Global::curTileID].handleInput(1, fail, scoreTxt, highScoreTxt, failPopUp);
                    break;
                case SDLK_h:
                    if (!Global::camera.stop)
                        tileList[Global::curTileID].handleInput(2, fail, scoreTxt, highScoreTxt, failPopUp);
                    break;
                case SDLK_j:
                    if (!Global::camera.stop)
                        tileList[Global::curTileID].handleInput(3, fail, scoreTxt, highScoreTxt, failPopUp);
                    break;
                case SDLK_SPACE:
                {
                    if (Global::camera.stop)
                        Global::camera.stop = 0;
                    else
                        Global::camera.stop = 1;
                }
                break;
                case SDLK_ESCAPE:
                    isRunning = 0; break;
                default: break;
            }
        }
        else{
            switch(event.key.keysym.sym)
            {
            case SDLK_SPACE:
                fail = 0; break;
            case SDLK_ESCAPE:
                isRunning = 0; break;
            default: break;
            }
        }
    }break;
    default: break;
    }
    //tileList[Global::curTileID].handleInput(3, fail, scoreTxt, highScoreTxt, failPopUp);
}

void update(bool& isRunning, int& fail){
    Global::camera.update();
    int goback = 0;
    if (Global::curTileID < Global::tileCount)
        goback = tileList[Global::curTileID].desR.y + tileList[Global::curTileID].desR.h;
    for (int i = Global::lastSeenID; i < Global::tileCount; i++)
        tileList[i].update(fail, goback, scoreTxt, highScoreTxt, failPopUp);

    if (tileList[Global::lastSeenID].desR.y > WINDOW_HEIGHT &&
        tileList[Global::lastSeenID].hadTouched())
        Global::lastSeenID++;
    if (Global::lastSeenID >= Global::tileCount){
        Mix_PlayChannel(6, AudioManager::winnerChunk, 0);
        SDL_Delay(2000);
        tileList.clear();
        int nextID = rand() % songCnt;
        nextID == curSongId ? addTile((nextID + 1) % songCnt) : addTile(nextID);
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
