#include "Game.h"

Block::Block(){}
Block::Block(const string& _name, int blox, int bloy, int blow, int bloh, Color blockColor,
          const string& _text, int x, int y, int fontSize, Color textColor){
    bloR = {blox, bloy, blow, bloh};
    colorType = blockColor;
    content = Text(_text, x + blox, y + bloy, fontSize, textColor);
    name = _name;
}

void Block::setText(int fontType, Color colorType){
    content.updateColor(colorType);
    //content.updateFont(fontType);
}

void Block::show(){
    SDL_SetRenderDrawColor(Global::renderer, colorList[colorType].r,
        colorList[colorType].g,
        colorList[colorType].b,
        colorList[colorType].a);
    SDL_RenderFillRect(Global::renderer, &bloR);
    content.show();
}

void Block::setColor(Color newColor) {
    colorType = newColor;
}

void Block::update(){
    if (name == "score"){
        content.updateText("Score: " + to_string(Global::score));
    }
    else if (name == "scoreOnlyNum"){
        string test = to_string(Global::score);
        content.updateText(test);
    }
    else if (name == "failTitle"){
        if (Global::score == Global::highScore && Global::score > 0) {
            content.updateText("High Score!!");
            setColor(blue);
        }
        else {
            content.updateText("You lose!");
            setColor(darkGrey);
        }
    }
}

void Block::changePos(int x, int y) {
    bloR.x += x; content.desR.x += x;
    bloR.y += y; content.desR.y += y;
}