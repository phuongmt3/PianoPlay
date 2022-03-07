#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#pragma once
#include <iostream>
#include <SDL.h>
#include <ctime>
#include <vector>
#include "SDL_image.h"
using namespace std;//currently resources is not sync, change link variable

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 800;

class Global
{
public:
    static SDL_Renderer* renderer;
    static int curTileID;
    static SDL_Texture *click, *unclick, *bg;
};

class Camera
{
public:
    int y = 5, speed = 0;
    void update()
    {
        y += speed;
    }
};
static Camera camera;

class TextureManager
{
public:
    static SDL_Texture* takeTexture(const char* link);
    static void drawImage(SDL_Texture* tex, SDL_Rect src, SDL_Rect des);
};

class Tile
{
public:
    Tile(int width, int height, int stt);
    void show();
    void handleInput(int posInput, bool& isRunning);
    void update(bool& isRunning);
private:
    int w, h, pos;
    SDL_Rect srcR, desR;
    bool touched = 0;
};

void init(const char* title, int xpos, int ypos,
              int width, int height, bool fullscreen, bool& isRunning);
void render();
void handleInput(bool& isRunning);
void update(bool& isRunning);
void clean();

#endif // GAME_H_INCLUDED
