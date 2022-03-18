#include "Game.h"

Text::Text(){}
Text::Text(const string& _text, int x, int y, int w, int h)
{
    text = _text;
    desR = {x, y, w, h};
}

void Text::updateText(const string& newText, int _w)
{
    text = newText;
    if (_w != -1)
        desR.w = _w;
}

void Text::updateFont(int font)
{

}
void Text::updateColor(int color)
{

}
void Text::updateTexture()
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont,&text[0],textColor);
    texture = SDL_CreateTextureFromSurface(Global::renderer, textSurface);
    SDL_FreeSurface(textSurface);
}
void Text::show()
{
    SDL_RenderCopy(Global::renderer,texture,NULL,&desR);
}
