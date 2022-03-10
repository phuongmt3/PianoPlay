#include "Game.h"

Tile::Tile(int width, int height, int stt, int prePos)
{
    w = width; h = height;
    pos = rand()%4;
    touched = 0;
    fill(runSecondTimeForChannel, runSecondTimeForChannel + 6, 0);
    if (pos == prePos)
        pos = (pos + 1)%4;
    srcR.h = desR.h = h;
    srcR.w = desR.w = w;
    srcR.x = srcR.y = 0;
    desR.x = pos * w;
    desR.y = -h - stt * h;
}

void Tile::setNote(string _note, int channel, bool isSecond, int isBass)
{
    if (!isBass){
        if (!isSecond)
            note[channel][0] = _note;
        else note[channel][1] = _note;
    }
    else{
        if (!isSecond)
            bass[channel][0] = _note;
        else bass[channel][1] = _note;
    }
}

void Tile::show()
{
    if (!touched)
        TextureManager::drawImage(Global::unclick, srcR, desR);
    else
        TextureManager::drawImage(Global::click, srcR, desR);
}

void Tile::handleInput(int posInput, bool& isRunning)
{
    cout << Global::curTileID << endl;
    if (posInput == pos){
        touched = 1, Global::curTileID++;
        for (int channel = 0; channel < 3; channel++){
            if (note[channel][0] != ""){
                if (note[channel][1] == "")
                    AudioManager::playNote(note[channel][0], channel, 0), cout<<note[channel][0]<<'\n';
                else
                    AudioManager::playNote(note[channel][0], channel, waitingTimeForSecondNote),
                    runSecondTimeForChannel[channel] = 1;
            }
            else if (note[channel][1] != "")
                runSecondTimeForChannel[channel] = 1;
            if (bass[channel][0] != ""){
                if (bass[channel][1] == "")//channel nums
                    AudioManager::playNote(bass[channel][0], channel + 3, 0);
                else
                    AudioManager::playNote(bass[channel][0], channel + 3, waitingTimeForSecondNote),
                    runSecondTimeForChannel[channel + 3] = 1;
            }
            else if (bass[channel][1] != "")
                runSecondTimeForChannel[channel + 3] = 1;
        }
    }
    else{
        AudioManager::playNote("A0", 0, 0);
        isRunning = 0, cout << "You fail because of wrong key\n";
    }
}

void Tile::update(bool& isRunning)
{
    desR.y += Global::camera.y;
    if (desR.y > WINDOW_HEIGHT && !touched){
        //AudioManager::playNote("A0", 0, 0);
        //isRunning = 0, cout << "You fail because of untouched\n";
    }
    else if (desR.y + h < 0 && touched){
        AudioManager::playNote("A0", 0, 0);
        isRunning = 0;
    }
    else if (Global::curTileID == Global::tileCount){
        isRunning = 0; //cout << "You are winner!\n";//why keep running tileCount times??
        Mix_PlayChannel(3, AudioManager::winnerChunk, 0);
    }
    for (int channel = 0; channel < 6; channel++)//change num of channels
        if (!Mix_Playing(channel) && runSecondTimeForChannel[channel]){
            if (channel < 3)
                AudioManager::playNote(note[channel][1], channel, 0),
                runSecondTimeForChannel[channel] = 0;
            else
                AudioManager::playNote(bass[channel - 3][1], channel, 0),
                runSecondTimeForChannel[channel] = 0;
        }
}
