#pragma once
#include "Game.h"

Mix_Chunk* AudioManager::notesList[14][8];
Mix_Chunk* AudioManager::winnerChunk;
Mix_Music* AudioManager::menuMusic;

Uint32 Tile::curTick;

enum sdlEvent {
    mouseMotion, mouseUp, mouseDown, mouseWheel
};

int songCnt = 10;
string song[] = {
        "LittleStar",
        "MyAll_AyumiHamasaki",
        "YoruNiKakeru_Yoasobi",
        "ToLove'sEnd_Inuyasa",
        "3107_3",
        "ToiThayHoaVangTrenCoXanh",
        "DemonSlayer",
        "MelodyOfTheNight5_ShiJin",
        "KhongBang",
        "draft"
};

bool isChar(char c)
{
    if (c == ',' || c == '-' || c == '+' || c == '=')
        return 1;
    return 0;
}

void setBlocks(Game* game) {
    SDL_Renderer* renderer = game->renderer;
    game->scoreTxt = PopUp({ 20, 20, 200, 300 }, game->ratio);
    game->highScoreTxt = PopUp({ 770, 20, 200, 300 }, game->ratio);
    game->failPopUp = PopUp({ 300, 300, 400, 300 }, game->ratio);
    game->speedPopUp = PopUp({ 50, 500, 150, 303 }, game->ratio);
    game->chooseSongPopUp = PopUp({ 300, 200, 400, 480 }, game->ratio);
    game->menu = PopUp({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT }, game->ratio);
    game->manual = PopUp({ 150, 200, 700, 600 }, game->ratio);
    game->highScorePopUp = PopUp({ 300, 200, 400, 400 }, game->ratio);

    game->menu.setColor(transparent);
    game->menu.addBlock("title", { 300, 30, 300, 100 }, transparent, "Piano Tiles", 0, 0, 140, 0, white, renderer);
    game->menu.addBlock("title", { 250, 170, 500, 100 }, transparent, "Anyone can be a piano pro...", 0, 0, 60, 1, white, renderer);
    game->menu.addBlock("", { 330, 300, 300, 100 }, blueTranparent, "Play", 100, 0, 80, 0, white, renderer);
    game->menu.addBlock("", { 330, 400, 300, 100 }, blueTranparent, "Choose Song", 40, 0, 80, 0, white, renderer);
    game->menu.addBlock("", { 330, 500, 300, 100 }, blueTranparent, "High Score", 50, 0, 80, 0, white, renderer);
    game->menu.addBlock("", { 330, 600, 300, 100 }, blueTranparent, "Help", 100, 0, 80, 0, white, renderer);
    game->menu.addBlock("", { 330, 700, 300, 100 }, blueTranparent, "Exit", 100, 0, 80, 0, white, renderer);

    game->manual.setColor(lightGrey);
    game->manual.addBlock("", { 0, 0, 700, 100 }, darkGrey, "User manual", 200, 0, 100, 0, white, renderer);
    game->manual.addBlock("", { 0, 100, 700, 50 }, transparent, "You just got stucked then now come to see me, right?", 10, 0, 40, 2, white, renderer);
    game->manual.addBlock("", { 0, 150, 700, 50 }, transparent, "Anyway, I will forgive you for that...", 10, 0, 40, 2, white, renderer);
    game->manual.addBlock("", { 0, 230, 700, 50 }, transparent, "Press SPACE to start, pause and continue game.", 10, 0, 40, 2, white, renderer);
    game->manual.addBlock("", { 0, 280, 700, 50 }, transparent, "Press F, G, H, J conrresponding to postion of tiles.", 10, 0, 40, 2, white, renderer);
    game->manual.addBlock("", { 0, 330, 700, 50 }, transparent, "Default speed is 1.5, but you can change it based on your", 10, 0, 40, 2, white, renderer);
    game->manual.addBlock("", { 0, 380, 700, 50 }, transparent, " reference.", 10, 0, 40, 2, white, renderer);
    game->manual.addBlock("", { 0, 430, 700, 50 }, transparent, "Choose AutoPlay to listen to music without playing, but", 10, 0, 40, 2, white, renderer);
    game->manual.addBlock("", { 0, 480, 700, 50 }, transparent, " you won't get any score.", 10, 0, 40, 2, white, renderer);
    game->manual.addBlock("", { 0, 530, 700, 50 }, transparent, "The last thing, press Esc to back to menu.", 10, 0, 40, 2, white, renderer);

    game->highScorePopUp.setColor(lightGrey);
    string bestScoreLine = "Best Score: " + to_string(game->highScore);
    game->highScorePopUp.addBlock("", { 0, 0, 400, 100 }, darkGrey, "Victory", 100, 0, 100, 0, white, renderer);
    game->highScorePopUp.addBlock("", { 0, 125, 400, 100 }, transparent, bestScoreLine, 30, 0, 60, 1, white, renderer);
    game->highScorePopUp.addBlock("", { 0, 200, 400, 100 }, blueTranparent, "Reset High Score", 40, 0, 80, 0, white, renderer);

    game->scoreTxt.setColor(transparent);
    game->scoreTxt.addBlock("", { 0,0,200,100 }, transparent, "Score", 30, 0, 100, 0, white, renderer);
    game->scoreTxt.addBlock("scoreOnlyNum", { 0,100,200,100 }, transparent, "0", 15, 0, 150, 0, white, renderer);

    game->highScoreTxt.setColor(transparent);
    game->highScoreTxt.addBlock("", { 0,0,200,100 }, transparent, "High Score", 0, 20, 80, 0, white, renderer);
    game->highScoreTxt.addBlock("scoreOnlyNum", { 0,100,200,100 }, transparent, to_string(game->highScore), 10, 0, 150, 0, white, renderer);

    game->failPopUp.setColor(lightGrey);
    game->failPopUp.addBlock("failTitle", { 0,0,400,75 }, darkGrey, "You lose!", 100, 0, 80, 0, white, renderer);
    game->failPopUp.addBlock("score", { 0,75,400,75 }, transparent, "Your score: 0", 80, 0, 70, 0, white, renderer);
    game->failPopUp.addBlock("", { 0,140,400,75 }, transparent, "SPACE to play again", 70, 10, 50, 0, yellow, renderer);
    game->failPopUp.addBlock("", { 90,215,220,75 }, transparent, "Choose song", 10, 0, 80, 0, white, renderer);

    game->autoPlay = Block("", { 795, 800, 160, 100 }, blueTranparent, "AutoPlay", 15, 10, 70, 0, white, renderer, game->ratio);

    game->speedTxt = Block("", { 50,800,150,100 }, blueTranparent, "Speed", 25, 10, 70, 0, white, renderer, game->ratio);
    game->speedPopUp.setColor(lightGrey);
    game->speedPopUp.addBlock("", { 0,0,150,50 }, lightGrey, "0.5", 60, -5, 60, 0, white, renderer);
    game->speedPopUp.addBlock("", { 0,50,150,50 }, lightGrey, "1", 70, -5, 60, 0, white, renderer);
    game->speedPopUp.addBlock("", { 0,100,150,50 }, white, "1.5", 60, -5, 60, 0, black, renderer);
    game->speedPopUp.addBlock("", { 0,150,150,50 }, lightGrey, "2", 70, -5, 60, 0, white, renderer);
    game->speedPopUp.addBlock("", { 0,200,150,50 }, lightGrey, "2.5", 60, -5, 60, 0, white, renderer);
    game->speedPopUp.addBlock("", { 0,250,150,50 }, lightGrey, "Auto", 50, 0, 45, 0, white, renderer);

    game->chooseSongPopUp.setColor(lightGrey);
    game->chooseSongPopUp.addBlock("title", { 0, 0, 400, 100 }, transparent, "Song List", 85, -5, 100, 0, white, renderer);
    for (int i = 0; i < songCnt; i++)
        game->chooseSongPopUp.addBlock("", { 0, 100 + i * 75, 400, 75 }, darkGrey, song[i], 10, 10, 50, 1, white, renderer);
    game->chooseSongPopUp.setLimit(100, -1);
}

