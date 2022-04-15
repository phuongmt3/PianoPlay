#include "Game.h"
#include "OtherFunctionForGameCpp.h"

void Game::init(const char* title){
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << "Init completed!\n";

        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        ratio = DM.h / double(WINDOW_HEIGHT);

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH * ratio, WINDOW_HEIGHT * ratio, 0);
        if (window) {
            cout << "Created window!\n";
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
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

    ifstream fin("PianoPlay/pianoHub/highscore.txt");
    fin >> highScore;
    setBlocks(this);

    camera.stop = 1; waitingTimeForAQuarterNote = round(ratio * waitingTimeForAQuarterNote);
    fail = 0; menu.isShown = 1;

    gameBg = TextureManager::takeTexture("PianoPlay/pianoHub/piano.png", renderer);
    bg = TextureManager::takeTexture("PianoPlay/pianoHub/pianobg.png", renderer);
    srcRGame = { 0,0,1080,2052 }; 
    desRGame = { int(WINDOW_WIDTH * ratio / 2 - GAME_WIDTH * ratio / 2),
                int(WINDOW_HEIGHT * ratio / 2 - GAME_HEIGHT * ratio / 2),
                int(GAME_WIDTH * ratio), int(GAME_HEIGHT * ratio) };
    srcRBg = { 0,0,1000,900 }; 
    desRBg = { 0,0,int(WINDOW_WIDTH * ratio), int(WINDOW_HEIGHT * ratio) };

    AudioManager::winnerChunk = Mix_LoadWAV("PianoPlay/pianoHub/piano-mp3/mixkit-male-voice-cheer-2010.wav");
    AudioManager::menuMusic = Mix_LoadMUS("PianoPlay/pianoHub/piano-mp3/Beautiful-Piano.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    Mix_PlayMusic(AudioManager::menuMusic, -1);

    addSound();
    addTile(rand() % songCnt, this);
}

void Game::render(){
    if (menu.isShown) {
        SDL_RenderClear(renderer);
        if (Mix_PausedMusic()) {
            Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
            Mix_PlayMusic(AudioManager::menuMusic, -1);
        }
        TextureManager::drawImage(bg, srcRBg, desRBg, this->renderer);
        menu.show(renderer);
        if (chooseSongPopUp.isShown)
            chooseSongPopUp.show(renderer);
        if (manual.isShown)
            manual.show(renderer);
        if (highScorePopUp.isShown)
            highScorePopUp.show(renderer);
        SDL_RenderPresent(renderer);
        return;
    }
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
    if (Mix_PlayingMusic())
        Mix_PauseMusic();
    TextureManager::drawImage(bg, srcRBg, desRBg, this->renderer);
    TextureManager::drawImage(gameBg, srcRGame, desRGame, this->renderer);
    showTile(this);
    scoreTxt.show(renderer); highScoreTxt.show(renderer); 
    speedTxt.show(); autoPlay.show();
    if (speedPopUp.isShown)
        speedPopUp.show(renderer);
    if (fail) 
        failPopUp.show(renderer);
    if (chooseSongPopUp.isShown) 
        chooseSongPopUp.show(renderer);
    SDL_RenderPresent(renderer);
}

void Game::exit() {
    curTileID = lastSeenID = 0;
    score = 0; scoreTxt.update(this);
    chooseSongPopUp.isShown = speedPopUp.isShown = fail = autoPlay.isShown = 0;
    camera.stop = 1; camera.speed = 1.5;
    for (int i = 0; i < 5; i++) {
        if (i == 2) {
            speedPopUp.container[i].setColor(white);
            speedPopUp.container[i].setText(-1, black);
        }
        else 
            speedPopUp.container[i].setColor(lightGrey),
            speedPopUp.container[i].setText(-1, white);
    }
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
        if (menu.isShown) {
            menuHandle(x, y, mouseMotion, this);
            highScorePopUpHandle(x, y, mouseMotion, this);
            chooseSong(x, y, mouseMotion, this, event);
            break;
        }
        chooseSpeed(x, y, mouseMotion, this);
        chooseSongButton(x, y, mouseMotion, this);
        chooseSong(x, y, mouseMotion, this, event);
        autoPlayHandle(x, y, mouseMotion, this);
    }break;
    case SDL_MOUSEBUTTONDOWN:
    {
        int x, y; SDL_GetMouseState(&x, &y);
        if (menu.isShown) {
            chooseSong(x, y, mouseDown, this, event);
            manualHandle(x, y, mouseDown, this);
            highScorePopUpHandle(x, y, mouseDown, this);
            break;
        }
        chooseSpeed(x, y, mouseDown, this);
        chooseSong(x, y, mouseDown, this, event);
    }break;
    case SDL_MOUSEBUTTONUP:
    {
        int x, y; SDL_GetMouseState(&x, &y);
        if (menu.isShown) {
            menuHandle(x, y, mouseUp, this);
            highScorePopUpHandle(x, y, mouseUp, this);
            chooseSong(x, y, mouseUp, this, event);
            break;
        }
        chooseSpeed(x, y, mouseUp, this);
        chooseSong(x, y, mouseUp, this, event);
        autoPlayHandle(x, y, mouseUp, this);
    }break;
    case SDL_MOUSEWHEEL:
    {
        int x, y; SDL_GetMouseState(&x, &y);
        chooseSong(x, y, mouseWheel, this, event);
    }break;
    case SDL_KEYDOWN:
    {
        if (!fail && !menu.isShown){
            switch(event.key.keysym.sym)
            {
                case SDLK_f:
                    if (!camera.stop)
                        tileList[curTileID].handleInput(0, this);
                    break;
                case SDLK_g:
                    if (!camera.stop)
                        tileList[curTileID].handleInput(1, this);
                    break;
                case SDLK_h:
                    if (!camera.stop)
                        tileList[curTileID].handleInput(2, this);
                    break;
                case SDLK_j:
                    if (!camera.stop)
                        tileList[curTileID].handleInput(3, this);
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
                    menu.isShown = 1; exit(); break;
                default: break;
            }
        }
        else{
            switch(event.key.keysym.sym)
            {
                case SDLK_SPACE:
                    if (fail)
                        fail = 0; 
                break;
                case SDLK_ESCAPE:
                    menu.isShown = 1; exit(); break;
                default: break;
            }
        }
    }break;
    default: break;
    }
    if (autoPlay.isShown && curTileID < tileList.size())
        tileList[curTileID].handleInput(3, this);
}

void Game::update(){
    if (menu.isShown)
        return;
    camera.update(ratio);
    int goback = 0;
    if (curTileID < tileList.size())
        goback = tileList[curTileID].desR.y + tileList[curTileID].desR.h;
    for (int i = lastSeenID; i < tileList.size(); i++)
        tileList[i].update(goback, i, this);
    while (lastSeenID > 0) {
        tileList.erase(tileList.begin());
        curTileID--;
        lastSeenID--;
    }
    if (tileList.size() > 0 && tileList[lastSeenID].desR.y > WINDOW_HEIGHT * ratio + 10 &&
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
