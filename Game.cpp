#include "Game.h"

Mix_Chunk* AudioManager::notesList[14][8];
Mix_Chunk* AudioManager::winnerChunk;

Uint32 Tile::curTick;

int cur1stSongInList = 1;//chuyen thanh static trong Game

enum sdlEvent {
    mouseMotion, mouseUp, mouseDown, mouseWheel
};

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
    game->curSongId = songID; //songID = 6;
    ifstream fin("PianoPlay/pianoHub/" + song[songID] + ".txt");
    fin >> game->tileCount;
    string s;
    game->tileList.clear();
    for (int i = 0; i < game->tileCount; i++){
        fin >> s;
        if (i > 0)
            game->tileList.push_back(Tile(GAME_WIDTH/4,GAME_HEIGHT/4,i, game->tileList[i - 1].takePos(),game->ratio));
        else
            game->tileList.push_back(Tile(GAME_WIDTH/4,GAME_HEIGHT/4,i,4,game->ratio));
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

void chooseSpeed(const int& x, const int& y, sdlEvent event, Game* game) {
    if (event == mouseMotion) {
        if (inside(x, y, game->speedTxt.bloR))
            game->speedTxt.setColor(white), game->speedTxt.setText(-1, black);
        else if (!game->speedPopUp.isShown)
            game->speedTxt.setColor(blueTranparent), game->speedTxt.setText(-1, white);
    }
    else if (event == mouseDown) {
        if (game->speedPopUp.isShown) {
            for (int i = 0; i < 5; i++) {
                if (inside(x, y, game->speedPopUp.container[i].bloR)) {
                    game->camera.speed = 0.5 + 0.5 * i;
                    game->speedPopUp.container[i].setColor(white);
                    game->speedPopUp.container[i].setText(-1, black);
                }
                else if (inside(x, y, game->speedPopUp.desR))
                    game->speedPopUp.container[i].setColor(lightGrey),
                    game->speedPopUp.container[i].setText(-1, white);
            }
        }
    }
    else if (event == mouseUp) {
        if (inside(x, y, game->speedTxt.bloR))
            game->speedPopUp.isShown = 1;
        else
            game->speedPopUp.isShown = 0;
    }
}

void chooseSong(const int& x, const int& y, sdlEvent event, Game* game, const SDL_Event& e) {
    if (event == mouseMotion) {
        if (game->chooseSongPopUp.isShown) {
            for (int i = 1; i <= songCnt; i++) {
                if (game->chooseSongPopUp.visibleBlock(i) && inside(x, y, game->chooseSongPopUp.container[i].bloR)) {
                    game->chooseSongPopUp.container[i].setColor(white);
                    game->chooseSongPopUp.container[i].setText(-1, black);
                }
                else {
                    game->chooseSongPopUp.container[i].setColor(darkGrey);
                    game->chooseSongPopUp.container[i].setText(-1, white);
                }
            }
        }
    }
    else if (event == mouseDown) {
        if (game->chooseSongPopUp.isShown) {
            for (int i = 1; i <= songCnt; i++)
                if (game->chooseSongPopUp.visibleBlock(i) && inside(x, y, game->chooseSongPopUp.container[i].bloR)) {
                    game->menu.isShown = 0;
                    game->tileList.clear();
                    addTile(i - 1, game);
                    game->curTileID = 0; game->lastSeenID = 0;
                }
            if (!inside(x, y, game->chooseSongPopUp.desR))
                game->chooseSongPopUp.isShown = 0;
        }
    }
    else if (event == mouseWheel) {
        if (inside(x, y, game->chooseSongPopUp.desR)) {
            if (e.wheel.y < 0 && cur1stSongInList + 4 < songCnt) {
                cur1stSongInList++;
                int step = game->chooseSongPopUp.takeY_BasePopUp(cur1stSongInList) - game->chooseSongPopUp.limitMoveUp();
                for (int i = 1; i <= songCnt; i++)
                    game->chooseSongPopUp.container[i].changePos(0, -step);
            }
            else if (e.wheel.y > 0 && cur1stSongInList > 1) {
                cur1stSongInList--;
                int step = game->chooseSongPopUp.limitMoveUp() - game->chooseSongPopUp.takeY_BasePopUp(cur1stSongInList);
                for (int i = 1; i <= songCnt; i++)
                    game->chooseSongPopUp.container[i].changePos(0, step);
            }
        }
    }
}

void autoPlayHandle(const int& x, const int& y, sdlEvent event, Game* game) {
    if (event == mouseMotion) {
        if (!game->autoPlay.isShown) {
            if (inside(x, y, game->autoPlay.bloR)) {
                game->autoPlay.setColor(white);
                game->autoPlay.setText(0, black);
            }
            else {
                game->autoPlay.setColor(blueTranparent);
                game->autoPlay.setText(0, white);
            }
        }
    }
    else if (event == mouseUp) {
        if (inside(x, y, game->autoPlay.bloR)) {
            if (!game->autoPlay.isShown) {
                game->autoPlay.isShown = 1;
                game->autoPlay.setColor(black);
                game->autoPlay.setText(0, red);
            }
            else {
                game->autoPlay.isShown = 0; game->camera.stop = 1;
                game->autoPlay.setColor(white);
                game->autoPlay.setText(0, black);
            }
        }
    }
}

void chooseSongButton(const int& x, const int& y, sdlEvent event, Game* game) {
    if (event == mouseMotion) {
        if (game->fail && inside(x, y, game->failPopUp.container[3].bloR))
            game->failPopUp.container[3].setColor(white), game->failPopUp.container[3].setText(-1, black);
        else
            game->failPopUp.container[3].setColor(transparent), game->failPopUp.container[3].setText(-1, white);
    }
    else if (event == mouseUp) {
        if (game->fail && !game->chooseSongPopUp.isShown && inside(x, y, game->failPopUp.container[3].bloR))
            game->chooseSongPopUp.isShown = 1;
        else {
            game->chooseSongPopUp.isShown = 0; cur1stSongInList = 1;
            int comeback = game->chooseSongPopUp.limitMoveUp() - game->chooseSongPopUp.takeY_BasePopUp(1);
            if (comeback > 0)
                for (int i = 1; i <= songCnt; i++)
                    game->chooseSongPopUp.container[i].changePos(0, comeback);
        }
    }
}

void menuHandle(const int& x, const int& y, sdlEvent event, Game* game) {
    if (event == mouseUp && !game->manual.isShown && !game->highScorePopUp.isShown) {
        if (inside(x, y, game->menu.container[2].bloR)) {
            game->menu.isShown = 0;
            int nextID = rand() % songCnt;
            nextID == game->curSongId ? addTile((nextID + 1) % songCnt, game) : addTile(nextID, game);
        }
        else if (inside(x, y, game->menu.container[3].bloR))
            game->chooseSongPopUp.isShown = 1;
        else if (inside(x, y, game->menu.container[4].bloR))
            game->highScorePopUp.isShown = 1;
        else if (inside(x, y, game->menu.container[5].bloR))
            game->manual.isShown = 1;
        else if (inside(x, y, game->menu.container[6].bloR))
            game->isRunning = 0;
    }
}

void manualHandle(const int& x, const int& y, sdlEvent event, Game* game) {
    if (event == mouseDown) {
        if (game->manual.isShown)
            if (!inside(x, y, game->manual.desR))
                game->manual.isShown = 0;
    }
}

void highScorePopUpHandle(const int& x, const int& y, sdlEvent event, Game* game) {
    if (event == mouseDown) {
        if (game->highScorePopUp.isShown)
            if (!inside(x, y, game->highScorePopUp.desR))
                game->highScorePopUp.isShown = 0;
    }
    else if (event == mouseUp)
        if (inside(x, y, game->highScorePopUp.container[2].bloR)) {
            game->highScore = 0;
            game->highScoreTxt.update(game);
            game->highScorePopUp.container[1].content.updateText("Best score: 0");
            ofstream fout("PianoPlay/pianoHub/highscore.txt");
            fout << game->highScore;
        }
}

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

    scoreTxt = PopUp(20, 20, 200, 250, ratio);
    highScoreTxt = PopUp(770, 20, 200, 250, ratio);
    failPopUp = PopUp(300, 300, 400, 300, ratio);
    speedPopUp = PopUp(50, 550, 150, 253, ratio);
    chooseSongPopUp = PopUp(300, 200, 400, 480, ratio);
    menu = PopUp(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, ratio);
    manual = PopUp(150, 200, 700, 600, ratio);
    highScorePopUp = PopUp(300, 200, 400, 500, ratio);

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
    menu.addBlock("title", 300, 30, 300, 100, transparent, "Piano Tiles", 0, 0, 100, white, renderer);
    menu.addBlock("title", 200, 130, 500, 100, transparent, "Anyone can be a piano pro...", 0, 0, 60, white, renderer);
    menu.addBlock("", 300, 300, 300, 100, blueTranparent, "Play", 0, 0, 80, white, renderer);
    menu.addBlock("", 300, 400, 300, 100, blueTranparent, "Choose Song", 0, 0, 80, white, renderer);
    menu.addBlock("", 300, 500, 300, 100, blueTranparent, "High Score", 0, 0, 80, white, renderer);
    menu.addBlock("", 300, 600, 300, 100, blueTranparent, "Help", 0, 0, 80, white, renderer);
    menu.addBlock("", 300, 700, 300, 100, blueTranparent, "Exit", 0, 0, 80, white, renderer);

    manual.setColor(lightGrey);
    manual.addBlock("", 0, 0, 700, 100, darkGrey, "User manual", 0, 0, 100, white, renderer);
    manual.addBlock("", 0, 100, 700, 50, transparent, "You just got stucked then now come to see me, right?", 0, 0, 60, white, renderer);
    manual.addBlock("", 0, 150, 700, 50, transparent, "Anyway, I will forgive you for that...", 0, 0, 60, white, renderer);
    manual.addBlock("", 0, 200, 700, 50, transparent, "Space to start, pause and consume game.", 0, 0, 60, white, renderer);
    manual.addBlock("", 0, 250, 700, 50, transparent, "Press F, G, H, J conrresponding to postion of tiles.", 0, 0, 60, white, renderer);
    manual.addBlock("", 0, 300, 700, 50, transparent, "Default speed is 1.5, but you can change it based on your reference.", 0, 0, 60, white, renderer);
    manual.addBlock("", 0, 350, 700, 50, transparent, "Choose AutoPlay to listen to music without playing, but you won't get score", 0, 0, 60, white, renderer);
    manual.addBlock("", 0, 400, 700, 50, transparent, "The last thing, press Esc to back to menu.", 0, 0, 60, white, renderer);

    highScorePopUp.setColor(lightGrey);
    string bestScoreLine = "Best Score: " + to_string(highScore);
    highScorePopUp.addBlock("", 0, 0, 400, 100, darkGrey, "Victory", 0, 0, 100, white, renderer);
    highScorePopUp.addBlock("", 0, 100, 400, 100, transparent, bestScoreLine, 10, 0, 60, white, renderer);
    highScorePopUp.addBlock("", 0, 200, 400, 100, blueTranparent, "Reset High Score", 20, 0, 80, white, renderer);

    scoreTxt.setColor(transparent);
    scoreTxt.addBlock("",0,0,200,100,transparent,"Score",30,0,100,white,renderer);
    scoreTxt.addBlock("scoreOnlyNum",0,100,200,100,transparent,"0",20,0,150,white,renderer);

    highScoreTxt.setColor(transparent);
    highScoreTxt.addBlock("",0,0,200,100,transparent,"High Score",0,20,80,white, renderer);
    highScoreTxt.addBlock("scoreOnlyNum",0,100,200,100,transparent,to_string(highScore), 20, 0, 150, white, renderer);

    failPopUp.setColor(lightGrey);
    failPopUp.addBlock("failTitle",0,0,400,75,darkGrey,"You lose!",100,0,80,white, renderer);
    failPopUp.addBlock("score",0,75,400,75,transparent,"Your score: 0",80,0,70,white, renderer);
    failPopUp.addBlock("",0,140,400,75,transparent,"SPACE to play again",70,10,50,red, renderer);
    failPopUp.addBlock("",100,215,200,75,transparent,"Choose song",0,0,80,white, renderer);

    autoPlay = Block("", 795, 800, 160, 100, blueTranparent, "AutoPlay", 15, 10, 70, white, renderer, ratio);

    speedTxt = Block("",50,800,150,100,blueTranparent,"Speed",25,10,70,white, renderer, ratio);
    speedPopUp.setColor(lightGrey);
    speedPopUp.addBlock("",0,0,150,50,lightGrey,"0.5",10,0,60,white, renderer);
    speedPopUp.addBlock("",0,50,150,50,lightGrey,"1",10,0,60,white, renderer);
    speedPopUp.addBlock("",0,100,150,50,white,"1.5",10,0,60,black, renderer);
    speedPopUp.addBlock("",0,150,150,50,lightGrey,"2",10,0,60,white, renderer);
    speedPopUp.addBlock("",0,200,150,50,lightGrey,"2.5",10,0,60,white, renderer);

    chooseSongPopUp.setColor(lightGrey);
    chooseSongPopUp.addBlock("title", 0, 0, 400, 100, transparent, "Song List", 85, -5, 100, white, renderer);
    for (int i = 0; i < songCnt; i++)
        chooseSongPopUp.addBlock("", 0, 100 + i * 75, 400, 75, darkGrey, song[i], 10, 0, 60, white, renderer);
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
    score = 0;
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
            break;
        }
        chooseSpeed(x, y, mouseUp, this);
        chooseSongButton(x, y, mouseUp, this);
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
        tileList[curTileID].handleInput(3, fail, scoreTxt, highScoreTxt, failPopUp, this);
}

void Game::update(){
    if (menu.isShown)
        return;
    camera.update(ratio);
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