void addTile(int songID, Game* game)
{
    game->curSongId = songID;
    ifstream fin("PianoPlay/pianoHub/" + song[songID] + ".txt");
    fin >> game->tileCount;
    string s;
    game->tileList.clear();
    for (int i = 0; i < game->tileCount; i++) {
        fin >> s;
        if (i > 0)
            game->tileList.push_back(Tile(GAME_WIDTH / 4, GAME_HEIGHT / 4, i, game->tileList[i - 1].takePos(), game->ratio));
        else
            game->tileList.push_back(Tile(GAME_WIDTH / 4, GAME_HEIGHT / 4, i, 4, game->ratio));
        Tile* curTile = &game->tileList.back();

        int pos = 0, channel = 0;
        int consecutiveNotes = 0;
        for (int type = 0; type < 2; type++) {
            channel = 0;
            while (pos < int(s.length()) && s[pos] != ',') {
                if (!isChar(s[pos])) {
                    string note = "";
                    while (!isChar(s[pos]) && pos < int(s.length()))
                        note += s[pos], pos++;
                    curTile->setNote(note, channel, consecutiveNotes, type);
                }
                else if (s[pos] == '-') {
                    consecutiveNotes += 2; consecutiveNotes = min(consecutiveNotes, 3);
                    pos++; channel = 0;
                }
                else if (s[pos] == '=') {
                    consecutiveNotes++;
                    pos++; channel = 0;
                }
                else if (s[pos] == '+') {
                    channel++; pos++;
                }
            }
            pos++; consecutiveNotes = 0;
        }
    }
}

