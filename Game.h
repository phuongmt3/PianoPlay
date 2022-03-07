#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#pragma once
#include <iostream>
#include <SDL.h>
#include <ctime>
#include "SDL_image.h"
using namespace std;//currently resources is not sync, change link variable

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 800;

class Renderer
{
public:
    static SDL_Renderer* renderer;
};

class TextureManager
{
public:
    static SDL_Texture* takeTexture(const char* link);
    static void drawImage(const char* link, SDL_Rect src, SDL_Rect des);
};

class Title
{
public:
    Title(int width, int height){
        w = width; h = height;
    }
    void setPos();
    void show(bool untouched);

private:
    int w, h;
    int pos;
    SDL_Rect srcR, desR;
};

void init(const char* title, int xpos, int ypos,
              int width, int height, bool fullscreen, bool& isRunning);
void render();
void handleInput(bool& isRunning);
void update();
void clean();

#endif // GAME_H_INCLUDED
