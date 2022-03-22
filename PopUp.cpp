#include "Game.h"

PopUp::PopUp(int x, int y, int w, int h){
    desR = {x, y, w, h};
    limitMoveUp = 0; limitMoveDown = h;
}

void PopUp::addBlock(const string& _name, int blox, int bloy, int blow, int bloh,//block so voi Popup, text sv block
      const string& _text, int x, int y, int w, int h){
    container.push_back(Block(_name,blox + desR.x, bloy + desR.y,blow,bloh,_text,x,y,w,h));
}
void PopUp::update(){
    for (auto& i: container)
        i.update();
}
void PopUp::show(){
    SDL_SetRenderDrawColor(Global::renderer,100,100,100,255);
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