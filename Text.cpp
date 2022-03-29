#include "Game.h"

Text::Text(){}
Text::Text(const string& _text, int x, int y, int fontSize, Color textColor, SDL_Renderer* Orenderer, double _ratio)
{
    renderer = Orenderer; ratio = _ratio;
    text = _text;
    updateFont(fontSize);
    updateTexture();
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    desR = {int(x * ratio), int(y * ratio), w, h};
    updateColor(textColor);
}

void Text::updateText(const string& newText)
{
    text = newText;
    updateTexture();
    int w;
    SDL_QueryTexture(texture, NULL, NULL, &w, NULL);
    desR.w = w;
    updateTexture();
}

void Text::updateFont(int fontSize)
{
    gFont = TTF_OpenFont("PianoPlay/TTFfonts/ENDORALT.ttf", fontSize * ratio);
    updateTexture();
}
void Text::updateColor(Color type)
{
    textColor = colorList[type];
    updateTexture();
}
void Text::updateTexture()
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont,&text[0],textColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}
void Text::show()
{
    SDL_RenderCopy(renderer,texture,NULL,&desR);
}
