#include "Game.h"

Tile::Tile(int width, int height, int stt, int prePos, double _ratio)
{
    ratio = _ratio;
    w = width * ratio; h = height * ratio;
    pos = rand()%4;
    if (pos == prePos)
        pos = (pos + 1)%4;
    desR.h = h;
    desR.w = w;
    desR.x = pos * w + WINDOW_WIDTH * ratio/2 - GAME_WIDTH * ratio/2;
    desR.y = -h - stt * h;
}

void Tile::setNote(string _note, int channel, int consecutiveNotes, int isBass)
{
    if (!isBass)
        note[channel][consecutiveNotes] = _note;
    else
        bass[channel][consecutiveNotes] = _note;
}

void Tile::show(SDL_Renderer* renderer)
{
    if (!touched){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderFillRect(renderer, &desR);
    }
    else{
        SDL_SetRenderDrawColor(renderer,255,255,255,150);
        SDL_RenderFillRect(renderer, &desR);
    }
}

int Tile::duration(int channel, int curpos, bool isNote) {
    int cnt = 1, orgCurpos = curpos;
    if (isNote) {
        while (curpos < 3 && this->note[channel][++curpos] == "")
            cnt++;
        if (cnt + orgCurpos <= 4)
            runNextTimeForChannel[channel] = cnt + orgCurpos;
    }
    else {
        while (curpos < 3 && this->bass[channel][++curpos] == "")
            cnt++;
        if (cnt + orgCurpos <= 4)
            runNextTimeForChannel[channel + channelCount] = cnt + orgCurpos;
    }
    return cnt;
}

void Fail(int& fail, PopUp& failPopUp, Game* game) {
    AudioManager::playNote("A0", 0, 0);
    fail = 1;
    game->camera.stop = 1; game->lastSeenID = game->curTileID;
    failPopUp.update(game);
    game->score = 0;
}

void Tile::playNote(int channel, bool isNote, int noteLength, int notePos, const Game* game)
{
    Mix_Pause(channel);
    string curNote = isNote ? note[channel][notePos] : bass[channel - channelCount][notePos];
    if (curNote == "!");
    else if (runNextTimeForChannel[channel] == 4)
        AudioManager::playNote(curNote, channel, 0);
    else
        AudioManager::playNote(curNote, channel,
            game->waitingTimeForAQuarterNote * noteLength / game->camera.speed);
}

void Tile::rightFirstNote(Game* game, PopUp& highScoreTxt) {
    cout << SDL_GetTicks() - curTick << '\n';
    curTick = SDL_GetTicks();
    touched = 1; game->curTileID++;
    for (int channel = 0; channel < channelCount; channel++) {
        int noteLength = duration(channel, 0, 1);
        int bassLength = duration(channel, 0, 0);
        if (note[channel][0] != "")
            playNote(channel, 1, noteLength, 0, game);
        if (bass[channel][0] != "")
            playNote(channel + channelCount, 0, bassLength, 0, game);
    }
    if (game->score > game->highScore) {
        game->highScore = game->score;
        highScoreTxt.update(game);
        ofstream fout("PianoPlay/pianoHub/highscore.txt");
        fout << game->highScore;
    }
}

void Tile::handleInput(int posInput, int& fail, PopUp& scoreTxt, PopUp& highScoreTxt,
                                    PopUp& failPopUp, Game* game)
{
    cout << game->curTileID << '\n';
    if (game->autoPlay.isShown) {
        if (desR.y > 300 && !touched)
            rightFirstNote(game, highScoreTxt);
        return;
    }
    if (posInput == pos && desR.y + h >= 0){
        game->score++;
        rightFirstNote(game, highScoreTxt);
    }
    else {
        Fail(fail, failPopUp, game); cout << "You fail because of wrong key\n";
        game->showWrongKey = 1;
        game->wrongRect = {int(posInput * w + WINDOW_WIDTH * ratio/2 - GAME_WIDTH * ratio/2), desR.y, w, h};
    }
    scoreTxt.update(game);
}

void Tile::update(int& fail, int gobackLength, PopUp& scoreTxt, PopUp& highScoreTxt,
                   PopUp& failPopUp, int stt, Game* game)
{
    if (fail){
        desR.y -= gobackLength * ratio + 1;
        return;
    }
    desR.y += int(game->camera.y * game->camera.speed);
    if (desR.y > GAME_HEIGHT * ratio + 10 && !touched){
        Fail(fail, failPopUp, game); cout << "You fail because of untouched\n";
        desR.y -= (gobackLength + int(game->camera.y * game->camera.speed)) * ratio;
        scoreTxt.update(game);
    }
    if (stt != game->curTileID - 1)
        return;
    for (int channel = 0; channel < channelCount * 2; channel++)
        if (runNextTimeForChannel[channel] < 4){
            int notePos = runNextTimeForChannel[channel];
            if (!Mix_Playing(channel)) {
                if (channel < channelCount){
                    int noteLength = duration(channel, runNextTimeForChannel[channel], 1);
                    playNote(channel, 1, noteLength, notePos, game);
                }
                else{
                    int bassLength = duration(channel - channelCount, runNextTimeForChannel[channel], 0);
                    playNote(channel, 0, bassLength, notePos, game);
                }
            }
            else if (SDL_GetTicks() - curTick >= int(game->waitingTimeForAQuarterNote
                                            * runNextTimeForChannel[channel] / game->camera.speed)) {
                if (channel < channelCount && note[channel][0] == ""){
                    int noteLength = duration(channel, runNextTimeForChannel[channel], 1);
                    playNote(channel, 1, noteLength, notePos, game);
                }
                else if (channel >= channelCount && bass[channel - channelCount][0] == ""){
                    int bassLength = duration(channel - channelCount, runNextTimeForChannel[channel], 0);
                    playNote(channel, 0, bassLength, notePos, game);
                }
            }
    }
}