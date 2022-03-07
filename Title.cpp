#include "Game.h"

void Title::setPos()
{
    pos = rand()%4; std::cout << pos << '\n';
    srcR.h = desR.h = h;
    srcR.w = desR.w = w;
    srcR.x = srcR.y = 0;
    desR.x = pos * w;
    desR.y = 600;
}

void Title::show(bool untouched)
{
    if (untouched)
        TextureManager::drawImage("pianoHub/tileUntouched.png", srcR, desR);
}
