#include "Game.h"

Tile::Tile(int width, int height, int stt)
{
    w = width; h = height;
    pos = rand()%4;
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
    if (posInput == pos)
        touched = 1, Global::curTileID++;
    else
        isRunning = 0, cout << "You fail because of wrong key\n";
}

void Tile::update(bool& isRunning)
{
    desR.y += camera.y;
    if (desR.y > WINDOW_HEIGHT && !touched)
        isRunning = 0, cout << "You fail because of untouched\n";
    else if (desR.y + h < 0 && touched)
        isRunning = 0;
}
