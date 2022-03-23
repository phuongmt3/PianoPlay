#include "Game.h"

PopUp::PopUp(int x, int y, int w, int h){
    desR = {x, y, w, h};
    limitMoveUp = 0; limitMoveDown = h;
}

void PopUp::addBlock(const string& _name, int blox, int bloy, int blow, int bloh, Color blockColor,//block so voi Popup, text sv block
      const string& _text, int x, int y, int fontSize, Color textColor){
    container.push_back(Block(_name,blox + desR.x, bloy + desR.y,blow,bloh,blockColor,
                            _text,x,y,fontSize,textColor));
}
void PopUp::update(){
    for (auto& i: container)
        i.update();
}
void PopUp::setColor(Color newColor) {
    colorType = newColor;
}
void PopUp::show(){
    SDL_SetRenderDrawColor(Global::renderer, colorList[colorType].r,
        colorList[colorType].g,
        colorList[colorType].b,
        colorList[colorType].a);
    SDL_RenderFillRect(Global::renderer, &desR);
    for (int i = 0; i < container.size(); i++)
        if (visibleBlock(i))
            container[i].show();
}
bool PopUp::visibleBlock(int i) {
    if (container[i].getName() == "title")
        return 1;
    if (takeY_BasePopUp(i) >= limitMoveUp &&
        takeY_BasePopUp(i) + container[i].bloR.h <= limitMoveDown)
        return 1;
    return 0;
}
int PopUp::takeY_BasePopUp(int i) {
    return container[i].bloR.y - desR.y;
}