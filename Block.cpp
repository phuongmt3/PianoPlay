#include "Game.h"

Block::Block(){}
Block::Block(const string& _name, int blox, int bloy, int blow, int bloh,
          const string& _text, int x, int y, int w, int h){
    bloR = {blox, bloy, blow, bloh};
    content = Text(_text, x + blox, y + bloy, w, h);
    name = _name;
}

void Block::setText(int font, int color){}

void Block::show(){
    SDL_SetRenderDrawColor(Global::renderer,50,50,50,255);
    SDL_RenderFillRect(Global::renderer, &bloR);
    content.show();
}

void Block::update(){
    if (name == "score"){
        content.updateText("Score: " + to_string(Global::score), -1);
    }
}
