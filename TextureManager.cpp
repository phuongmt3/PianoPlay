#include "Game.h"

SDL_Texture* TextureManager::takeTexture(const char* link, SDL_Renderer* renderer){
    SDL_Surface* bg = IMG_Load(link);
    if (!bg)
        printf("IMG_Load: %s\n", IMG_GetError());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bg);
    SDL_FreeSurface(bg);
    return texture;
}

void TextureManager::drawImage(SDL_Texture* tex, SDL_Rect src, SDL_Rect des, SDL_Renderer* renderer){
    SDL_RenderCopy(renderer,tex,&src,&des);
}
