#include "Game.h"

Tile::Tile(int width, int height, int stt, int prePos, string _note)
{
    w = width; h = height; note = _note;
    pos = rand()%4;
    if (pos == prePos)
        pos = (pos + 1)%4;
    srcR.h = desR.h = h;
    srcR.w = desR.w = w;
    srcR.x = srcR.y = 0;
    desR.x = pos * w;
    desR.y = -h - stt * h;
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
        AudioManager::playNote(note, 1);
    }
    else{
        AudioManager::playNote("A0", 1);
        isRunning = 0, cout << "You fail because of wrong key\n";
    }
}

void Tile::update(bool& isRunning)
{
    desR.y += Global::camera.y;
    if (desR.y > WINDOW_HEIGHT && !touched){
        AudioManager::playNote("A0", 1);
        isRunning = 0, cout << "You fail because of untouched\n";
    }
    else if (desR.y + h < 0 && touched){
        AudioManager::playNote("A0", 1);
        isRunning = 0;
    }
    else if (Global::curTileID == Global::tileCount){
        isRunning = 0; cout << "You are winner!\n";//why keep running tileCount times??
        Mix_PlayChannel(2, AudioManager::winnerChunk, 0);

    }
}
