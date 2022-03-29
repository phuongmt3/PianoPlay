#include "Game.h"

PopUp::PopUp(int x, int y, int w, int h, double _ratio){
    ratio = _ratio;
    desR = {int(x * ratio), int(y * ratio), int(w * ratio), int(h * ratio) };
    limitMoveUpValue = 0; limitMoveDownValue = h * ratio;
}

void PopUp::setLimit(int limitUp, int limitDown) {
    if (limitUp != -1)
        limitMoveUpValue = limitUp * ratio;
    if (limitDown != -1)
        limitMoveDownValue = limitDown * ratio;
}

void PopUp::addBlock(const string& _name, int blox, int bloy, int blow, int bloh, Color blockColor,//block so voi Popup, text sv block
      const string& _text, int x, int y, int fontSize, Color textColor, SDL_Renderer* Orenderer){
    container.push_back(Block(_name,(blox * ratio + desR.x)/ratio + 1, (bloy * ratio + desR.y)/ratio + 1,blow,bloh,blockColor,
                            _text,x,y,fontSize,textColor,Orenderer,ratio));
}
void PopUp::update(const Game* game){
    for (auto& i: container)
        i.update(game);
}
void PopUp::setColor(Color newColor) {
    colorType = newColor;
}
void PopUp::show(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, colorList[colorType].r,
        colorList[colorType].g,
        colorList[colorType].b,
        colorList[colorType].a);
    SDL_RenderFillRect(renderer, &desR);
    for (int i = 0; i < container.size(); i++)
        if (visibleBlock(i))
            container[i].show();
}
bool PopUp::visibleBlock(int i) {
    if (container[i].getName() == "title")
        return 1;
    if (takeY_BasePopUp(i) >= limitMoveUpValue &&
        takeY_BasePopUp(i) + container[i].bloR.h <= limitMoveDownValue)
        return 1;
    return 0;
}
int PopUp::takeY_BasePopUp(int i) {
    return container[i].bloR.y - desR.y;
}