void addSound()
{
    AudioManager::winnerChunk = Mix_LoadWAV("PianoPlay/pianoHub/piano-mp3/mixkit-male-voice-cheer-2010.wav");
    AudioManager::menuMusic = Mix_LoadMUS("PianoPlay/pianoHub/piano-mp3/Beautiful-Piano.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    Mix_PlayMusic(AudioManager::menuMusic, -1);

    for (int chu = 0; chu < 14; chu++)
        for (int so = 0; so < 8; so++) {
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
            game->speedTxt.setColor(white), game->speedTxt.setText(black);
        else if (!game->speedPopUp.isShown)
            game->speedTxt.setColor(blueTranparent), game->speedTxt.setText(white);
    }
    else if (event == mouseDown) {
        if (game->speedPopUp.isShown) {
            for (int i = 0; i < 6; i++) {
                if (inside(x, y, game->speedPopUp.container[i].bloR)) {
                    if (i == 5)
                        game->camera.autoSpeed = 1, game->camera.speed = 1.2;
                    else
                        game->camera.speed = 0.5 + 0.5 * i;
                    game->speedPopUp.container[i].setColor(white);
                    game->speedPopUp.container[i].setText(black);
                    AudioManager::playNote("A" + to_string(i + 1), -1, 2000);
                }
                else if (inside(x, y, game->speedPopUp.desR)) {
                    game->speedPopUp.container[i].setColor(lightGrey);
                    game->speedPopUp.container[i].setText(white);
                    if (i == 5)
                        game->camera.autoSpeed = 0;
                }
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
                    game->chooseSongPopUp.container[i].setText(black);
                }
                else {
                    game->chooseSongPopUp.container[i].setColor(darkGrey);
                    game->chooseSongPopUp.container[i].setText(white);
                }
            }
        }
    }
    else if (event == mouseDown) {
        if (game->chooseSongPopUp.isShown) {
            for (int i = 1; i <= songCnt; i++)
                if (game->chooseSongPopUp.visibleBlock(i) && inside(x, y, game->chooseSongPopUp.container[i].bloR)) {
                    game->menu.isShown = 0; game->fail = 0;
                    addTile(i - 1, game);
                    game->curTileID = 0; game->lastSeenID = 0;
                }
            if (!inside(x, y, game->chooseSongPopUp.desR))
                game->chooseSongPopUp.isShown = 0;
        }
    }
    else if (event == mouseWheel) {
        if (inside(x, y, game->chooseSongPopUp.desR)) {
            if (e.wheel.y < 0 && game->cur1stSongInList + 4 < songCnt) {
                game->cur1stSongInList++;
                int step = game->chooseSongPopUp.takeY_BasePopUp(game->cur1stSongInList) - game->chooseSongPopUp.limitMoveUp();
                for (int i = 1; i <= songCnt; i++)
                    game->chooseSongPopUp.container[i].changePos(0, -step);
            }
            else if (e.wheel.y > 0 && game->cur1stSongInList > 1) {
                game->cur1stSongInList--;
                int step = game->chooseSongPopUp.limitMoveUp() - game->chooseSongPopUp.takeY_BasePopUp(game->cur1stSongInList);
                for (int i = 1; i <= songCnt; i++)
                    game->chooseSongPopUp.container[i].changePos(0, step);
            }
        }
    }
    else if (event == mouseUp) {
        if (game->fail && !game->chooseSongPopUp.isShown && inside(x, y, game->failPopUp.container[3].bloR))
            game->chooseSongPopUp.isShown = 1;
        else if (!game->menu.isShown || !inside(x, y, game->chooseSongPopUp.desR)) {
            game->chooseSongPopUp.isShown = 0; game->cur1stSongInList = 1;
            int comeback = game->chooseSongPopUp.limitMoveUp() - game->chooseSongPopUp.takeY_BasePopUp(1);
            if (comeback > 0)
                for (int i = 1; i <= songCnt; i++)
                    game->chooseSongPopUp.container[i].changePos(0, comeback);
        }
    }
}

