#include "Game.h"

Mix_Chunk* AudioManager::notesList[14][8];
Mix_Chunk* AudioManager::winnerChunk;

Uint32 Tile::curTick;

PopUp scoreTxt(20,20,200,250);
PopUp highScoreTxt(770,20,200,250);
PopUp failPopUp(300,300,400,300);
Block speedTxt, autoPlay;
PopUp speedPopUp(50,550,150,250);
PopUp chooseSongPopUp(300, 200, 400, 475);
bool showSpeedPopUp = 0, showChooseSong = 0;

int songCnt = 8;
string song[] = {
        "TwinkleTwinkleLittleStar",
        "MyAll_AyumiHamasaki",
        "YoruNiKakeru_Yoasobi",
        "ToLove'sEnd_Inuyasa",
        "3107_3",
        "ToiThayHoaVangTrenCoXanh",
        "DemonSlayer",
        "draft"
};

bool isChar(char c)
{
    if (c == ',' || c == '-' || c == '+' || c == '=')
        return 1;
    return 0;
}

void addTile(int songID, Game* game)
{
    game->curSongId = songID; songID = 6;
    ifstream fin("PianoPlay/pianoHub/" + song[songID] + ".txt");
    fin >> game->tileCount;
    string s;
    for (int i = 0; i < game->tileCount; i++){
        fin >> s;
        if (i > 0)
            game->tileList.push_back(Tile(GAME_WIDTH/4,GAME_HEIGHT/4,i, game->tileList[i - 1].takePos()));
        else
            game->tileList.push_back(Tile(GAME_WIDTH/4,GAME_HEIGHT/4,i,4));
        Tile* curTile = &game->tileList.back();
        int pos = 0, channel = 0;
        int consecutiveNotes = 0;
        for (int bass = 0; bass < 2; bass++){
            channel = 0;
            while (pos < int(s.length()) && s[pos] != ','){
                if (!isChar(s[pos])){
                    string note = "";
                    while (!isChar(s[pos]) && pos < int(s.length()))
                        note += s[pos], pos++;
                    curTile->setNote(note,channel, consecutiveNotes,bass);
                }
                else if (s[pos] == '-'){
                    consecutiveNotes += 2; consecutiveNotes = min(consecutiveNotes, 3);
                    pos++; channel = 0;
                }
                else if (s[pos] == '=') {
                    consecutiveNotes++;
                    pos++; channel = 0;
                }
                else if (s[pos] == '+'){
                    channel++; pos++;
                }
            }
            pos++; consecutiveNotes = 0;
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

void showTile(Game* game)
{
    for (int i = game->lastSeenID; i < game->tileList.size() &&
                                    i < game->lastSeenID + 5; i++)
        game->tileList[i].show(game->renderer);
}

bool inside(const int& x, const int& y, const SDL_Rect& rec)
{
    if (x < rec.x || x > rec.x + rec.w)
        return 0;
    if (y < rec.y || y > rec.y + rec.h)
        return 0;
    return 1;
}

void Game::init(const char* title, int xpos, int ypos,
              int width, int height, bool fullscreen){
    int flag = 0;
    if (fullscreen)
        flag = SDL_WINDOW_FULLSCREEN;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        cout << "Init completed!\n";
        window = SDL_CreateWindow(title,xpos,ypos,width,height,flag);
        if (window){
            cout << "Created window!\n";
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
        if (renderer)
            cout << "Created renderer\n";
        else cout << "Cannot create renderer\n";
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
            printf("%s", Mix_GetError());
        if (TTF_Init() == -1)
            printf("TTF_Init: %s\n", TTF_GetError());
        isRunning = 1;
    }
    else isRunning = 0;
    camera.stop = 1; fail = 0;
    gameBg = TextureManager::takeTexture("PianoPlay/pianoHub/piano.png", renderer);
    bg = TextureManager::takeTexture("PianoPlay/pianoHub/pianobg.png", renderer);
    AudioManager::winnerChunk = Mix_LoadWAV("PianoPlay/pianoHub/piano-mp3/mixkit-male-voice-cheer-2010.wav");
    addSound();
    addTile(rand() % songCnt, this);

    scoreTxt.setColor(transparent);
    scoreTxt.addBlock("",0,0,200,100,transparent,"Score",30,0,100,white,renderer);
    scoreTxt.addBlock("scoreOnlyNum",0,100,200,100,transparent,"0",20,0,150,white,renderer);

    highScoreTxt.setColor(transparent);
    highScoreTxt.addBlock("",0,0,200,100,transparent,"High Score",0,20,80,white, renderer);
    highScoreTxt.addBlock("scoreOnlyNum",0,100,200,100,transparent,"0",20,0,150,white, renderer);

    failPopUp.setColor(lightGrey);
    failPopUp.addBlock("failTitle",0,0,400,75,darkGrey,"You lose!",100,0,80,white, renderer);
    failPopUp.addBlock("score",0,75,400,75,transparent,"Your score: 0",80,0,70,white, renderer);
    failPopUp.addBlock("",0,140,400,75,transparent,"SPACE to play again",70,10,50,red, renderer);
    failPopUp.addBlock("",100,215,200,75,transparent,"Choose song",0,0,80,white, renderer);

    autoPlay = Block("", 795, 800, 160, 100, blueTranparent, "AutoPlay", 15, 0, 70, white, renderer);

    speedTxt = Block("",50,800,150,100,blueTranparent,"Speed",25,0,70,white, renderer);
    speedPopUp.setColor(transparent);
    speedPopUp.addBlock("",0,0,150,50,lightGrey,"0.5",10,0,60,white, renderer);
    speedPopUp.addBlock("",0,50,150,50,lightGrey,"1",10,0,60,white, renderer);
    speedPopUp.addBlock("",0,100,150,50,white,"1.5",10,0,60,black, renderer);
    speedPopUp.addBlock("",0,150,150,50,lightGrey,"2",10,0,60,white, renderer);
    speedPopUp.addBlock("",0,200,150,50,lightGrey,"2.5",10,0,60,white, renderer);

    chooseSongPopUp.setColor(darkGrey);
    chooseSongPopUp.addBlock("title", 0, 0, 400, 100, transparent, "Song List", 85, -5, 100, white, renderer);
    for (int i = 0; i < songCnt; i++)
        chooseSongPopUp.addBlock("", 0, 100 + i * 75, 400, 75, lightGrey, song[i], 10, 0, 60, white, renderer);
    chooseSongPopUp.limitMoveUp = 100;
}

void Game::render(){
    if (fail == 1){
        if (showWrongKey){
            SDL_SetRenderDrawColor(renderer,255,13,13,255);
            SDL_RenderFillRect(renderer, &wrongRect);
            SDL_RenderPresent(renderer);
            showWrongKey = 0;
        }
        SDL_Delay(3000);
        fail = 2;
    }
    SDL_RenderClear(renderer);
    SDL_Rect srcRGame = {0,0,1080,2052}, desRGame = {WINDOW_WIDTH/2 - GAME_WIDTH/2,
                                                    WINDOW_HEIGHT/2 - GAME_HEIGHT/2,
                                                    GAME_WIDTH,GAME_HEIGHT};
    SDL_Rect srcRBg = {0,0,1000,900}, desRBg = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
    TextureManager::drawImage(bg, srcRBg, desRBg, this->renderer);
    TextureManager::drawImage(gameBg, srcRGame, desRGame, this->renderer);
    showTile(this);
    scoreTxt.show(renderer); highScoreTxt.show(renderer); 
    speedTxt.show(); autoPlay.show();
    if (showSpeedPopUp)
        speedPopUp.show(renderer);
    if (fail) 
        failPopUp.show(renderer);
    if (showChooseSong) 
        chooseSongPopUp.show(renderer);
    SDL_RenderPresent(renderer);
}

void Game::handleInput(){
    SDL_Event event;
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

        if (fail && inside(x, y, failPopUp.container[3].bloR))
            failPopUp.container[3].setColor(white), failPopUp.container[3].setText(-1,black);
        else
            failPopUp.container[3].setColor(transparent), failPopUp.container[3].setText(-1, white);

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

        if (!isAutoPlay) {
            if (inside(x, y, autoPlay.bloR)) {
                autoPlay.setColor(white);
                autoPlay.setText(0, black);
            }
            else {
                autoPlay.setColor(blueTranparent);
                autoPlay.setText(0, white);
            }
        }
    }break;
    case SDL_MOUSEBUTTONDOWN:
    {
        int x, y; SDL_GetMouseState(&x, &y);
        if (showSpeedPopUp){
            for (int i = 0; i < 5; i++) {
                if (inside(x, y, speedPopUp.container[i].bloR)) {
                    camera.speed = 0.5 + 0.5 * i;
                    speedPopUp.container[i].setColor(white);
                    speedPopUp.container[i].setText(-1, black);
                }
                else if (inside(x, y, speedPopUp.desR))
                    speedPopUp.container[i].setColor(lightGrey),
                    speedPopUp.container[i].setText(-1, white);
            }
        }
        if (showChooseSong) {
            for (int i = 1; i <= songCnt; i++)
                if (chooseSongPopUp.visibleBlock(i) && inside(x, y, chooseSongPopUp.container[i].bloR)) {
                    tileList.clear();
                    addTile(i - 1, this);
                    curTileID = 0; lastSeenID = 0;
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

        if (fail && !showChooseSong && inside(x, y, failPopUp.container[3].bloR))
            showChooseSong = 1;
        else {
            showChooseSong = 0;
            int comeback = chooseSongPopUp.limitMoveUp - chooseSongPopUp.takeY_BasePopUp(1);
            if (comeback > 0)
                for (int i = 1; i <= songCnt; i++)
                    chooseSongPopUp.container[i].changePos(0, comeback);
        }

        if (inside(x, y, autoPlay.bloR)) {
            if (!isAutoPlay) {
                isAutoPlay = 1;
                autoPlay.setColor(black);
                autoPlay.setText(0, red);
            }
            else {
                isAutoPlay = 0; camera.stop = 1;
                autoPlay.setColor(white);
                autoPlay.setText(0, black);
            }
        }
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
                    if (!camera.stop)
                        tileList[curTileID].handleInput(0, fail, scoreTxt, highScoreTxt, failPopUp, this);
                    break;
                case SDLK_g:
                    if (!camera.stop)
                        tileList[curTileID].handleInput(1, fail, scoreTxt, highScoreTxt, failPopUp, this);
                    break;
                case SDLK_h:
                    if (!camera.stop)
                        tileList[curTileID].handleInput(2, fail, scoreTxt, highScoreTxt, failPopUp, this);
                    break;
                case SDLK_j:
                    if (!camera.stop)
                        tileList[curTileID].handleInput(3, fail, scoreTxt, highScoreTxt, failPopUp, this);
                    break;
                case SDLK_SPACE:
                {
                    if (camera.stop)
                        camera.stop = 0;
                    else
                        camera.stop = 1;
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
    if (isAutoPlay && curTileID < tileList.size())
        tileList[curTileID].handleInput(3, fail, scoreTxt, highScoreTxt, failPopUp, this);
}

void Game::update(){
    camera.update();
    int goback = 0;
    if (curTileID < tileList.size())
        goback = tileList[curTileID].desR.y + tileList[curTileID].desR.h;
    for (int i = lastSeenID; i < tileList.size(); i++)
        tileList[i].update(fail, goback, scoreTxt, highScoreTxt, failPopUp, i, this);
    while (lastSeenID > 0) {
        tileList.erase(tileList.begin());
        curTileID--;
        lastSeenID--;
    }
    if (tileList.size() > 0 && tileList[lastSeenID].desR.y > WINDOW_HEIGHT &&
        tileList[lastSeenID].hadTouched()) {
        tileList.erase(tileList.begin());
        curTileID--;
    }
    if (tileList.empty()){
        Mix_PlayChannel(6, AudioManager::winnerChunk, 0);
        SDL_Delay(2000);
        int nextID = rand() % songCnt;
        nextID == curSongId ? addTile((nextID + 1) % songCnt, this) : addTile(nextID, this);
        curTileID = 0; lastSeenID = 0;
    }
}

void Game::clean(){
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "Game clean!";
}
