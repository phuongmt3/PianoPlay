#include "Game.h"

PopUp::PopUp(const SDL_Rect& rec, double _ratio){
    ratio = _ratio;
    desR = {int(rec.x * ratio), int(rec.y * ratio), int(rec.w * ratio), int(rec.h * ratio) };
    limitMoveUpValue = 0; limitMoveDownValue = desR.h;
}

void PopUp::setLimit(int limitUp, int limitDown) {
    if (limitUp != -1)
        limitMoveUpValue = limitUp * ratio;
    if (limitDown != -1)
        limitMoveDownValue = limitDown * ratio;
}

void PopUp::addBlock(const string& _name, const SDL_Rect& block, Color blockColor,//block so voi Popup, text sv block
      const string& _text, int x, int y, int fontSize, int fontType, Color textColor, SDL_Renderer* Orenderer){
    container.push_back(Block(_name, 
        { int((block.x * ratio + desR.x) / ratio + 1), int((block.y * ratio + desR.y) / ratio + 1), block.w, block.h },
        blockColor,_text,x,y,fontSize,fontType,textColor,Orenderer,ratio));
}
void PopUp::update(Game* game){
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