void autoPlayHandle(const int& x, const int& y, sdlEvent event, Game* game) {
    if (event == mouseMotion) {
        if (!game->autoPlay.isShown) {
            if (inside(x, y, game->autoPlay.bloR)) {
                game->autoPlay.setColor(white);
                game->autoPlay.setText(black);
            }
            else {
                game->autoPlay.setColor(blueTranparent);
                game->autoPlay.setText(white);
            }
        }
    }
    else if (event == mouseUp) {
        if (inside(x, y, game->autoPlay.bloR)) {
            if (!game->autoPlay.isShown) {
                game->autoPlay.isShown = 1;
                game->autoPlay.setColor(black);
                game->autoPlay.setText(red);
                AudioManager::playNote("C4", -1, 2000);
            }
            else {
                game->autoPlay.isShown = 0; game->camera.stop = 1;
                game->autoPlay.setColor(white);
                game->autoPlay.setText(black);
            }
        }
    }
}

void chooseSongButton(const int& x, const int& y, sdlEvent event, Game* game) {
    if (event == mouseMotion) {
        if (game->fail && inside(x, y, game->failPopUp.container[3].bloR))
            game->failPopUp.container[3].setColor(white), game->failPopUp.container[3].setText(black);
        else
            game->failPopUp.container[3].setColor(transparent), game->failPopUp.container[3].setText(white);
    }
}

void menuHandle(const int& x, const int& y, sdlEvent event, Game* game) {
    if (event == mouseMotion && !game->manual.isShown &&
        !game->highScorePopUp.isShown && !game->chooseSongPopUp.isShown) {
        for (int i = 2; i <= 6; i++) {
            if (inside(x, y, game->menu.container[i].bloR)) {
                game->menu.container[i].setColor(white);
                game->menu.container[i].setText(black);
            }
            else {
                game->menu.container[i].setColor(blueTranparent);
                game->menu.container[i].setText(white);
            }
        }
    }
    else if (event == mouseUp && !game->manual.isShown &&
        !game->highScorePopUp.isShown && !game->chooseSongPopUp.isShown) {
        if (inside(x, y, game->menu.container[2].bloR)) {
            game->menu.isShown = 0;
            int nextID = rand() % songCnt;
            nextID == game->curSongId ? addTile((nextID + 1) % songCnt, game) : addTile(nextID, game);
            AudioManager::playNote("C5", -1, 2000);
        }
        else if (inside(x, y, game->menu.container[3].bloR)) {
            game->chooseSongPopUp.isShown = 1;
            AudioManager::playNote("E5", -1, 2000);
        }
        else if (inside(x, y, game->menu.container[4].bloR)) {
            game->highScorePopUp.isShown = 1;
            AudioManager::playNote("G5", -1, 2000);
        }
        else if (inside(x, y, game->menu.container[5].bloR)) {
            game->manual.isShown = 1;
            AudioManager::playNote("A5", -1, 2000);
        }
        else if (inside(x, y, game->menu.container[6].bloR)) {
            game->isRunning = 0;
        }
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
    if (event == mouseMotion) {
        if (inside(x, y, game->highScorePopUp.container[2].bloR)) {
            game->highScorePopUp.container[2].setColor(white);
            game->highScorePopUp.container[2].setText(black);
        }
        else {
            game->highScorePopUp.container[2].setColor(blueTranparent);
            game->highScorePopUp.container[2].setText(white);
        }
    }
    else if (event == mouseDown) {
        if (game->highScorePopUp.isShown)
            if (!inside(x, y, game->highScorePopUp.desR))
                game->highScorePopUp.isShown = 0;
    }
    else if (game->highScorePopUp.isShown && event == mouseUp)
        if (inside(x, y, game->highScorePopUp.container[2].bloR)) {
            game->highScore = 0;
            game->highScoreTxt.update(game);
            game->highScorePopUp.container[1].content.updateText("Best Score: 0");
            ofstream fout("PianoPlay/pianoHub/highscore.txt");
            fout << game->highScore;
            AudioManager::playNote("A2", -1, 2000);
        }
}