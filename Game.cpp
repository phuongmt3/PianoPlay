#include "Game.h"
#include "OtherFunctionForGameCpp.h"

void Game::init(const char* title){
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        cout << "Init completed!\n";
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        ratio = DM.h / double(WINDOW_HEIGHT); //ratio = 1;
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH * ratio,WINDOW_HEIGHT * ratio,0);
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

    scoreTxt = PopUp(20, 20, 200, 300, ratio);
    highScoreTxt = PopUp(770, 20, 200, 300, ratio);
    failPopUp = PopUp(300, 300, 400, 300, ratio);
    speedPopUp = PopUp(50, 550, 150, 253, ratio);
    chooseSongPopUp = PopUp(300, 200, 400, 480, ratio);
    menu = PopUp(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, ratio);
    manual = PopUp(150, 200, 700, 600, ratio);
    highScorePopUp = PopUp(300, 200, 400, 400, ratio);

    ifstream fin("PianoPlay/pianoHub/highscore.txt");
    fin >> highScore;
    camera.stop = 1; waitingTimeForAQuarterNote = round(ratio * waitingTimeForAQuarterNote);
    fail = 0; menu.isShown = 1;
    gameBg = TextureManager::takeTexture("PianoPlay/pianoHub/piano.png", renderer);
    bg = TextureManager::takeTexture("PianoPlay/pianoHub/pianobg.png", renderer);
    AudioManager::winnerChunk = Mix_LoadWAV("PianoPlay/pianoHub/piano-mp3/mixkit-male-voice-cheer-2010.wav");
    addSound();
    addTile(rand() % songCnt, this);

    menu.setColor(transparent);
    menu.addBlock("title", 300, 30, 300, 100, transparent, "Piano Tiles", 0, 0, 140, 0, white, renderer);
    menu.addBlock("title", 250, 170, 500, 100, transparent, "Anyone can be a piano pro...", 0, 0, 60, 1, white, renderer);
    menu.addBlock("", 330, 300, 300, 100, blueTranparent, "Play", 100, 0, 80, 0, white, renderer);
    menu.addBlock("", 330, 400, 300, 100, blueTranparent, "Choose Song", 40, 0, 80, 0, white, renderer);
    menu.addBlock("", 330, 500, 300, 100, blueTranparent, "High Score", 50, 0, 80, 0, white, renderer);
    menu.addBlock("", 330, 600, 300, 100, blueTranparent, "Help", 100, 0, 80, 0, white, renderer);
    menu.addBlock("", 330, 700, 300, 100, blueTranparent, "Exit", 100, 0, 80, 0, white, renderer);

    manual.setColor(lightGrey);
    manual.addBlock("", 0, 0, 700, 100, darkGrey, "User manual", 200, 0, 100, 0, white, renderer);
    manual.addBlock("", 0, 100, 700, 50, transparent, "You just got stucked then now come to see me, right?", 10, 0, 40, 2, white, renderer);
    manual.addBlock("", 0, 150, 700, 50, transparent, "Anyway, I will forgive you for that...", 10, 0, 40, 2, white, renderer);
    manual.addBlock("", 0, 230, 700, 50, transparent, "Space to start, pause and consume game.", 10, 0, 40, 2, white, renderer);
    manual.addBlock("", 0, 280, 700, 50, transparent, "Press F, G, H, J conrresponding to postion of tiles.", 10, 0, 40, 2, white, renderer);
    manual.addBlock("", 0, 330, 700, 50, transparent, "Default speed is 1.5, but you can change it based on your", 10, 0, 40, 2, white, renderer);
    manual.addBlock("", 0, 380, 700, 50, transparent, " reference.", 10, 0, 40, 2, white, renderer);
    manual.addBlock("", 0, 430, 700, 50, transparent, "Choose AutoPlay to listen to music without playing, but", 10, 0, 40, 2, white, renderer);
    manual.addBlock("", 0, 480, 700, 50, transparent, " you won't get any score.", 10, 0, 40, 2, white, renderer);
    manual.addBlock("", 0, 530, 700, 50, transparent, "The last thing, press Esc to back to menu.", 10, 0, 40, 2, white, renderer);

    highScorePopUp.setColor(lightGrey);
    string bestScoreLine = "Best Score: " + to_string(highScore);
    highScorePopUp.addBlock("", 0, 0, 400, 100, darkGrey, "Victory", 100, 0, 100, 0, white, renderer);
    highScorePopUp.addBlock("", 0, 125, 400, 100, transparent, bestScoreLine, 30, 0, 60, 1, white, renderer);
    highScorePopUp.addBlock("", 0, 200, 400, 100, blueTranparent, "Reset High Score", 40, 0, 80, 0, white, renderer);

    scoreTxt.setColor(transparent);
    scoreTxt.addBlock("",0,0,200,100,transparent,"Score",30,0,100,0,white,renderer);
    scoreTxt.addBlock("scoreOnlyNum",0,120,200,100,transparent,"0",15,0,100,1,white,renderer);

    highScoreTxt.setColor(transparent);
    highScoreTxt.addBlock("",0,0,200,100,transparent,"High Score",0,20,80,0,white, renderer);
    highScoreTxt.addBlock("scoreOnlyNum",0,120,200,100,transparent,to_string(highScore), 10, 0, 100, 1, white, renderer);

    failPopUp.setColor(lightGrey);
    failPopUp.addBlock("failTitle",0,0,400,75,darkGrey,"You lose!",100,0,80,0,white, renderer);
    failPopUp.addBlock("score",0,75,400,75,transparent,"Your score: 0",80,0,70,0,white, renderer);
    failPopUp.addBlock("",0,140,400,75,transparent,"SPACE to play again",70,10,50,0,yellow, renderer);
    failPopUp.addBlock("",90,215,220,75,transparent,"Choose song",10,0,80,0,white,renderer);

    autoPlay = Block("", 795, 800, 160, 100, blueTranparent, "AutoPlay", 15, 10, 70, 0, white, renderer, ratio);

    speedTxt = Block("",50,800,150,100,blueTranparent,"Speed",25,10,70,0,white, renderer, ratio);
    speedPopUp.setColor(lightGrey);
    speedPopUp.addBlock("",0,0,150,50,lightGrey,"0.5",10,0,60,0,white, renderer);
    speedPopUp.addBlock("",0,50,150,50,lightGrey,"1",10,0,60,0,white, renderer);
    speedPopUp.addBlock("",0,100,150,50,white,"1.5",10,0,60,0,black, renderer);
    speedPopUp.addBlock("",0,150,150,50,lightGrey,"2",10,0,60,0,white, renderer);
    speedPopUp.addBlock("",0,200,150,50,lightGrey,"2.5",10,0,60,0,white, renderer);

    chooseSongPopUp.setColor(lightGrey);
    chooseSongPopUp.addBlock("title", 0, 0, 400, 100, transparent, "Song List", 85, -5, 100, 0, white, renderer);
    for (int i = 0; i < songCnt; i++)
        chooseSongPopUp.addBlock("", 0, 100 + i * 75, 400, 75, darkGrey, song[i], 10, 10, 50, 1, white, renderer);
    chooseSongPopUp.setLimit(100, -1);
}

void Game::render(){
    if (menu.isShown) {
        SDL_RenderClear(renderer);
        SDL_Rect srcRBg = { 0,0,1000,900 }, desRBg = { 0,0,WINDOW_WIDTH * ratio,WINDOW_HEIGHT * ratio };
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
    SDL_Rect srcRGame = {0,0,1080,2052}, desRGame = {WINDOW_WIDTH * ratio /2 - GAME_WIDTH * ratio /2,
                                                    WINDOW_HEIGHT* ratio /2 - GAME_HEIGHT * ratio /2,
                                                    GAME_WIDTH* ratio,GAME_HEIGHT* ratio };
    SDL_Rect srcRBg = {0,0,1000,900}, desRBg = {0,0,WINDOW_WIDTH * ratio,WINDOW_HEIGHT * ratio };
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
                {
                    if (fail)
                        fail = 0; 
                }break;
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
