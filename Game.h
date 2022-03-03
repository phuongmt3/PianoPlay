#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#pragma once
#include <iostream>
#include <SDL.h>
#include <ctime>
#include "SDL_image.h"
using namespace std;//currently resources is not sync

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 800;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;//why event cannot be a pointer

class TextureManager{
public:
    static SDL_Texture* takeTexture(const char* link){
        SDL_Surface* bg = IMG_Load(link);
        if (!bg)
            printf("IMG_Load: %s\n", IMG_GetError());
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bg);
        SDL_FreeSurface(bg);
        return texture;
    }
    static void drawImage(const char* link, SDL_Rect src, SDL_Rect des){
        SDL_RenderCopy(renderer,takeTexture(link),&src,&des);
    }
};

class Title
{
public:
    Title(int width, int height){
        w = width; h = height;
    }
    void setPos(){
        pos = rand()%4; std::cout << pos << '\n';
        srcR.h = desR.h = h;
        srcR.w = desR.w = w;
        srcR.x = srcR.y = 0;
        desR.x = pos * w;
        desR.y = 600;
    }
    void show(bool untouched){
        if (untouched)
            TextureManager::drawImage("pianoHub/titleUntouched.png", srcR, desR);
    }

private:
    int w, h;
    int pos;
    SDL_Rect srcR, desR;
};

void init(const char* title, int xpos, int ypos,
              int width, int height, bool fullscreen, bool& isRunning){
    int flag = 0;
    if (fullscreen)
        flag = SDL_WINDOW_FULLSCREEN;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        cout << "Init completed!\n";
        window = SDL_CreateWindow(title,xpos,ypos,width,height,flag);
        if (window){
            cout << "Created window!\n";
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawColor(renderer,200,180,150,255);
        if (renderer)
            cout << "Created renderer\n";
        else cout << "Cannot create renderer\n";
        isRunning = 1;
    }
    else isRunning = 0;
}

void render(){
    SDL_RenderClear(renderer);
    SDL_Rect srcR = {0,0,1080,2052}, desR = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
    TextureManager::drawImage("pianoHub/piano.png", srcR, desR);
    SDL_RenderPresent(renderer);
}

void handleInput(bool& isRunning){
    SDL_PollEvent(&event);
    switch(event.type)
    {
    case SDL_QUIT:
        isRunning = 0; break;
    default: break;
    }
}

void update(){}

void clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    cout << "Game clean!";
}

#endif // GAME_H_INCLUDED
