#include "Game.h"

Tile::Tile(int width, int height, int stt, int prePos)
{
    w = width; h = height;
    pos = rand()%4;
    if (pos == prePos)
        pos = (pos + 1)%4;
    desR.h = h;
    desR.w = w;
    desR.x = pos * w + WINDOW_WIDTH/2 - GAME_WIDTH/2;
    desR.y = -h - stt * h + WINDOW_HEIGHT/2 - GAME_HEIGHT/2;
}

void Tile::setNote(string _note, int channel, int consecutiveNotes, int isBass)
{
    if (!isBass)
        note[channel][consecutiveNotes] = _note;
    else
        bass[channel][consecutiveNotes] = _note;
}

void Tile::show()
{
    if (!touched){
        SDL_SetRenderDrawColor(Global::renderer,255,255,255,255);
        SDL_RenderFillRect(Global::renderer, &desR);
    }
    else{
        SDL_SetRenderDrawColor(Global::renderer,255,255,255,150);
        SDL_RenderFillRect(Global::renderer, &desR);
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

void Tile::handleInput(int posInput, int& fail, PopUp& scoreTxt, PopUp& highScoreTxt,
                                    PopUp& failPopUp)
{
    cout << Global::curTileID << '\n';
    if (posInput == pos && desR.y + h >= 0){
    //if (desR.y > 500 && !touched){
        cout << SDL_GetTicks() - curTick << '\n';
        curTick = SDL_GetTicks();
        touched = 1, Global::curTileID++; Global::score++;
        for (int channel = 0; channel < channelCount; channel++){
            int noteLength = duration(channel, 0, 1);
            int bassLength = duration(channel, 0, 0);
            if (note[channel][0] != "") {
                Mix_Pause(channel);
                if (note[channel][0] == "!");
                else if (noteLength == 4)//if (note[channel][1] == "")
                    AudioManager::playNote(note[channel][0], channel, 0);
                else
                    AudioManager::playNote(note[channel][0], channel,
                        Global::waitingTimeForSecondNote / Global::camera.speed * noteLength);
            }
            if (bass[channel][0] != ""){
                Mix_Pause(channel + channelCount);
                if (bass[channel][0] == "!");
                else if (bassLength == 4)//if (bass[channel][1] == "")
                    AudioManager::playNote(bass[channel][0], channel + channelCount, 0);
                else
                    AudioManager::playNote(bass[channel][0], channel + channelCount,
                        Global::waitingTimeForSecondNote / Global::camera.speed * bassLength);
            }
        }
    }
    else {
        AudioManager::playNote("A0", 0, 0);
        fail = 1, cout << "You fail because of wrong key\n";
        Global::camera.stop = 1; Global::lastSeenID = Global::curTileID;
        Global::showWrongKey = 1;
        Global::wrongRect = {posInput * w + WINDOW_WIDTH/2 - GAME_WIDTH/2, desR.y, w, h};

        if (Global::score > Global::highScore){
            Global::highScore = Global::score;
            highScoreTxt.update();
        }
        failPopUp.update();
        Global::score = 0;
    }
    string str = to_string(Global::score);
    scoreTxt.update();
}

void Tile::update(int& fail, int gobackLength, PopUp& scoreTxt, PopUp& highScoreTxt,
                   PopUp& failPopUp, int stt)
{
    if (fail){
        desR.y -= gobackLength;
        return;
    }
    desR.y += int(Global::camera.y * Global::camera.speed);
    if (desR.y > GAME_HEIGHT && !touched){
        AudioManager::playNote("A0", 0, 0);
        fail = 1, cout << "You fail because of untouched\n";
        Global::camera.stop = 1; Global::lastSeenID = Global::curTileID;
        desR.y -= (gobackLength + int(Global::camera.y * Global::camera.speed));

        if (Global::score > Global::highScore){
            Global::highScore = Global::score;
            highScoreTxt.update();
        }
        failPopUp.update();
        Global::score = 0;
        scoreTxt.update();
    }
    if (stt != Global::curTileID - 1)
        return;
    for (int channel = 0; channel < channelCount * 2; channel++)
        if (runNextTimeForChannel[channel] < 4){
            int notePos = runNextTimeForChannel[channel];
            if (!Mix_Playing(channel)) {
                if (channel < channelCount){
                    int noteLength = duration(channel, runNextTimeForChannel[channel], 1);
                    if (note[channel][notePos] == "!");
                    else if (runNextTimeForChannel[channel] == 4)
                        AudioManager::playNote(note[channel][notePos], channel, 0);
                    else
                        AudioManager::playNote(note[channel][notePos], channel,
                            Global::waitingTimeForSecondNote / Global::camera.speed * noteLength);
                }
                else{
                    int bassLength = duration(channel - channelCount, runNextTimeForChannel[channel], 0);
                    if (bass[channel - channelCount][notePos] == "!");
                    else if (runNextTimeForChannel[channel] == 4)
                        AudioManager::playNote(bass[channel - channelCount][notePos], channel, 0);
                    else
                        AudioManager::playNote(bass[channel - channelCount][notePos], channel,
                            Global::waitingTimeForSecondNote / Global::camera.speed * bassLength);
                }
            }
            else if (SDL_GetTicks() - curTick >= int(Global::waitingTimeForSecondNote / Global::camera.speed
                                            * runNextTimeForChannel[channel])) {
                if (channel < channelCount && note[channel][0] == ""){
                    Mix_Pause(channel);
                    int noteLength = duration(channel, runNextTimeForChannel[channel], 1);
                    if (note[channel][notePos] == "!");
                    else if (runNextTimeForChannel[channel] == 4)
                        AudioManager::playNote(note[channel][notePos], channel, 0);
                    else
                        AudioManager::playNote(note[channel][notePos], channel,
                            Global::waitingTimeForSecondNote / Global::camera.speed * noteLength);
                }
                else if (channel >= channelCount && bass[channel - channelCount][0] == ""){
                    Mix_Pause(channel);
                    int bassLength = duration(channel - channelCount, runNextTimeForChannel[channel], 0);
                    if (bass[channel - channelCount][notePos] == "!");
                    else if (runNextTimeForChannel[channel] == 4)
                        AudioManager::playNote(bass[channel - channelCount][notePos], channel, 0);
                    else
                        AudioManager::playNote(bass[channel - channelCount][notePos], channel,
                            Global::waitingTimeForSecondNote / Global::camera.speed * bassLength);
                }
            }
    }
}
//if (runNextNote() === bool)
//thi k p lap lai code