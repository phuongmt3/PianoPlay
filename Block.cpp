#include "Game.h"

Block::Block(){}
Block::Block(const string& _name, int blox, int bloy, int blow, int bloh, Color blockColor,
          const string& _text, int x, int y, int fontSize, Color textColor, SDL_Renderer* Orenderer, double _ratio){
    renderer = Orenderer; ratio = _ratio;
    bloR = {int(blox * ratio), int(bloy * ratio), int(blow * ratio), int(bloh * ratio)};
    colorType = blockColor;
    content = Text(_text, (x * ratio + bloR.x)/ratio + 1, (y * ratio + bloR.y)/ratio + 1, fontSize, textColor, renderer, ratio);
    name = _name;
}

void Block::setText(int fontType, Color colorType){
    content.updateColor(colorType);
    //content.updateFont(fontType);
}

void Block::show(){
    SDL_SetRenderDrawColor(renderer, colorList[colorType].r,
        colorList[colorType].g,
        colorList[colorType].b,
        colorList[colorType].a);
    SDL_RenderFillRect(renderer, &bloR);
    content.show();
}

void Block::setColor(Color newColor) {
    colorType = newColor;
}

void Block::update(const Game* game){
    if (name == "score"){
        content.updateText("Your score: " + to_string(game->score));
    }
    else if (name == "scoreOnlyNum"){
        string test = to_string(game->score);
        content.updateText(test);
    }
    else if (name == "failTitle"){
        if (game->score == game->highScore && game->score > 0) {
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