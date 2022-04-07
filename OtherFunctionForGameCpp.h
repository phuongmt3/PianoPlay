#pragma once
#include "Game.h"

Mix_Chunk* AudioManager::notesList[14][8];
Mix_Chunk* AudioManager::winnerChunk;

Uint32 Tile::curTick;

int cur1stSongInList = 1;//chuyen thanh static trong Game

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

void addTile(int songID, Game* game)
{
    game->curSongId = songID; //songID = 6;
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
        for (int bass = 0; bass < 2; bass++) {
            channel = 0;
            while (pos < int(s.length()) && s[pos] != ',') {
                if (!isChar(s[pos])) {
                    string note = "";
                    while (!isChar(s[pos]) && pos < int(s.length()))
                        note += s[pos], pos++;
                    curTile->setNote(note, channel, consecutiveNotes, bass);
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
                    AudioManager::playNote("A" + to_string(i + 2), -1, 2000);
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
    else if (event == mouseUp) {
        if (game->fail && !game->chooseSongPopUp.isShown && inside(x, y, game->failPopUp.container[3].bloR))
            game->chooseSongPopUp.isShown = 1;
        else if (!game->menu.isShown || !inside(x, y, game->chooseSongPopUp.desR)) {
            game->chooseSongPopUp.isShown = 0; cur1stSongInList = 1;
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
                AudioManager::playNote("C4", -1, 2000);
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
}

void menuHandle(const int& x, const int& y, sdlEvent event, Game* game) {
    if (event == mouseMotion && !game->manual.isShown &&
        !game->highScorePopUp.isShown && !game->chooseSongPopUp.isShown) {
        for (int i = 2; i <= 6; i++) {
            if (inside(x, y, game->menu.container[i].bloR)) {
                game->menu.container[i].setColor(white);
                game->menu.container[i].setText(-1, black);
            }
            else {
                game->menu.container[i].setColor(blueTranparent);
                game->menu.container[i].setText(-1, white);
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
            game->highScorePopUp.container[2].setText(-1, black);
        }
        else {
            game->highScorePopUp.container[2].setColor(blueTranparent);
            game->highScorePopUp.container[2].setText(-1, white);
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
            AudioManager::playNote("A2", -1, 1000);
        }
}